#pragma once
#include <stdint.h>
#include "Vector3.hpp"


// Created with ReClass.NET 1.2 by KN4CK3R

class EntryInEntList
{
public:
	class Ent* entity; //0x0000
	char pad_0004[4]; //0x0004
	void* prev; //0x0008
	void* next; //0x000C
}; //Size: 0x0010

class EntListClass
{
public:
	class EntryInEntList entArray[32]; //0x0000
}; //Size: 0x0200

class Ent
{
public:
	char pad_0000[160]; //0x0000
	Vector3 pos; //0x00A0
	Vector3 pos2; //0x00AC
	char pad_00B8[53]; //0x00B8
	bool bDormant; // 0x00ED
	char pad_00EE[6]; // 0x00EE
	int32_t team; //0x00F4
	int32_t team2; //0x00F8
	char pad_00FC[4]; //0x00FC
	int32_t health; //0x0100
	char pad_0104[4]; //0x0104
	Vector3 viewOffset; // 0x108
	char pad_0114[24]; // 0x114
	float pitch; //0x012C
	float yaw; //0x0130
}; //Size: 0x0134
