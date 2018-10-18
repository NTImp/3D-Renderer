#include "Renderer.hpp"

#include <algorithm>
#include <stdio.h>


Renderer::Renderer(int w, int h, Pixel* screen) {
	this->w = w;
	this->h = h;
	this->screen = screen;

	depthBuffer = new float[w * h];
	projOut = new ProjOutTriangle[maxTriangles];
	clipOut = new ProjOutTriangle[maxTriangles * 2];
}

Renderer::~Renderer() {
	delete depthBuffer;
}

void Renderer::setLightDirection(Math::Vector3& light) {
	lightDir = Math::Vector4(light, 0);
	float mod = Math::mod(light);
	lightDir.x /= mod;
	lightDir.y /= mod;
	lightDir.z /= mod;
}

void Renderer::setView(Math::Vector3& position, Math::Vector3& rotation)
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

void Renderer::setView(Math::Vector3& position, Math::Vector3& f, Math::Vector3& up)
{
	viewMatrix = Math::Matrix4(1);
	//viewMatrix = Math::LookAt(f, up);
	Math::Matrix4 helper = Math::Translate(position, -1);

	viewMatrix = Math::mul(viewMatrix, helper);

	pvMatrix = Math::mul(projMatrix, viewMatrix);
}

void Renderer::setProjection(float w, float h, float fov)
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

void Renderer::setModelMatrix(Transform& tr)
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



void Renderer::draw(std::vector<Triangle>& model, Transform& tr, Image* img, CullType c)
{
	texture = img;
	setModelMatrix(tr);
	renderTriangles(model);
}

void Renderer::clearDepthBuffer()
{
	for(int i = 0; i < w * h; i++) depthBuffer[i] = INFINITY;
}
