#include "pch.h"
#include "includes.hpp"

#define PI 3.14159265
#define m_dwBoneMatrix 0x26a8

//Global vars from Hack.cpp
extern Globals globals;

void Aimbot() {
    Ent* localPlayerPtr_copy = globals.localPlayerPtr;

    // localPlayerPtr == nullptr
    if (!localPlayerPtr_copy) {
        return;
    }
    Ent localPlayer = *localPlayerPtr_copy;

    Ent* currClosestPlayerPtr = globals.closestPlayerPtr;
    // currClosestPlayerPtr == nullptr
    if (!currClosestPlayerPtr) {
        return;
    }
    Ent closestPlayer = *currClosestPlayerPtr;


    // Get positions for pitch/yaw
    Vector3 localPos = localPlayer.pos + localPlayer.viewOffset;

    // Get enemy headpos for pitch yaw
    Vector3 enemyHeadPos = Vector3();
    GetEnemyHeadPos(currClosestPlayerPtr, &enemyHeadPos);

    SetPitchAndYaw(localPos, enemyHeadPos);
}

void SetPitchAndYaw(Vector3 localPos, Vector3 entPos) {
    Vector3 distanceVector;
    Vector3 distanceVecXY;
    float newYaw;
    float newPitch;

    distanceVector = entPos - localPos;
    distanceVecXY = Vector3(distanceVector.x, distanceVector.y, 0);

    newYaw = atan2(distanceVector.y, distanceVector.x) * 180.f / PI;

    newPitch = atan(distanceVecXY.len() / distanceVector.z /*- PI/4*/) * 178.f / PI; // -89 - 89 (sky - ground)
    newPitch += 89.f;
    newPitch = newPitch > 89.f ? newPitch - 178.f : newPitch;

    *globals.netvYaw = newYaw;
    *globals.netvPitch = newPitch;
}

void GetEnemyHeadPos(Ent* entPtr, Vector3* ret) {
    int index;
    BYTE* boneMatrixArray;

    // Head Index is 8
    index = 8;
    
    // Get ptr to boneMatrix
    boneMatrixArray = *(BYTE**)((int)entPtr + m_dwBoneMatrix);

    ret->x = *(float*)(boneMatrixArray + 0x0C + index * 0x30);
    ret->y = *(float*)(boneMatrixArray + 0x1C + index * 0x30);
    ret->z = *(float*)(boneMatrixArray + 0x2C + index * 0x30);
}

// ---- adapted w2s from Draw.cpp ----
// true -> on screen and distance to center in len
// false -> not on screen
bool IsOnScreen(Vector3 pos, float* len) {
    // no vec4 here, so use z as w
    Vector3 clipCoords;
    float* viewMatrix = globals.viewMatrixPtr;

    clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
    clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
    clipCoords.z = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

    if (clipCoords.z < 0.1f) {
        return false;
    }

    Vector2 NDC{ 0, 0 };
    NDC.x = clipCoords.x / clipCoords.z;
    NDC.y = clipCoords.y / clipCoords.z;

    if (NDC.x < -1.0 || NDC.x > 1.0 || NDC.y < -1.0 || NDC.y > 1.0) {
        return false;
    }
    
    *len = NDC.len();
    return true;
}
