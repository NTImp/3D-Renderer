#include "Renderer.hpp"
#include "Math/Math.hpp"

#include <algorithm>
#include <stdio.h>


//W and H coordinates of the screen for a faster use
extern int w, h;

//Get a clip coordinate and returns its screen coordinate
//p is either the x or y clip coordinate and tam width or height screen coordinates
#define ClipToPixel(p, tam) ((int) (((p + 1) * tam / 2)))
//Get a screen coordinate and returns its clip coordinate
//p is either the x or y screen coordinate and tam width or height screen coordinates
#define PixelToClip(p, tam) (((float) p / tam) * 2 - 1)

#define SIGN(a, b) ((x - a.x) * (a.y - b.y) - (y - a.y) * (a.x - b.x))


namespace Renderer {
	void DrawPixel(int x, int y, float r, float g, float b, float l);

	inline bool TrianglePointCheck(float x, float y, Triangle& t)
	{
		if (	SIGN(t.t[0].position, t.t[1].position) < 0
			&& SIGN(t.t[1].position, t.t[2].position) < 0
			&& SIGN(t.t[2].position, t.t[0].position) < 0) return 1;
		return 0;
	}

	void FromClipToScreen(Triangle& t)
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
		int smaxx = std::min(w, ClipToPixel(maxx, w));

		int sminy = std::max(0, ClipToPixel(-maxy, h)); //Remember that y is inverted in screen space
		int smaxy = std::min(h, ClipToPixel(-miny, h)); //Remember that y is inverted in screen space

		for (int i = sminy; i < smaxy; i++) {
			for(int e = sminx; e < smaxx; e++) {
				if(TrianglePointCheck(PixelToClip(e, w), -PixelToClip(i, h), t))
				{
					DrawPixel(e, i, 1, 1, 1, 0.8);
				}
			}
		}
	}
}
