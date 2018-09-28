#pragma once
#include <vector>

#include "Render3D/Render3D.hpp"

void LoadModel(const char* file, std::vector<Render3D::Triangle>& out, bool auto_normals = false, bool auto_txt = false);
