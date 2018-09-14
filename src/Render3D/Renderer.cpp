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
			
			ProjOutTriangle& ot = clipOut[clipCount];
			ot = it;

			ot.t[0].position.x /= ot.t[0].position.w;
			ot.t[0].position.y /= ot.t[0].position.w;
			
			ot.t[1].position.x /= ot.t[1].position.w;
			ot.t[1].position.y /= ot.t[1].position.w;
			
			ot.t[2].position.x /= ot.t[2].position.w;
			ot.t[2].position.y /= ot.t[2].position.w;

			bool visible = true;
			for (int e = 0; e < 3 && visible; e++)
			{
				if (	ot.t[e].position.z < 0.1 ||
					ot.t[e].position.x < -1   ||
					ot.t[e].position.x > 1   ||
					ot.t[e].position.y < -1   ||
					ot.t[e].position.y > 1) visible = false;
			}

			if (!visible) continue;

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

			ot.t[0].position.x += 1;
			ot.t[0].position.y += 1;

			ot.t[1].position.x += 1;
			ot.t[1].position.y += 1;

			ot.t[2].position.x += 1;
			ot.t[2].position.y += 1;

			ot.t[0].position.x *= 0.5 * w;
			ot.t[0].position.y *= 0.5 * h;

			ot.t[1].position.x *= 0.5 * w;
			ot.t[1].position.y *= 0.5 * h;

			ot.t[2].position.x *= 0.5 * w;
			ot.t[2].position.y *= 0.5 * h;


			clipCount++;
		}

#define Edge(px, py, a, b) (((float)px - a.x) * (b.y - a.y) - ((float)py - a.y) * (b.x - a.x))
		for (int i = 0; i < clipCount; i++) {
			ProjOutTriangle& t = clipOut[i];
			int minx = w;
			int miny = h;
			int maxx = 0;
			int maxy = 0;
	
			//Calculate the minimum rectangle (in clip coordinates) thar contains the triangle
			for(int i = 0; i < 3; i ++) {
				minx = std::min((int) t.t[i].position.x, minx);
				miny = std::min((int) t.t[i].position.y, miny);
				maxx = std::max((int) t.t[i].position.x, maxx);
				maxy = std::max((int) t.t[i].position.y, maxy);
			}

			//The minimum rectange on screen coordinates
			int sminx = std::max(0, minx);
			int smaxx = std::min(w, maxx + 1);

			int sminy = std::max(0, miny);
			int smaxy = std::min(h, maxy + 1);

			float y = sminy;
			for (int i = sminy; i < smaxy; i++) {
				float x = sminx;
				for(int e = sminx; e < smaxx; e++) {
					float e1 = Edge(x, y, t.t[1].position, t.t[2].position) / 2;
					float e2 = Edge(x, y, t.t[2].position, t.t[0].position) / 2;
					float e3 = Edge(x, y, t.t[0].position, t.t[1].position) / 2;


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
					x++;
				}
				y++;
			}
		}
	}
}
