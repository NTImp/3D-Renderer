#include "Render3D.hpp"
#include "Structs.hpp"

#include <vector>
#include <algorithm>

extern Math::Vector4 lightDir;

extern Math::Matrix4 modelMatrix;
extern Math::Matrix4 viewMatrix;
extern Math::Matrix4 projMatrix;
extern Math::Matrix4 pvMatrix;
extern Math::Matrix4 vmMatrix;
extern Math::Matrix4 pvmMatrix;

//Clip plane
constexpr float cz = 0.1f;

//W and H coordinates of the screen for a faster use
extern int w, h;
extern float* depthBuffer;
extern Graphics::Screen* out;

namespace Render3D {
	extern CullType fcull;

	ProjOutTriangle projOut[maxTriangles];
	ProjOutTriangle clipOut[maxTriangles * 2];

	void RenderTriangles(std::vector<Triangle>& triangles)
	{
		int projCount = 0;
		for (auto& t : triangles) {
			ProjOutTriangle& ot = projOut[projCount++];
			ot = ProjOutTriangle(t);


			//Calculates light
			Math::Vector4 extN(t.t[0].normal, 0);
			extN = Math::mul(modelMatrix, extN);
			ot.t[0].light = std::max(0.2f, Math::dot(extN, lightDir));

			extN = Math::Vector4(t.t[1].normal, 0);
			extN = Math::mul(modelMatrix, extN);
			ot.t[1].light = std::max(0.2f, Math::dot(extN, lightDir));

			extN = Math::Vector4(t.t[2].normal, 0);
			extN = Math::mul(modelMatrix, extN);
			ot.t[2].light = std::max(0.2f, Math::dot(extN, lightDir));
			
			//Project
			ot.t[0].position = Math::mul(vmMatrix, ot.t[0].position);
			ot.t[1].position = Math::mul(vmMatrix, ot.t[1].position);
			ot.t[2].position = Math::mul(vmMatrix, ot.t[2].position);

			ot.t[0].position = Math::mul(projMatrix, ot.t[0].position);
			ot.t[1].position = Math::mul(projMatrix, ot.t[1].position);
			ot.t[2].position = Math::mul(projMatrix, ot.t[2].position);
		}

		//CullFace Clipping
		int clipCount = 0;
		for (int i = 0; i < projCount; i++) {
			ProjOutTriangle& it = projOut[i];
			if (	it.t[0].position.w < 0.1 ||
				it.t[1].position.w < 0.1 ||
				it.t[2].position.w < 0.1) continue;

			ProjOutTriangle& ot = clipOut[clipCount];
			ot = it;

			ot.t[0].position.x /= ot.t[0].position.w;
			ot.t[0].position.y /= ot.t[0].position.w;
			
			ot.t[1].position.x /= ot.t[1].position.w;
			ot.t[1].position.y /= ot.t[1].position.w;
			
			ot.t[2].position.x /= ot.t[2].position.w;
			ot.t[2].position.y /= ot.t[2].position.w;

			//Face culling
			Math::Vector2 v1;
			v1.x = ot.t[1].position.x - ot.t[0].position.x, 
			v1.y = ot.t[1].position.y - ot.t[0].position.y;

			Math::Vector2 v2;
			v2.x = ot.t[2].position.x - ot.t[0].position.x, 
			v2.y = ot.t[2].position.y - ot.t[0].position.y;
			bool back = Math::cross(v1, v2) > 0;
			ProjOutVertex swap;
			if (fcull == CullType::none && back) {
				swap = ot.t[2];
				ot.t[2] = ot.t[0];
				ot.t[0] = swap;
			} else if (fcull == CullType::front) {
				if (back) {
					swap = ot.t[2];
					ot.t[2] = ot.t[0];
					ot.t[0] = swap;
				} else continue;
			} else if (fcull == CullType::back && back) continue;
			clipCount++;
		}

#define ClipToPixel(p, tam) ((int) (((p + 1) * tam / 2)))
#define PixelToClip(p, tam) (((float) p / tam) * 2 - 1)
#define Edge(px, py, a, b) ((px - a.x) * (b.y - a.y) - (py - a.y) * (b.x - a.x))
		for (int i = 0; i < clipCount; i++) {
			ProjOutTriangle& t = clipOut[i];
			float minx = 1;
			float miny = 1;
			float maxx = -1;
			float maxy = -1;
	
			//Calculate the minimum rectangle (in clip coordinates) thar contains the triangle
			for(int i = 0; i < 3; i ++) {
				minx = std::min(t.t[i].position.x, minx);
				miny = std::min(t.t[i].position.y, miny);
				maxx = std::max(t.t[i].position.x, maxx);
				maxy = std::max(t.t[i].position.y, maxy);
			}

			//The minimum rectange on screen coordinates
			int sminx = std::max(0, ClipToPixel(minx, w));
			int smaxx = std::min(w, ClipToPixel(maxx, w) + 1);

			int sminy = std::max(0, ClipToPixel(miny, h)); //Remember that y is inverted in screen space
			int smaxy = std::min(h, ClipToPixel(maxy, h) + 1); //Remember that y is inverted in screen space

			for (int i = sminy; i < smaxy; i++) {
				for(int e = sminx; e < smaxx; e++) {
					float clipX = PixelToClip(e, w);
					float clipY = PixelToClip(i, h);

					float e1 = Edge(clipX, clipY, t.t[1].position, t.t[2].position) / 2;
					float e2 = Edge(clipX, clipY, t.t[2].position, t.t[0].position) / 2;
					float e3 = Edge(clipX, clipY, t.t[0].position, t.t[1].position) / 2;


					if(e1 > 0 && e2 > 0 && e3 > 0)
					{
						float area = e1 + e2 + e3;
						e1 /= area;
						e2 /= area;
						e3 /= area;

						float depth = 	e1 * t.t[0].position.z +
								e2 * t.t[1].position.z +
								e3 * t.t[2].position.z;

						float light = 	e1 * t.t[0].light +
								e2 * t.t[1].light +
								e3 * t.t[2].light;


						if (depth < depthBuffer[i * w + e])
						{
							depthBuffer[i * w + e] = depth;
							Graphics::Pixel pixel;
							pixel.r = light * 255;
							pixel.g = light * 255;
							pixel.b = light * 255;
							pixel.a = 1;

							out->data[i * w + e] = pixel;
						}
					}
				}
			}
		}
	}
}
