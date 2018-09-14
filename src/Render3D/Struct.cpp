#include "Structs.hpp"


namespace Render3D {
	ProjOutTriangle::ProjOutTriangle(Triangle& tr)
	{
		t[0].position = Math::Vector4(tr.t[0].position, 1);
		t[1].position = Math::Vector4(tr.t[1].position, 1);
		t[2].position = Math::Vector4(tr.t[2].position, 1);
	}
}