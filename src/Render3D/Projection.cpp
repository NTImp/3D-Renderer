#include "Render3D.hpp"

#include <algorithm>

#include "Structs.hpp"

extern Math::Vector4 lightDir;

extern Math::Matrix4 modelMatrix;
extern Math::Matrix4 viewMatrix;
extern Math::Matrix4 projMatrix;
extern Math::Matrix4 pvMatrix;
extern Math::Matrix4 vmMatrix;
extern Math::Matrix4 pvmMatrix;

extern bool fculling;

//Clip plane
constexpr float cz = 0.1f;

namespace Render3D {
	inline Math::Vector3 Intersection(ProjOutVertex& a, ProjOutVertex& b)
	{
		
	}

	/*
	int Clip(ProjOutTriangle& i, ProjOutTriangle& o1, ProjOutTriangle& o2)
	{
		Math::Vector4 p1, p2;

		Math::Vector4* vout[3];
		int oc = 0;
		Math::Vector4* vin[3];
		int ic = 0;

		for (int i1 = 0; i1 < 3; i1++) {
			if (i.t[i1].position.w < cz) {
				vout[oc++] = &i.t[i1].position;
			} else {
				vin[ic++] = &i.t[i1].position;
			}
		}

		if (oc == 0) { //Triangle is completly visible
			o1 = i;
			return 1;
		}
		if (oc == 3) return 0; //Triangle is completly invisible

		if (oc == 1) {
			p1 = Intersection(*vin[0], *vout[0]);
			p2 = Intersection(*vin[1], *vout[0]);

			o1 = i;
			o1.t[0].position = *vin[0];
			o1.t[1].position = *vin[1];
			o1.t[2].position = p1;


			o2 = i;
			o1.t[0].position = *vin[1];
			o1.t[1].position = p1;
			o1.t[2].position = p2;

			return 1;
		}

		if (oc == 2) {
			return 0;
			p1 = Intersection(*vin[0], *vout[0]);
			p2 = Intersection(*vin[0], *vout[1]);

			o1 = i;
			o1.t[0].position = *vin[0];
			o1.t[1].position = p2;
			o1.t[2].position = p1;

			return 1;
		}

		return 0;
	}
	*/

	int ProjectTriangle(Triangle& i, ProjOutTriangle o[2])
	{
		ProjOutTriangle ot(i);

		Math::Vector4 extN(i.t[0].normal, 0);
		extN = Math::mul(modelMatrix, extN);
		ot.t[0].light = std::max(0.2f, Math::dot(extN, lightDir));

		extN = Math::Vector4(i.t[1].normal, 0);
		extN = Math::mul(modelMatrix, extN);
		ot.t[1].light = std::max(0.2f, Math::dot(extN, lightDir));

		extN = Math::Vector4(i.t[2].normal, 0);
		extN = Math::mul(modelMatrix, extN);
		ot.t[2].light = std::max(0.2f, Math::dot(extN, lightDir));

		ot.t[0].position = Math::mul(vmMatrix, ot.t[0].position);
		ot.t[1].position = Math::mul(vmMatrix, ot.t[1].position);
		ot.t[2].position = Math::mul(vmMatrix, ot.t[2].position);

		ot.t[0].position = Math::mul(projMatrix, ot.t[0].position);
		ot.t[1].position = Math::mul(projMatrix, ot.t[1].position);
		ot.t[2].position = Math::mul(projMatrix, ot.t[2].position);

		if (	ot.t[0].position.w < 0.1 ||
			ot.t[1].position.w < 0.1 ||
			ot.t[2].position.w < 0.1) return 0;

		Math::Vector2 v1;
		v1.x = ot.t[1].position.x - ot.t[0].position.x, 
		v1.y = ot.t[1].position.y - ot.t[0].position.y;

		Math::Vector2 v2;
		v2.x = ot.t[2].position.x - ot.t[0].position.x, 
		v2.y = ot.t[2].position.y - ot.t[0].position.y;

		ot.back = Math::cross(v1, v2) > 0;

		ot.t[0].position.x /= ot.t[0].position.w;
		ot.t[0].position.y /= ot.t[0].position.w;
		
		ot.t[1].position.x /= ot.t[1].position.w;
		ot.t[1].position.y /= ot.t[1].position.w;
		
		ot.t[2].position.x /= ot.t[2].position.w;
		ot.t[2].position.y /= ot.t[2].position.w;

		//ot.t[2] = ProjectVertex(ot.t[0]);
		//ot.t[1] = ProjectVertex(ot.t[1]);
		//ot.t[0] = ProjectVertex(ot.t[2]);

		o[0] = ot;

		return 1;
	}
}
