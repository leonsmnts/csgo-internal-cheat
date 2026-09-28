#pragma once
#include "csgo_header.hpp"

class Globals {
public:
	Ent* localPlayerPtr = 0;
	Ent* closestPlayerPtr = 0;
	EntListClass* entListClass = 0;
	float* viewMatrixPtr = 0;
	intptr_t shaderApiBase = 0;
	intptr_t clientBase = 0;
	intptr_t engineBase = 0;
	bool bExit = false;
	bool bESP = false;
	bool bAimbot = false;
	bool bCheckTeams = true;
	float* netvPitch;
	float* netvYaw;
};
