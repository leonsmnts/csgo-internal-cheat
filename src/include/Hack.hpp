#pragma once

#include "csgo_header.hpp"

bool IsInvalidTarget(Ent localPlayer, Ent ent, Ent* entPtr);
Ent* GetClosestEntPtr();
DWORD WINAPI closestPlayerLoop(LPVOID lpParameter);

void setUpGlobals();
void mainLoop();
