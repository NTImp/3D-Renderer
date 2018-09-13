#include "ModelLoader.hpp"

#include "OBJ_Loader.h"

void LoadModel(const char* file, std::vector<Render3D::Triangle>& model)
{
	model.clear();

	objl::Loader loader;
	bool loadout = loader.LoadFile(file);

	if (loadout) {
		for (auto& m : loader.LoadedMeshes) {
			std::vector<Render3D::Vertex> vertices;
			for (auto& iv : m.Vertices) {
				Render3D::Vertex ov;
				ov.position = Math::Vector3(iv.Position.X, iv.Position.Y, iv.Position.Z);
				vertices.push_back(ov);
			}

			for (int i = 0; i < m.Indices.size(); i += 3) {
				Render3D::Triangle t;
				Math::Vector3 d1(	vertices[i + 1].position.x - vertices[i].position.x,
							vertices[i + 1].position.y - vertices[i].position.y,
							vertices[i + 1].position.z - vertices[i].position.z);
				float md1 = Math::mod(d1);
				d1.x /= md1;
				d1.y /= md1;
				d1.z /= md1;

				Math::Vector3 d2(	vertices[i + 2].position.x - vertices[i].position.x,
							vertices[i + 2].position.y - vertices[i].position.y,
							vertices[i + 2].position.z - vertices[i].position.z);

				float md2 = Math::mod(d2);
				d2.x /= md2;
				d2.y /= md2;
				d2.z /= md2;

				Math::Vector3 normal = Math::cross(d2, d1);

				t.t[0] = vertices[i + 0];
				t.t[1] = vertices[i + 1];
				t.t[2] = vertices[i + 2];

				t.t[0].normal = normal;
				t.t[1].normal = normal;
				t.t[2].normal = normal;

				model.push_back(t);
			}
		}
	}
}
