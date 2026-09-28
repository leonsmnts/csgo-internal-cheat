#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Vector3.hpp"


// Set w = 1.0f to tell D3D9 coords are in screenSpace already
// Set z = 0.0f is the nearest to Camera (on top of everything). 1.0f is most far away.
struct CUSTOMVERTEX
{
	float x, y, z, w; // always set z to 0.0 and w to 1.0
	DWORD argb;
};

namespace Draw {
	HRESULT APIENTRY DrawEntry(LPDIRECT3DDEVICE9 o_device);
	void DrawMain();
	void Shutdown();
	void Init();

	bool GetBonePosition(Ent* entPtr, int index, Vector3* bonePos);
	void DrawAllBonesWithIndex(Ent* entPtr);
	void DrawBoxESP(Ent* ent);
	void DrawBoneESP(Ent* entPtr, intptr_t vertexLists);

	void DrawString(float x, float y, const char* text, int n, D3DCOLOR color);
	void DrawSquareTopLeft(float x, float y, float width, float height, DWORD color);
	void DrawSquareCentered(float x, float y, float width, float height, DWORD color);
	void DrawLine(Vector2 from, Vector2 to, /*float thickness, */D3DCOLOR color);

	bool BonesInvalid(Ent ent);
	bool IsInvalidESPTarget(Ent ent);
	bool WorldToScreen(Vector3 pos, Vector2* screenPos, float viewMatrix[16], int screenWidth, int screenHeight);
}
