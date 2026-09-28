// dllmain.cpp
#include "pch.h"
#include "Hack.hpp"

#include <iostream>

/*
 Creates Console and attaches stdout to it.
 Returns 0 if error happened.
 Returns FILE* of the console to be able to properly close it later.
*/
FILE* StartConsole() {
    int retVal;
    
    retVal = AllocConsole();
    if (!retVal) {
        return 0;
    }

    FILE* new_console;
    retVal = freopen_s(&new_console, "CONOUT$", "w", stdout);

    if (retVal) {
        FreeConsole();
        return 0;
    }

    return new_console;
}

void EndConsole(FILE* new_console) {
    fclose(new_console);
    FreeConsole();
}

DWORD WINAPI setupMethod(LPVOID lpParameter) {
    // Allocates Console to be able to use std::cout
    // If not needed remove and remove EndConsole() also.
    FILE* new_console = StartConsole();

    // HERE IS THE MAIN HACK-LOOP
    mainLoop();

    // Always include if using the Console.
    if (new_console) {
        EndConsole(new_console);
    }

    // TODO: Likely does nothing if DLL was manually mapped
    FreeLibraryAndExitThread((HMODULE)lpParameter, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE thread_handle = CreateThread(0, 0, setupMethod, hModule, 0, 0);

        if (thread_handle) {
            CloseHandle(thread_handle);
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}