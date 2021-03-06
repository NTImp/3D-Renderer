#include "Renderer.hpp"

#include <vector>
#include <algorithm>


inline ProjOutVertex Intersection(const float cz, ProjOutVertex& va, ProjOutVertex& vb)
{
	ProjOutVertex ret = va;
	float u = (cz - vb.position.w) / (vb.position.w - va.position.w);
	ret.position =  Math::Vector4(	u * (vb.position.x - va.position.x) + vb.position.x,
					u * (vb.position.y - va.position.y) + vb.position.y,
					u * (vb.position.z - va.position.z) + vb.position.z,
					cz);
	ret.texture = Math::Vector2(	u * (vb.texture.x - va.texture.x) + vb.texture.x,
					u * (vb.texture.y - va.texture.y) + vb.texture.y);
	return ret;
}

void Renderer::renderTriangles(std::vector<Triangle>& triangles)
{
	int projCount = 0;
	for (auto& t : triangles) {
		ProjOutTriangle ot = ProjOutTriangle(t);


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
		
		ProjOutVertex* vout[3] = {0};
		ProjOutVertex* vin[3] = {0};
		int voutc = 0, vinc = 0;

		ProjOutTriangle i0, i1;
		for (int i = 0; i < 3; i++) {
			if (ot.t[i].position.w < cz)  vout[voutc++] = &ot.t[i];
			else  vin[vinc++] = &ot.t[i];
		}
		
		if (voutc == 0) projOut[projCount++] = ot;
		else if (voutc == 1) {
			ProjOutVertex cv0 = Intersection(cz, *vin[1], *vout[0]);
			ProjOutVertex cv1 = Intersection(cz, *vout[0], *vin[0]);

			i0.t[0] = *vin[0];
			i0.t[1] = cv0;
			i0.t[2] = cv1;

			i1.t[0] = *vin[0];
			i1.t[1] = *vin[1];
			i1.t[2] = cv0;

			projOut[projCount++] = i0;
			projOut[projCount++] = i1;
		} else if (voutc == 2) {
			i0.t[0] = *vin[0];
			i0.t[1] = Intersection(cz, *vin[0], *vout[0]);
			i0.t[2] = Intersection(cz, *vout[1], *vin[0]);
			projOut[projCount++] = i0;
		}
	}

	//CullFace Clipping
	int clipCount = 0;
	for (int i = 0; i < projCount; i++) {
		ProjOutTriangle& it = projOut[i];
		
		ProjOutTriangle& ot = clipOut[clipCount];
		ot = it;

		/*
		if (	ot.t[0].position.w < 0.1 ||
			ot.t[1].position.w < 0.1 ||
			ot.t[2].position.w < 0.1) continue;
		*/

		
		ot.t[0].position.x /= ot.t[0].position.w;
		ot.t[0].position.y /= ot.t[0].position.w;
		
		ot.t[1].position.x /= ot.t[1].position.w;
		ot.t[1].position.y /= ot.t[1].position.w;
		
		ot.t[2].position.x /= ot.t[2].position.w;
		ot.t[2].position.y /= ot.t[2].position.w;

		/*
		bool visible = true;
		for (int e = 0; e < 3 && visible; e++)
		{
			if (	ot.t[e].position.z < 0.1 ||
				ot.t[e].position.x < -1  ||
				ot.t[e].position.x > 1   ||
				ot.t[e].position.y < -1  ||
				ot.t[e].position.y > 1) visible = false;
		}

		if (!visible) continue;
		*/

		if (	ot.t[0].position.x < -1 &&
			ot.t[1].position.x < -1 &&
			ot.t[2].position.x < -1) continue;

		if (	ot.t[0].position.x > 1 &&
			ot.t[1].position.x > 1 &&
			ot.t[2].position.x > 1) continue;

		if (	ot.t[0].position.y < -1 &&
			ot.t[1].position.y < -1 &&
			ot.t[2].position.y < -1) continue;

		if (	ot.t[0].position.y > 1 &&
			ot.t[1].position.y > 1 &&
			ot.t[2].position.y > 1) continue;

		//Face culling
		Math::Vector2 v1;
		v1.x = ot.t[1].position.x - ot.t[0].position.x, 
		v1.y = ot.t[1].position.y - ot.t[0].position.y;

		Math::Vector2 v2;
		v2.x = ot.t[2].position.x - ot.t[0].position.x, 
		v2.y = ot.t[2].position.y - ot.t[0].position.y;
		bool back = Math::cross(v2, v1) > 0;
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

		/*
		ot.t[0].position.y *= -1;
		ot.t[0].position.y *= -1;
		ot.t[0].position.y *= -1;
		*/
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

#define Edge(px, py, a, b) ((px - a.x) * (b.y - a.y) - (py - a.y) * (b.x - a.x))
	for (int i = 0; i < clipCount; i++) {
		ProjOutTriangle t = clipOut[i];
		int minx = w;
		int miny = h;
		int maxx = 0;
		int maxy = 0;

		/*
		t.t[2].position.x = 100;
		t.t[2].position.y = 10;

		t.t[1].position.x = w - 1;
		t.t[1].position.y = 0;

		t.t[0].position.x = 0;
		t.t[0].position.y = h - 1;

		t.t[0].position.z = 1;
		t.t[1].position.z = 1;
		t.t[2].position.z = 1;
		*/
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

		int a01 = t.t[0].position.y - t.t[1].position.y;
		int a12 = t.t[1].position.y - t.t[2].position.y;
		int a20 = t.t[2].position.y - t.t[0].position.y;

		int b01 = t.t[1].position.x - t.t[0].position.x;
		int b12 = t.t[2].position.x - t.t[1].position.x;
		int b20 = t.t[0].position.x - t.t[2].position.x;

		int e01_r = Edge(sminx, sminy, t.t[0].position, t.t[1].position);
		int e12_r = Edge(sminx, sminy, t.t[1].position, t.t[2].position);
		int e20_r = Edge(sminx, sminy, t.t[2].position, t.t[0].position);


		//Inversed z for perspective correction
		float iz[3];
		iz[0] = 1.f / t.t[0].position.w;
		iz[1] = 1.f / t.t[1].position.w;
		iz[2] = 1.f / t.t[2].position.w;
		//Texture coordinates divided by z
		float ztxt[3][2];
		ztxt[0][0] = iz[0] * t.t[0].texture.x;
		ztxt[0][1] = iz[0] * t.t[0].texture.y;

		ztxt[1][0] = iz[1] * t.t[1].texture.x;
		ztxt[1][1] = iz[1] * t.t[1].texture.y;

		ztxt[2][0] = iz[2] * t.t[2].texture.x;
		ztxt[2][1] = iz[2] * t.t[2].texture.y;
		for (int i = sminy; i < smaxy; i++) {
			int e01 = e01_r;
			int e12 = e12_r;
			int e20 = e20_r;

			for(int e = sminx; e < smaxx; e++) {
				/*
				e01 = Edge(e, i, t.t[0].position, t.t[1].position);
				e12 = Edge(e, i, t.t[1].position, t.t[2].position);
				e20 = Edge(e, i, t.t[2].position, t.t[0].position);
				*/
				if(e01 <= 0 && e20 <= 0 && e12 <= 0) {
					int area = (float) (e01 + e20 + e12) / 2;
					float d12 = (float) e12 / (float) area;
					float d20 = (float) e20 / (float) area;
					float d01 = (float) e01 / (float) area;

					float depth = 	1.f / (
							d12 * iz[0] +
							d20 * iz[1] +
							d01 * iz[2]);
					float u, v;
					if (texture != nullptr) {
						u = 	depth *	(ztxt[0][0] * d12 +
								ztxt[1][0] * d20 +
								ztxt[2][0] * d01);

						v = 	depth *	(ztxt[0][1] * d12 +
								ztxt[1][1] * d20 +
								ztxt[2][1] * d01);
					}

					float light = t.t[0].light;
					if (depth < depthBuffer[i * w + e])
					{
						depthBuffer[i * w + e] = depth;
						Pixel pixel;
						if (texture == nullptr) {
							pixel.r = 255;
							pixel.g = 255;
							pixel.b = 255;
						} else {
							int cu = u * texture->getW();
							int cv = v * texture->getH();
							pixel = texture->getData()[cv * texture->getW() + cu];
						}

						pixel.r = pixel.r * light;
						pixel.g = pixel.g * light;
						pixel.b = pixel.b * light;
						pixel.a = 0;

						screen[i * w + e] = pixel;
					}
				}
				e01 += -a01;
				e20 += -a20;
				e12 += -a12;
			}
			e01_r += -b01;
			e20_r += -b20;
			e12_r += -b12;
		}
	}
}
