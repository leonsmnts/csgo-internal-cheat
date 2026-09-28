#include "pch.h"
#include "includes.hpp"

#define dwLocalPlayer 0xdea98c
#define dwEntityList 0x4dfff7c
#define dwViewMatrix 0x4df0dc4
#define dwClientState 0x59f19c
#define dwClientState_ViewAngles 0x4d90
#define m_bSpottedByMask 0x980


Globals globals;
Hook drawHook;

bool MeAlive() {
    Ent* lp = globals.localPlayerPtr;
    return lp && lp->health > 0;
}

void mainLoop() {
    std::cout << "Hack::mainLoop()\n";

    setUpGlobals();
    HANDLE closestPlayerLoopThreadHandle = CreateThread(0, 0, closestPlayerLoop, 0, 0, 0);

    while (true) {
        // Exit Cheat Loop
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x01) {
            globals.bExit = true;
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 0x01) {
            globals.bESP = !globals.bESP;
            drawHook.Toggle();
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 0x01) {
            globals.bCheckTeams = !globals.bCheckTeams;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 0x01) {
            globals.bAimbot = !globals.bAimbot;
        }

        if (globals.bAimbot && (GetAsyncKeyState(VK_LSHIFT) & 0x8000) && MeAlive()) {
            Aimbot();
        }

        
        globals.localPlayerPtr = *(Ent**)(globals.clientBase + dwLocalPlayer);
        globals.netvPitch = (float*)(*(intptr_t*)(globals.engineBase + dwClientState) + dwClientState_ViewAngles);
        globals.netvYaw = (float*)(*(intptr_t*)(globals.engineBase + dwClientState) + dwClientState_ViewAngles + 4);

        Sleep(1);
    }

    // Cheat Loop exited.

    if (closestPlayerLoopThreadHandle) {
        WaitForSingleObject(closestPlayerLoopThreadHandle, INFINITE);
        CloseHandle(closestPlayerLoopThreadHandle);
    }

    Draw::Shutdown();
    drawHook.Shutdown();
    Sleep(1000);
}

DWORD WINAPI closestPlayerLoop(LPVOID lpParameter) {
    while (true) {
        if (globals.bExit) {
            return 0;
        }

        globals.closestPlayerPtr = GetClosestEntPtr();
        Sleep(10);
    }
}

Ent* GetClosestEntPtr() {
    Ent* localPlayerPtr_copy = globals.localPlayerPtr;

    if (!localPlayerPtr_copy) {
        return 0;
    }

    Ent localPlayer = *localPlayerPtr_copy;
    float shortestDist = 999999;
    int shortestIndex = -1;
    int currIndex = 0;
    for (EntryInEntList entry : globals.entListClass->entArray) {
        Ent* entPtr = entry.entity;

        //ent is localPlayer
        if (entPtr == localPlayerPtr_copy) {
            currIndex++;
            continue;
        }

        // entPtr == nullptr
        if (!entPtr) {
            currIndex++;
            continue;
        }

        Ent ent = *entPtr;

        if (IsInvalidTarget(localPlayer, ent, entPtr)) {
            currIndex++;
            continue;
        }

        Vector3 enemyPos = ent.pos;
        Vector3 localPos = localPlayer.pos;

        float dist;
        if (!IsOnScreen(enemyPos, &dist)) {
            currIndex++;
            continue;
        }

        if (dist < shortestDist) {
            shortestDist = dist;
            shortestIndex = currIndex;
        }

        currIndex++;
    }

    return shortestIndex == -1 ? 0 : globals.entListClass->entArray[shortestIndex].entity;
}

bool IsInvalidTarget(Ent localPlayer, Ent ent, Ent* entPtr) {
    // TODO: add visibility and alive-bool check maybe
    return ent.health < 1 || ent.health > 100
        || globals.bCheckTeams && ent.team == localPlayer.team
        || ent.bDormant;
        /* || *(bool*)((intptr_t)entPtr + m_bSpottedByMask);*/
}

void setUpGlobals() {
    std::cout << "Hack::setUpGlobals()\n";
    globals.shaderApiBase = (intptr_t) GetModuleHandle(L"shaderapidx9.dll");
    globals.clientBase = (intptr_t) GetModuleHandle(L"client.dll");
    globals.engineBase = (intptr_t) GetModuleHandle(L"engine.dll");
    globals.localPlayerPtr = *(Ent**)(globals.clientBase + dwLocalPlayer);
    globals.entListClass = (EntListClass*)(globals.clientBase + dwEntityList);
    globals.viewMatrixPtr = (float*)(globals.clientBase + dwViewMatrix);
    globals.netvPitch = (float*)(*(intptr_t*)(globals.engineBase + dwClientState) + dwClientState_ViewAngles);
    globals.netvYaw = (float*)(*(intptr_t*)(globals.engineBase + dwClientState) + dwClientState_ViewAngles + 4);
}
