/*!***********************************************************************
\file	ParticleEffect.hpp
\author	Shirley Liau
\date	23/02/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	ParticleEffect is responsible for player particle spawn.

\par	Changes: None
**************************************************************************/

#pragma once
#ifndef _Particle_Effect_Header_
#define _Particle_Effect_Header_

#include <AEEngine.h>
#include <TextureManager.hpp>
#include <GameObject.hpp>
#include <MeshGenerator.hpp>
static const int MAX_PARTICLES = 300;

static const Color palette[] = {
    Colors::PastelPink,
    Colors::PastelOrange,
    Colors::PastelPurple,
    Colors::PastelBlue,
    Colors::PastelGreen,
    Colors::PastelYellow
};

struct ImageTexture {
    Texture coffee;
    Texture candycorn;
    Texture cottoncandy;
    Texture macaron;
    Texture sweet;
};


enum TYPE {
    GRAVITY,
    PLAYER
};

struct ParticleObjInst
{
    AEGfxVertexList*    pMesh;
    bool        		flag;		
    AEVec2				scale;		
    AEVec2				pos;
    float               lifetime;
    float               lifeRemaining;
    float               alpha;
    float               speed;
    float				dirCurr;
    AEMtx33				transform;	
    Color               color;
    TYPE                type;
    bool                alternate;
    GameObject          candy;

};

class ParticleEffect
{
private:
    ParticleObjInst particles[MAX_PARTICLES];
    AEGfxVertexList* circleMesh, *squareMesh;
    AEVec2 playerPrevPos;
    TextureManager& tex = TextureManager::GetInstance();
    Texture* imageTextures[4];

    int gravityParticle;
    int playerParticle;

public:
    ParticleEffect();

    int ActiveParticle(TYPE type);

    void Init();
    void Emit(AEVec2 position);
    void Emit(AEVec2 position, GameObjectTag grav);
    void Update(float dt, AEVec2 position, GameObjectTag grav = GameObjectTag::Static);
    void DrawParticle();
    void Clear();
    void ResizeGrav();
    void ResizePlayer(bool isSprint);
};

#endif // !_Particle_Effect_Header_