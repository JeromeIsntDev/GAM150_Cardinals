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
    : circleMesh(nullptr), squareMesh(nullptr), playerPrevPos{ 0.f,0.f }, gravityParticle{ 0 }, playerParticle{ 15 }

{
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        particles[i] = {};
    }
    for (int j = 0; j < 4; ++j)
    {
        imageTextures[j] = nullptr;
    }
}

void ParticleEffect::Init()
{
    circleMesh = CircleMesh(16);
    squareMesh = RectMeshCenter();

    imageTextures[0] = tex.Get("Candy_Corn1");
    imageTextures[1] = tex.Get("Cotton_Candy");
    imageTextures[2] = tex.Get("Macaron");
    imageTextures[3] = tex.Get("Sweet");    
    
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].flag = false;
        particles[i].alternate = (i % 4 == 0);
        particles[i].pMesh = circleMesh;
        if (particles[i].alternate) {
            particles[i].candy.SetMesh(squareMesh);
        }
    }
}

void ParticleEffect::Emit(AEVec2 position)
{
    if (ActiveParticle(TYPE::PLAYER) >= playerParticle)
        return;

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
        p->type = TYPE::PLAYER;
        p->pos = { position.x + spawnPos.x, position.y + spawnPos.y };
        p->scale.x = 5.f + ((float)rand() / RAND_MAX) * 5.f;
        p->scale.y = p->scale.x;
        p->lifetime = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        p->lifeRemaining = p->lifetime;
        p->alpha = 1.0f;
        p->dirCurr = oppositeDir + (((float)rand() / RAND_MAX) - 0.5f) * PI;
        p->speed = 20.f + ((float)rand() / RAND_MAX) * 40.f;
        p->color = palette[rand() % 6];

        if (p->alternate) {
            float scale = 0.f;
            scale = 10.f + ((float)rand() / RAND_MAX) * 5.f;
            particles[i].candy.SetTexture(imageTextures[rand() % 4]);
            p->candy.UpdatePosition(p->pos);
            p->candy.UpdateScale({ scale, scale });
        }

        playerParticle--;
        break; // spawn one per frame to stagger them
    }
}

void ParticleEffect::Emit(AEVec2 position, GameObjectTag grav)
{
    if (ActiveParticle(TYPE::GRAVITY) >= gravityParticle)
        return;

    AEVec2 spawnPos = { 0.f,0.f };
    float baseDir = 0.f;

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particles[i].flag) continue; // skip active ones

        spawnPos = { 0.f, 0.f };

        switch (grav) {
        case GameObjectTag::GravUp:
            spawnPos.x = ((float)rand() / RAND_MAX) * 50.f - 25.f;
            spawnPos.y = 25;
            baseDir = PI / 2.f;
            break;

        case GameObjectTag::GravDown:
            spawnPos.x = ((float)rand() / RAND_MAX) * 50.f - 25.f;
            spawnPos.y = -25;
            baseDir = -PI / 2.f;
            break;

        case GameObjectTag::GravLeft:
            spawnPos.x = -25;
            spawnPos.y = ((float)rand() / RAND_MAX) * 50.f - 25.f;
            baseDir = PI;
            break;
        case GameObjectTag::Gravright:
            spawnPos.x = 25;
            spawnPos.y = ((float)rand() / RAND_MAX) * 50.f - 25.f;
            baseDir = 0.f;
            break;
        default:
            return;
        }

        ParticleObjInst* p = &particles[i];
        p->flag = true;
        p->type = TYPE::GRAVITY;
        p->pos = { position.x + spawnPos.x, position.y + spawnPos.y };
        p->scale.x = 5.f + ((float)rand() / RAND_MAX) * 5.f;
        p->scale.y = p->scale.x;
        p->lifetime = 2.f + ((float)rand() / RAND_MAX) * 5.f;
        p->lifeRemaining = p->lifetime;
        p->alpha = 1.0f;
        p->dirCurr = baseDir;
        p->speed = 2.f + ((float)rand() / RAND_MAX) * 10.f;
        p->color = palette[rand() % 6];

        if (p->alternate) {
            particles[i].candy.SetTexture(imageTextures[rand() % 4]);
            p->candy.UpdatePosition(p->pos);
            p->candy.UpdateScale({ p->scale.x * 2, p->scale.y * 2 });
        }

        gravityParticle--;
        break; // spawn one per frame to stagger them
    }
}

void ParticleEffect::Update(float dt, AEVec2 pos, GameObjectTag grav)
{

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;

        p->lifeRemaining -= dt;
        p->alpha = p->lifeRemaining / p->lifetime;
        p->pos.x += cosf(p->dirCurr) * p->speed * dt;
        p->pos.y += sinf(p->dirCurr) * p->speed * dt;

        if (p->alternate) {
            p->candy.UpdatePosition(p->pos);
        }

        if (p->lifeRemaining <= 0.f) {
            if (p->type == TYPE::PLAYER) {
                playerParticle++;
            }
            else if (p->type == TYPE::GRAVITY) {
                gravityParticle++;
            }

            p->flag = false;
        }
    }

    bool isPlayer = (grav == GameObjectTag::Static);

    if (isPlayer)
    {
        if (pos.x != playerPrevPos.x || pos.y != playerPrevPos.y)
            Emit(pos);
        playerPrevPos = pos;
    }
    else
    {
        Emit(pos, grav);
    }
}

void ParticleEffect::DrawParticle()
{
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxTextureSet(nullptr, 0, 0); // clear any bound texture

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;
        if (p->alternate) continue;

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

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;
        if (!p->alternate) continue;

        if (p->candy.GetTexture()) {
            p->candy.Draw();
        }
    }
}

void ParticleEffect::Clear()
{
    if (circleMesh) {
        AEGfxMeshFree(circleMesh);
        circleMesh = nullptr;
    }
    if (squareMesh) {
        AEGfxMeshFree(squareMesh);
        squareMesh = nullptr;
    }

}

int ParticleEffect::ActiveParticle(TYPE type) {
    int count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        ParticleObjInst* p = &particles[i];
        if (!p->flag) continue;
        if (p->type == type) count++;
    }
    return count;
}

void ParticleEffect::ResizeGrav() {
    gravityParticle += 10;
}

void ParticleEffect::ResizePlayer(bool isSprint) {
    playerParticle = isSprint ? 50 : 15;
}