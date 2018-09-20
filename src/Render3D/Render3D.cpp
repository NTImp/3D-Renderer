#include "Render3D.hpp"

#include <algorithm>
#include <stdio.h>

#include "Math/Math.hpp"
#include "Structs.hpp"

Graphics::Screen* out = nullptr;
float* depthBuffer = nullptr;

Math::Vector4 lightDir = {};

Math::Matrix4 modelMatrix = {};
Math::Matrix4 viewMatrix = {};
Math::Matrix4 projMatrix = {};
Math::Matrix4 pvMatrix = {};
Math::Matrix4 vmMatrix = {};
Math::Matrix4 pvmMatrix = {};
//W and H coordinates of the screen for a faster use
int w = 0, h = 0;

namespace Render3D {
	CullType fcull = CullType::none;
	
	extern void RenderTriangles(std::vector<Triangle>& triangles);

	void Init(Graphics::Screen* scr)
	{
		if (depthBuffer) Finish(); //You can recreate the Renderer
		out = scr;
		w = out->w;
		h = out->h;
		depthBuffer = new float[w * h];
		clearDepthBuffer();
	};

	void Finish()
	{
		delete depthBuffer;
		depthBuffer = nullptr;
	};

	void SetLightDirection(Math::Vector3& light) {
		lightDir = Math::Vector4(light, 0);
		float mod = Math::mod(light);
		lightDir.x /= mod;
		lightDir.y /= mod;
		lightDir.z /= mod;
	}

	void SetView(Math::Vector3& position, Math::Vector3& rotation)
	{
		viewMatrix = Math::Matrix4(1);

		viewMatrix = Math::RotateX(rotation.x);
		
		Math::Matrix4 helper = Math::RotateY(rotation.y);
		viewMatrix = Math::mul(viewMatrix, helper);
		
		helper = Math::RotateZ(rotation.z);
		viewMatrix = Math::mul(viewMatrix, helper);

		//Translation
		helper = Math::Translate(position, -1);

		viewMatrix = Math::mul(viewMatrix, helper);
		pvMatrix = Math::mul(projMatrix, viewMatrix);
	}

	void SetView(Math::Vector3& position, Math::Vector3& f, Math::Vector3& up)
	{
		viewMatrix = Math::Matrix4(1);
		//viewMatrix = Math::LookAt(f, up);
		Math::Matrix4 helper = Math::Translate(position, -1);

		viewMatrix = Math::mul(viewMatrix, helper);

		pvMatrix = Math::mul(projMatrix, viewMatrix);
	}

	void SetProjection(float w, float h, float fov)
	{
		float zfar = 2000;
		float znear = 0.2;
		float zscale = zfar / (zfar - znear);
		float ar = w / h;
		float cfov = 1.f / Math::Tan(fov / 2);

		projMatrix = Math::Matrix4(0);
		projMatrix.m[0][0] = (h / w) * cfov;
		projMatrix.m[1][1] = -cfov; //Invert y
		projMatrix.m[2][2] = 1;//zscale;
		projMatrix.m[2][3] = 1;-zscale * zfar;
		projMatrix.m[3][2] = 1;
		
	}

	inline void setModelMatrix(Transform& tr)
	{
		modelMatrix = Math::Scale(tr.scale, 1);
		//Translation
		Math::Matrix4 helper = Math::Translate(tr.position, 1);
		modelMatrix = Math::mul(modelMatrix, helper);

		modelMatrix = Math::mul(modelMatrix, helper);
		
		helper = Math::RotateY(tr.rotation.y);
		modelMatrix = Math::mul(modelMatrix, helper);
		
		helper = Math::RotateZ(tr.rotation.z);
		modelMatrix = Math::mul(modelMatrix, helper);
		
		vmMatrix = Math::mul(viewMatrix, modelMatrix);
		pvmMatrix = Math::mul(projMatrix, vmMatrix);
	}

	

	void Draw(std::vector<Triangle>& model, Transform& tr)
	{
		setModelMatrix(tr);
		RenderTriangles(model);
	}

	void clearDepthBuffer()
	{
		for(int i = 0; i < w * h; i++) depthBuffer[i] = INFINITY;
	}
};
