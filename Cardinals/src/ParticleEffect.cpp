/*!***********************************************************************
\file	ParticleEffect.cpp
\author	Shirley Liau
\date	23/02/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	ParticleEffect is responsible for player particle spawn.

\par	Changes: None
**************************************************************************/

#include "../header/ParticleEffect.hpp"
#include <cmath>

ParticleEffect::ParticleEffect()
    : circleMesh(nullptr), playerPrevPos{0.f,0.f}
{
}

void ParticleEffect::Init()
{
    circleMesh = CircleMesh(16);
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].flag = false;
        particles[i].pMesh = circleMesh;
    }

}

void ParticleEffect::Emit(AEVec2 position)
{
    float dx = position.x - playerPrevPos.x;
    float dy = position.y - playerPrevPos.y;
    float moveDir = atan2f(dy, dx);
    float oppositeDir = moveDir + PI;

    AEVec2 spawnPos = {0.f,0.f};
    

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particles[i].flag) continue; // skip active ones

        if (dx > 0)       spawnPos.x = - (25.f + ((float)rand() / RAND_MAX) * 20.f); // moving right
        else if (dx < 0)  spawnPos.x = 25.f + ((float)rand() / RAND_MAX) * 20.f; // moving left
        if (dy > 0)       spawnPos.y = - (25.f + ((float)rand() / RAND_MAX) * 20.f); // moving up
        else if (dy < 0)  spawnPos.y = 25.f + ((float)rand() / RAND_MAX) * 20.f; // moving down

        ParticleObjInst* p = &particles[i];
        p->flag = true;
        p->pos = { position.x + spawnPos.x, position.y + spawnPos.y };
        p->scale.x = 5.f + ((float)rand() / RAND_MAX) * 5.f;
        p->scale.y = p->scale.x;
        p->lifetime = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        p->lifeRemaining = p->lifetime;
        p->alpha = 1.0f;
        p->dirCurr = oppositeDir + (((float)rand() / RAND_MAX) - 0.5f) * PI;
        p->speed = 20.f + ((float)rand() / RAND_MAX) * 40.f;
        p->color = palette[rand() % 6];
        break; // spawn one per frame to stagger them
    }
}

void ParticleEffect::Emit()
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particles[i].flag) continue; // skip active ones
        float posX = - 800.f + ((float)rand() / RAND_MAX) * 1600.f;
        ParticleObjInst* p = &particles[i];
        p->flag = true;
        p->pos = { posX, -451.f };
        p->scale.x = 5.f + ((float)rand() / RAND_MAX) * 5.f;
        p->scale.y = p->scale.x;
        p->lifetime = 1.f + ((float)rand() / RAND_MAX) * 3.f;
        p->lifeRemaining = p->lifetime;
        p->alpha = 1.0f;
        p->speed = 40.f + ((float)rand() / RAND_MAX) * 70.f;
        p->color = palette[rand() % 6];
        break; // spawn one per frame to stagger them
    }
}

void ParticleEffect::Update(float dt, AEVec2 playerPos)
{
    // only emit if player is actually moving
    bool isMoving = (playerPos.x != playerPrevPos.x || playerPos.y != playerPrevPos.y);

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;

        p->lifeRemaining -= dt;
        p->alpha = p->lifeRemaining / p->lifetime;
        p->pos.x += cosf(p->dirCurr) * p->speed * dt;
        p->pos.y += sinf(p->dirCurr) * p->speed * dt;

        if (p->lifeRemaining <= 0.f)
            p->flag = false;
    }

    if (isMoving)
        Emit(playerPos);

    playerPrevPos = playerPos;
}

void ParticleEffect::Update(float dt)
{

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;

        p->lifeRemaining -= dt;
        p->alpha = p->lifeRemaining / p->lifetime;
        p->pos.y += p->speed * dt;

        if (p->lifeRemaining <= 0.f)
            p->flag = false;
    }

    Emit();
}

void ParticleEffect::DrawParticle()
{
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxTextureSet(nullptr, 0, 0); // clear any bound texture

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;

        AEMtx33 scale, trans;
        AEMtx33Scale(&scale, p->scale.x, p->scale.y);
        AEMtx33Trans(&trans, p->pos.x, p->pos.y);
        AEMtx33Concat(&p->transform, &trans, &scale);
        AEGfxSetTransform(p->transform.m);
        AEGfxSetTransparency(p->alpha);
        AEGfxSetColorToMultiply(1.f, 1.f, 1.f, p->alpha);
        AEGfxSetColorToAdd(p->color.r, p->color.g, p->color.b, 0.f);
        AEGfxMeshDraw(p->pMesh, AE_GFX_MDM_TRIANGLES);
    }

    // reset state so nothing bleeds into subsequent draws
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
}

void ParticleEffect::Clear()
{
    if (circleMesh)
        AEGfxMeshFree(circleMesh);

}