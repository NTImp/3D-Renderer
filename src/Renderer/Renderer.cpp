#include "Renderer.hpp"
#include "Math/Math.hpp"

#include <algorithm>
#include <stdio.h>

Graphics::Screen* out = nullptr;
float* depthBuffer = nullptr;

Math::Matrix4 modelMatrix = {};
Math::Matrix4 viewMatrix = {};
Math::Matrix4 projMatrix = {};
Math::Matrix4 pvMatrix = {};

//W and H coordinates of the screen for a faster use
int w = 0, h = 0;

namespace Renderer {
	
	extern void FromClipToScreen(Triangle& t);

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


	void SetView(Math::Vector3& position, Math::Vector3& rotation)
	{
		viewMatrix = Math::RotateX(rotation.x);
		
		Math::Matrix4 helper = Math::RotateY(rotation.y);
		viewMatrix = Math::mul(viewMatrix, helper);
		
		helper = Math::RotateZ(rotation.z);
		viewMatrix = Math::mul(viewMatrix, helper);

		//Translation
		helper = Math::Matrix4(1);
		helper.m[0][3] = position.x;
		helper.m[1][3] = position.y;
		helper.m[2][3] = position.z;

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
		projMatrix.m[0][0] = (w / h) * cfov;
		projMatrix.m[1][1] = cfov;
		projMatrix.m[2][2] = zscale;
		projMatrix.m[2][3] = -zscale * zfar;
		projMatrix.m[3][2] = 1;
		
	}

	inline Math::Vector3 ProjectPoint(Math::Vector3& point)
	{
		Math::Matrix4 pvm = Math::mul(projMatrix, viewMatrix);
		pvm = Math::mul(pvm, modelMatrix);
		Math::Vector4 ext(point, 1);

		Math::Vector4 pp = Math::mul(pvm, ext);

		return Math::Vector3(pp.x / pp.w, pp.y / pp.w, pp.z);
	}

	inline void setModelMatrix(Transform& tr)
	{
		//Translation
		modelMatrix = Math::Matrix4(1);
		modelMatrix.m[0][3] = tr.position.x;
		modelMatrix.m[1][3] = tr.position.y;
		modelMatrix.m[2][3] = tr.position.z;

		Math::Matrix4 helper = Math::RotateX(tr.rotation.x);
		modelMatrix = Math::mul(modelMatrix, helper);
		
		helper = Math::RotateY(tr.rotation.y);
		modelMatrix = Math::mul(modelMatrix, helper);
		
		helper = Math::RotateZ(tr.rotation.z);
		modelMatrix = Math::mul(modelMatrix, helper);
	}

	void Draw(std::vector<Triangle>& model, Transform& tr)
	{
		setModelMatrix(tr);
		for (auto& t : model) {
			Triangle pt(t);//ProjectedTriangle

			pt.t[0].position = ProjectPoint(t.t[0].position);
			pt.t[1].position = ProjectPoint(t.t[1].position);
			pt.t[2].position = ProjectPoint(t.t[2].position);
			FromClipToScreen(pt);
		}
	}

	void clearDepthBuffer()
	{
		for(int i = 0; i < w * h; i++) depthBuffer[i] = INFINITY;
	}
};
