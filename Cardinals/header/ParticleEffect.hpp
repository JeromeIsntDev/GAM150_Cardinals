/*!***********************************************************************
\file	TextureManager.hpp
\author	Shirley Liau
\date	14/01/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	TextureManager is responsible for colors and images.

\par	Changes: None
**************************************************************************/

#pragma once
#ifndef _Particle_Effect_Header_
#define _Particle_Effect_Header_

#include <AEEngine.h>
#include <TextureManager.hpp>
#include <GameObject.hpp>
#include <MeshGenerator.hpp>

class ParticleEffect
{
private:
    f32 _x;
    f32 _y;
    f32 _changeX;
    f32 _changeY;

public:
    ParticleEffect();

    bool isObjectMoving(AEVec2 pos);
    AEVec2 Update_Particle(AEVec2 pos);
    bool Change_Position();
};

#endif // !_Particle_Effect_Header_