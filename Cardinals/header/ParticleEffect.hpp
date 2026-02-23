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
static const int MAX_PARTICLES = 30;

static const Color palette[] = {
    Colors::PastelPink,
    Colors::PastelOrange,
    Colors::PastelPurple,
    Colors::PastelBlue,
    Colors::PastelGreen,
    Colors::PastelYellow
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
};

class ParticleEffect
{
private:
    ParticleObjInst particles[MAX_PARTICLES];
    AEGfxVertexList* circleMesh;
    AEVec2 playerPrevPos;

public:
    ParticleEffect();

    void Init();
    void Emit(AEVec2 position);
    void Update(float dt, AEVec2 position);
    void DrawParticle();
    void Clear();
};

#endif // !_Particle_Effect_Header_