#pragma once
#include "hack.h"
#include "src/SS/Memory/Memory.h"
#include <Windows.h>
#include <d3d9.h>

#ifndef _DEBUG
#define printf
#endif


typedef HRESULT(__stdcall *Present_t)(IDirect3DDevice9 *device,
                                      CONST RECT       *pSourceRect,
                                      CONST RECT       *pDestRect,
                                      HWND              hDestWindowOverride,
                                      CONST RGNDATA    *pDirtyRegion);

namespace CSGO
{
    Present_t **present         = (Present_t **)(SS::Memory::FindPattern("GameOverlayRenderer.dll", "\xFF\x15????\x8B\xF0\x85\xFF") + 2);
    Present_t   originalPresent = **present;

    HRESULT __stdcall MyPresent(IDirect3DDevice9 *device,
                                CONST RECT       *pSourceRect,
                                CONST RECT       *pDestRect,
                                HWND              hDestWindowOverride,
                                CONST RGNDATA    *pDirtyRegion)
    {
        CSGO::AimBot();
        return originalPresent(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }

    void Hook()
    {
        **present = MyPresent;
    }

    void UnHook()
    {
        **present = originalPresent;
    }
} // namespace CSGO
