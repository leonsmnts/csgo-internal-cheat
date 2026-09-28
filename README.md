# CS:GO Internal Cheat

A small x86 internal cheat for Counter-Strike: Global Offensive written in C++.

I originally wrote this project around 2022 while experimenting with game hacking, reverse engineering, game memory, and Direct3D 9 hooking.

This repository is preserved as an old learning project. It targets the legacy 32-bit CS:GO client, uses historical hard-coded offsets, and is not compatible with Counter-Strike 2.

## Features

- Direct access to reverse-engineered game structures
- Direct3D 9 `EndScene` trampoline hook for in-game rendering
- World-to-screen projection
- Box ESP
- Bone ESP with model-specific bone mappings
- Entity filtering and target selection
- Simple view-angle aimbot
- Runtime feature toggling through keyboard input

## Project Structure

The DLL starts in [`dllmain.cpp`](src/dllmain.cpp), where a worker thread is created before entering the main cheat loop in [`Hack.cpp`](src/Hack.cpp).

The main loop handles feature toggling, updates pointers to relevant game state, and runs the aimbot when enabled.

- [`Hack.cpp`](src/Hack.cpp) contains the main loop, game offsets, entity filtering, and target selection
- [`Hook.cpp`](src/Hook.cpp) implements the Direct3D 9 `EndScene` trampoline hook
- [`Draw.cpp`](src/Draw.cpp) contains ESP rendering and world-to-screen projection
- [`Aimbot.cpp`](src/Aimbot.cpp) contains the target-angle calculations and view-angle manipulation
- [`Bone_ESP_Helper.hpp`](src/include/Bone_ESP_Helper.hpp) contains model-specific bone mappings used by the bone ESP
- [`csgo_header.hpp`](src/include/csgo_header.hpp) contains reconstructed game/entity structures
- [`Globals.hpp`](src/include/Globals.hpp) contains shared runtime state used by the different components

## Build Requirements

- Windows / x86 (Win32)
- Visual Studio with C++ desktop development tools
- Microsoft DirectX SDK (June 2010)
- Direct3D 9 / D3DX9

The Visual Studio project currently expects the June 2010 DirectX SDK at the default installation path:

```text
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\
```

The include and x86 library paths are hard-coded in [`src/csgo-internal.vcxproj`](src/csgo-internal.vcxproj) and may need to be changed if the SDK is installed elsewhere.

## Limitations

This is an older experimental project and not production-quality code.

- x86 only
- Uses hard-coded offsets for an old CS:GO version
- Not compatible with Counter-Strike 2
- Relies on legacy DirectX 9 / D3DX components
- Does not include explicit anti-cheat bypass functionality; manual mapping was sufficient for loading and executing the DLL, but this does not imply that the cheat was undetected
