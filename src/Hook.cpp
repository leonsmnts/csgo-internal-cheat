#include "pch.h"
#include "includes.hpp"

#define dwppDirect3DDevice9 0xA62C0

extern Globals globals;

void Hook::Toggle() {
	std::cout << "Hook::Toggle()\n";
	bool state = globals.bESP;
	
	if (!isSetUp) {
		SetUp();
	}

	if (state) {
		TurnOn();
	}
	else {
		TurnOff();
	}
}	 
	 
void Hook::Shutdown() {
	std::cout << "Hook::Shutdown()\n";
	if (isHooked) {
		TurnOff();
	}

	if (isSetUp) {
		VirtualFree((LPVOID)orig_EndScene, 0, MEM_RELEASE);
		isSetUp = false;
	}
}

void Hook::SetUp() {
	std::cout << "Hook::SetUp()\n";
	void** vTable = **(void****)(globals.shaderApiBase + dwppDirect3DDevice9);

	funcToHook = vTable[42];
	memcpy(&stolenBytes, funcToHook, len);

	orig_EndScene = (_EndScene)SetUpGateway();
	std::cout << "gateway at: " << std::hex << orig_EndScene << std::dec << std::endl;
	if (orig_EndScene) {
		isSetUp = true;
	}
}

void* Hook::SetUpGateway() {
	std::cout << "Hook::SetUpGateway()\n";
	LPVOID pGateway = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pGateway) {
		return nullptr;
	}

	memcpy(pGateway, &stolenBytes, len);

	*((BYTE*)pGateway + len) = '\xE9';
	int relativeOffset = (int)funcToHook - (int)pGateway - 5;
	*(int*)((int)pGateway + len + 1) = relativeOffset;

	return pGateway;
}

void Hook::TurnOn() {
	std::cout << "Hook::TurnOn()\n";

	DWORD oldProtect;
	VirtualProtect(funcToHook, len, PAGE_EXECUTE_READWRITE, &oldProtect);

	*(BYTE*)funcToHook = '\xE9';
	int relativeOffset = (int)Draw::DrawEntry - (int)funcToHook - 5;
	*(int*)((int)funcToHook + 1) = relativeOffset;

	VirtualProtect(funcToHook, len, oldProtect, &oldProtect);

	isHooked = true;
}

void Hook::TurnOff() {
	std::cout << "Hook::TurnOff()\n";

	DWORD oldProtect;
	VirtualProtect(funcToHook, len, PAGE_EXECUTE_READWRITE, &oldProtect);

	memcpy(funcToHook, &stolenBytes, len);


	VirtualProtect(funcToHook, len, oldProtect, &oldProtect);

	isHooked = false;
}
