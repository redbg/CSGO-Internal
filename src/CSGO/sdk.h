#pragma once

#include "hazedumper.h"
#include "src/SS/Math/Math.h"
#include <Windows.h>

using namespace SS::Math;

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b)      STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size)       STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) \
    struct                                  \
    {                                       \
        unsigned char MAKE_PAD(offset);     \
        type          name;                 \
    }

#define NUM_ENT_ENTRIES 32

namespace CSGO
{
    // ================================================== ClientEntity ==================================================

    struct ClientEntity
    {
        union
        {
            DEFINE_MEMBER_N(bool, m_bDormant, hazedumper::signatures::m_bDormant);           // 是否休眠
            DEFINE_MEMBER_N(int, m_iHealth, hazedumper::netvars::m_iHealth);                 // 血量
            DEFINE_MEMBER_N(Vector, m_vecOrigin, hazedumper::netvars::m_vecOrigin);          // 坐标
            DEFINE_MEMBER_N(Vector, m_vecViewOffset, hazedumper::netvars::m_vecViewOffset);  // 视图坐标
            DEFINE_MEMBER_N(Matrix3x4 *, m_BoneMatrix, hazedumper::netvars::m_dwBoneMatrix); // 骨骼矩阵
            DEFINE_MEMBER_N(int, m_iTeamNum, hazedumper::netvars::m_iTeamNum);               // 队伍
            DEFINE_MEMBER_N(int, m_iGlowIndex, hazedumper::netvars::m_iGlowIndex);
        };
    };

    struct ClientEntityInfo
    {
    public:
        ClientEntity     *m_pEntity;
        int               m_SerialNumber;
        ClientEntityInfo *m_pPrev;
        ClientEntityInfo *m_pNext;
    };

    class ClientEntityList
    {
    public:
        ClientEntityInfo clientEntityInfo[NUM_ENT_ENTRIES];
    };

    struct GlowStruct
    {
        int           nextFreeSlot;
        ClientEntity *entity;

        float red;
        float green;
        float blue;
        float alpha;

        bool  glowAlphaCappedByRenderAlpha;
        float glowAlphaFunctionOfMaxVelocity;
        float glowAlphaMax;
        float glowPulseOverdrive;
        bool  renderWhenOccluded;
        bool  renderWhenUnoccluded;
        bool  fullBloomRender;
        int   fullBloomStencilTestValue;
        int   glowStyle;
        int   splitScreenSlot;
    };

    // ================================================== ClientState ==================================================

    struct ClientState
    {
        DEFINE_MEMBER_N(Vector, m_ViewAngles, hazedumper::signatures::dwClientState_ViewAngles); // 视角
    };

    // ================================================== Data ==================================================

    DWORD client = (DWORD)GetModuleHandle(TEXT("client.dll"));
    // Matrix4x4        *ViewMatrix        = (Matrix4x4 *)(client + hazedumper::signatures::dwViewMatrix);
    ClientEntityList *clientEntityList  = (ClientEntityList *)(client + hazedumper::signatures::dwEntityList);
    ClientEntity    **localPlayer       = (ClientEntity **)(client + hazedumper::signatures::dwLocalPlayer);
    GlowStruct      **glowObjectManager = (GlowStruct **)(client + hazedumper::signatures::dwGlowObjectManager);

    DWORD         engine      = (DWORD)GetModuleHandle(TEXT("engine.dll"));
    ClientState **clientState = (ClientState **)(engine + hazedumper::signatures::dwClientState);
} // namespace CSGO
