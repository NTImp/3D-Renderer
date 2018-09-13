#include "Render3D.hpp"

extern Graphics::Screen* out;
extern float* depthBuffer;

extern int w, h;


namespace Render3D {
	void DrawPixel(int x, int y, float r, float g, float b, float l)
	{
		Graphics::Pixel np;
		np.r = r * l * 255;
		np.g = r * l * 255;
		np.b = r * l * 255;
		np.a = 0;
		out->data[y * w + x] = np;
	}
};
