#pragma once

void Aimbot();
void SetPitchAndYaw(Vector3 localPos, Vector3 entPos);
void GetEnemyHeadPos(Ent* entPtr, Vector3* ret);
bool IsOnScreen(Vector3 pos, float* len);
