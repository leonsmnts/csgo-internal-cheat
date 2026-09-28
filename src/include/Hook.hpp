#pragma once

#include <d3d9.h>

typedef HRESULT(APIENTRY* _EndScene)(LPDIRECT3DDEVICE9 pDevice);

class Hook {
public:
	_EndScene orig_EndScene;
	void Toggle();
	void Shutdown();

private:
	const int len = 7;

	bool isSetUp = false;
	bool isHooked = false;
	void* funcToHook = 0;
	BYTE stolenBytes[7] = { 0 };

	void SetUp();
	void* SetUpGateway();
	void TurnOn();
	void TurnOff();
};
