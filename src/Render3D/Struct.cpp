#include "Structs.hpp"


ProjOutTriangle::ProjOutTriangle(Triangle& tr)
{
	t[0].position = Math::Vector4(tr.t[0].position, 1);
	t[1].position = Math::Vector4(tr.t[1].position, 1);
	t[2].position = Math::Vector4(tr.t[2].position, 1);
	t[0].texture = tr.t[0].texture;
	t[1].texture = tr.t[1].texture;
	t[2].texture = tr.t[2].texture;
}
