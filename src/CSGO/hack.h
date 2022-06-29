#pragma once
#include "sdk.h"
#include <stdio.h>

#define PI 3.141592653589793f

namespace CSGO
{
    Vector CalcAngle(Vector viewAngle, Vector aimPos)
    {
        Vector angle = {0, 0, 0};

        Vector delta = aimPos - viewAngle;

        angle.x = -asin(delta.z / delta.Size()) * (180.0f / PI); // pitch
        angle.y = atan2(delta.y, delta.x) * (180.0f / PI);       // yaw

        return angle;
    }

    Vector NormalizeAngles(Vector angle)
    {
        while (angle.x > 89.0f)
            angle.x -= 180.f;

        while (angle.x < -89.0f)
            angle.x += 180.f;

        while (angle.y > 180.f)
            angle.y -= 360.f;

        while (angle.y < -180.f)
            angle.y += 360.f;

        return angle;
    }

    float GetFov(Vector viewAngle, Vector aimAngle)
    {
        Vector delta = aimAngle - viewAngle;
        delta        = NormalizeAngles(delta);
        return sqrt(pow(delta.x, 2.0f) + pow(delta.y, 2.0f));
    }

    void AimBot()
    {
        // 自瞄
        if ((GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_LCONTROL)) && *localPlayer != NULL)
        {
            Vector        vecView      = (*localPlayer)->m_vecOrigin + (*localPlayer)->m_vecViewOffset;
            Vector        aimbotAngle  = {0, 0, 0};
            ClientEntity *aimbotEntity = NULL;

            for (size_t i = 0; i < NUM_ENT_ENTRIES; i++)
            {
                ClientEntityInfo *entityInfo = &clientEntityList->clientEntityInfo[i];

                if (ClientEntity *entity = entityInfo->m_pEntity)
                {
                    if (entity != *localPlayer &&
                        entity->m_iTeamNum != (*localPlayer)->m_iTeamNum &&
                        entity->m_bDormant == false &&
                        entity->m_iHealth > 0)
                    {
                        // 遍历骨骼
                        for (int j = 4; j <= 8; j += 2)
                        {
                            // 瞄准位置
                            Matrix3x4 boneMatrix = entity->m_BoneMatrix[j];
                            Vector    vecBone    = Vector{boneMatrix.m14, boneMatrix.m24, boneMatrix.m34};

                            Vector aimbotAngleTemp = CalcAngle(vecView, vecBone);

                            // 冒泡排序
                            if (GetFov((*clientState)->m_ViewAngles, aimbotAngleTemp) < GetFov((*clientState)->m_ViewAngles, aimbotAngle) || aimbotEntity == NULL)
                            {
                                aimbotEntity = entity;
                                aimbotAngle  = aimbotAngleTemp;
                            }
                        }
                    }
                }
            }

            // Set ViewAngles
            if (aimbotEntity != NULL)
            {
                // 发光
                (*glowObjectManager)[aimbotEntity->m_iGlowIndex].renderWhenOccluded = true;
                (*glowObjectManager)[aimbotEntity->m_iGlowIndex].alpha              = aimbotEntity->m_bSpotted ? 0.8 : 0.6;
                (*glowObjectManager)[aimbotEntity->m_iGlowIndex].green              = (1.0f / 100.0f) * aimbotEntity->m_iHealth;
                (*glowObjectManager)[aimbotEntity->m_iGlowIndex].red                = 1.0f - (*glowObjectManager)[aimbotEntity->m_iGlowIndex].green;

                if (GetFov((*clientState)->m_ViewAngles, aimbotAngle) < 10 && aimbotEntity->m_bSpotted)
                {
                    // 自瞄
                    Vector delta = NormalizeAngles(aimbotAngle - (*clientState)->m_ViewAngles);
                    (*clientState)->m_ViewAngles.x += delta.x / ((rand() % 50) + 10); // pitch
                    (*clientState)->m_ViewAngles.y += delta.y / ((rand() % 50) + 10); // yaw
                }
            }
        }
    }
} // namespace CSGO
