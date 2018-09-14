#include "Render3D.hpp"

#include <algorithm>

#include "Structs.hpp"

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
}
