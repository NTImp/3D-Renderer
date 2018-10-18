#pragma once
#include <vector>

#include "Render3D/Renderer.hpp"

void LoadModel(const char* file, std::vector<Triangle>& out, bool auto_normals = false, bool auto_txt = false);
