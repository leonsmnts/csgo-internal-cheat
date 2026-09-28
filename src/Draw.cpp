#include "pch.h"
#include "includes.hpp"
#include <string>
#include "Bone_ESP_Helper.hpp"

#define m_dwBoneMatrix 0x26a8

// means that the FVF (VERTICES are in) is x/y/z/w/color
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

/*
	FOR DRAWING YOU HAVE TO USE:
		-  device->SetFVF(CUSTOMFVF);
		-  device->DrawPrimitiveUP(args..);
*/

extern Globals globals;
extern Hook drawHook;

LPDIRECT3DDEVICE9 device;
LPD3DXFONT font;

int screenWidth;
int screenHeight;

const int MAX_AMOUNT_BONE_APART = 150.f;

int ct_indices[] = {0, 3, 4, 5, 6, 7, 11, 12, 18, 34, 35, 36, 37, 40, 41, 47, 63, 64, 66, 72, 73, 74, 77, 78, 81, 82, 83};
int t_indices[] = {0, 3, 4, 5, 6, 7, 11, 12, 18, 35, 37, 39, 40, 41, 63, 65, 66, 67, 68, 71, 72, 73, 74, 75, 78, 79};
int ct_vertex[5][8] = { {8, 7, 6, 5, 4, 3, 0},{6, 11, 36, 37, 12, 34, 35, 18},{6, 40, 66, 41, 63, 64, 47},{0, 72, 77, 78, 73, 74},{0, 81, 82, 83} };
int t_vertex[5][8] = { {8, 7, 6, 5, 4, 3, 0}, {6, 11, 37, 12, 35, 18}, {6, 39, 65, 40, 63, 41}, {0, 66, 71, 72, 67, 68}, {0, 73, 78, 79, 74, 75} };

bool Draw::IsInvalidESPTarget(Ent ent) {
	return ent.health < 1 || ent.health > 100 || ent.bDormant || (ent.team == (*(Ent*)globals.localPlayerPtr).team && globals.bCheckTeams);
}

void DrawModelString(Ent* entPtr, Vector2 entScreenPos);

void Draw::DrawMain() {
	
	// Important
	device->SetFVF(CUSTOMFVF);
	
	if (!globals.localPlayerPtr) {
		return;
	}

	Ent localPlayer = *globals.localPlayerPtr;
	
	for (EntryInEntList entry : globals.entListClass->entArray) {
		Ent* entPtr = entry.entity;

		// entPtr == nullptr
		if (!entPtr) {
			continue;
		}

		//ent is localPlayer
		if (entPtr == globals.localPlayerPtr) {
			continue;
		}

		Ent ent = *entPtr;

		if (IsInvalidESPTarget(ent)) {
			continue;
		}

		Vector2 entScreenPos;
		if (!WorldToScreen(ent.pos, &entScreenPos, globals.viewMatrixPtr, screenWidth, screenHeight)) {
			//not on screen
			continue;
		}

		DrawBoxESP(entPtr);
		DrawLine({ screenWidth / 2.f, (float)screenHeight }, entScreenPos, D3DCOLOR_RGBA(255, 0, 0, 255));
		DrawBoneESP(entPtr, getCorrectBoneIndexMatrix(entPtr));
	}
	
}

void DrawModelString(Ent* entPtr, Vector2 entScreenPos) {
	Ent ent = *entPtr;
	char* modelName = (char*)(*(intptr_t*)((int)entPtr + 0x6c) + 0x04);
	Draw::DrawString(entScreenPos.x, entScreenPos.y - 20, modelName, -1, D3DCOLOR_RGBA(220, 220, 220, 225));
}

void Draw::DrawBoxESP(Ent* entPtr) {
	Vector3 topPos, bottomPos;
	GetBonePosition(entPtr, 8, &topPos);
	topPos.z = topPos.z + 8.f;
	GetBonePosition(entPtr, 1, &bottomPos);

	Vector2 topSc, botSc;
	if (!WorldToScreen(topPos, &topSc, globals.viewMatrixPtr, screenWidth, screenHeight)) {
		return;
	}

	if (!WorldToScreen(bottomPos, &botSc, globals.viewMatrixPtr, screenWidth, screenHeight)) {
		return;
	}

	Vector2 centerSc{0, 0};
	centerSc.x = topSc.x;
	centerSc.y = botSc.y - topSc.y;

	DrawSquareCentered(centerSc.x, botSc.y - centerSc.y / 2, centerSc.y / 2.25f, centerSc.y, D3DCOLOR_RGBA(255, 0, 0, 255));
}

