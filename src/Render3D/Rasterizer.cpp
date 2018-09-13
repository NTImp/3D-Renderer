#include "Render3D.hpp"

#include <algorithm>
#include <stdio.h>

#include "Structs.hpp"
#include "Math/Math.hpp"

//W and H coordinates of the screen for a faster use
extern int w, h;
extern float* depthBuffer;

//Get a clip coordinate and returns its screen coordinate
//p is either the x or y clip coordinate and tam width or height screen coordinates
#define ClipToPixel(p, tam) ((int) (((p + 1) * tam / 2)))
//Get a screen coordinate and returns its clip coordinate
//p is either the x or y screen coordinate and tam width or height screen coordinates
#define PixelToClip(p, tam) (((float) p / tam) * 2 - 1)

namespace Render3D {
	extern void DrawPixel(int x, int y, float r, float g, float b, float l);

	/*
	inline bool TrianglePointCheck(float x, float y, ProjOutTriangle& t)
	{
		if (	SIGN(t.t[0].position, t.t[1].position) < 0
			&& SIGN(t.t[1].position, t.t[2].position) < 0
			&& SIGN(t.t[2].position, t.t[0].position) < 0) return 1;
		return 0;
	}
	*/

	inline float Edge(float x, float y, Math::Vector4& a, Math::Vector4& b)
	{
		return (x - a.x) * (b.y - a.y) - (y - a.y) * (b.x - a.x);
	}

	void FromClipToScreen(ProjOutTriangle& t)
	{
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
						DrawPixel(e, i, 1, 1, 1, light);
					}
				}
			}
		}
	}
}