// vertexLists is pointer to array of type int[5][4];
// where the first subarray int[0] only uses 3 indices to draw 2 lines!
void Draw::DrawBoneESP(Ent* entPtr, intptr_t vertexLists) {

	if (!entPtr) {
		return;
	}

	int i = 0;
	for (int* vertices = (int*)vertexLists; i < 5; vertices = (int*)(vertexLists + 4 * i * sizeof(int))) {
		int amount = i == 0 ? 3 : 4;
		
		CUSTOMVERTEX vertices_screen[4];

		for (int j = 0; j < amount; j++) {
			int to = vertices[j];

			// world position
			Vector3 pBone;
			GetBonePosition(entPtr, to, &pBone);
			// screen pos
			Vector2 sBone;
			if (!WorldToScreen(pBone, &sBone, globals.viewMatrixPtr, screenWidth, screenHeight)) {
				return;
			}

			vertices_screen[j] = { sBone.x, sBone.y, 0.f, 1.f, D3DCOLOR_RGBA(255, 0, 0, 255)};
		}
		
		device->DrawPrimitiveUP(D3DPT_LINESTRIP, amount-1, &vertices_screen, sizeof(CUSTOMVERTEX));
		i++;
	}
}

/*
Draws all 85 bones in an entity to the screen at the bone pos with the specific index.
Can come in handy to find out which bone is at which index and to find out the order
for e.g. a Bone-ESP.
*/
void Draw::DrawAllBonesWithIndex(Ent* entPtr) {
	int AMOUNT_BONES = 85;
	for (int i = 0; i < AMOUNT_BONES; i++) {
		Vector3 bonePos{ 0, 0, 0 };
		if (!GetBonePosition(entPtr, i, &bonePos)) {
			continue;
		}

		Vector2 screenCoords;
		if (!WorldToScreen(bonePos, &screenCoords, globals.viewMatrixPtr, screenWidth, screenHeight)) {
			// behind me
			continue;
		}

		std::string s = std::to_string(i);
		char const* pchar = s.c_str();
		DrawString(screenCoords.x, screenCoords.y, pchar, s.length(), D3DCOLOR_RGBA(255, 0, 0, 255));
	}
}

bool Draw::GetBonePosition(Ent* entPtr, int index, Vector3* bonePos) {
	if (index < 0 || index > 84 || !entPtr) {
		return false;
	}
	BYTE* boneMatrixArray = *(BYTE**)((int)entPtr+m_dwBoneMatrix);

	bonePos->x = *(float*)(boneMatrixArray + 0x0C + index * 0x30);
	bonePos->y = *(float*)(boneMatrixArray + 0x1C + index * 0x30);
	bonePos->z = *(float*)(boneMatrixArray + 0x2C + index * 0x30);

	return true;
}

void Draw::Init() {
	D3DXCreateFont(device, 14, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &font);
	
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;
	device->GetCreationParameters(&cparams);
	GetClientRect(cparams.hFocusWindow, &rect);

	screenWidth = rect.right;
	screenHeight = rect.bottom;
}

HRESULT APIENTRY Draw::DrawEntry(LPDIRECT3DDEVICE9 o_device) {
	if (!device) {
		device = o_device;
		Init();
	}

	DrawMain();

	return drawHook.orig_EndScene(o_device);
}

void Draw::DrawString(float x, float y, const char* text, int n, D3DCOLOR color) {
	// for efficiency change from null to some LPD3DXSPRITE ..?
	RECT rect{ x, y, x + n * 10 , y + 16 };
	font->DrawTextA(NULL, text, n, &rect, DT_NOCLIP | DT_VCENTER | DT_CENTER, color);
}

void Draw::DrawSquareTopLeft(float x, float y, float width, float height, D3DCOLOR color) {
	CUSTOMVERTEX Vertices[] =
	{
		{ x, y, 0.0, 1.0, color },
		{ x + width, y , 0.0, 1.0, color },
		{ x + width, y + height , 0.0, 1.0, color },
		{ x, y + height, 0.0, 1.0, color },
		{ x, y , 0.0, 1.0, color }
	};

	device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &Vertices, sizeof(CUSTOMVERTEX));
}

void Draw::DrawSquareCentered(float x, float y, float width, float height, D3DCOLOR color) {
	DrawSquareTopLeft(x - width / 2, y - height / 2, width, height, color);
}

void Draw::DrawLine(Vector2 from, Vector2 to, /*float thickness, */D3DCOLOR color) {
	CUSTOMVERTEX Vertices[] =
	{
		{ from.x, from.y, 0.0, 1.0, color },
		{ to.x, to.y, 0.0, 1.0, color }
	};

	device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &Vertices, sizeof(CUSTOMVERTEX));
}

void Draw::Shutdown() {
	if (font) {
		font->Release();
	}
}


// ---- w2s ----
bool Draw::WorldToScreen(Vector3 pos, Vector2* screenPos, float viewMatrix[16], int screenWidth, int screenHeight) {
	// no vec4 here, so use z as w
	Vector3 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.z < 0.1f) {
		return false;
	}

	Vector2 NDC{ 0, 0 };
	NDC.x = clipCoords.x / clipCoords.z;
	NDC.y = clipCoords.y / clipCoords.z;

	screenPos->x = (screenWidth / 2.f * NDC.x) + (screenWidth / 2.f);
	screenPos->y = -(screenHeight / 2.f * NDC.y) + (screenHeight / 2.f);

	return true;
}
