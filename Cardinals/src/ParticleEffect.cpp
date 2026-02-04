/*!***********************************************************************
\file	ParticleEffect.cpp
\author	Shirley Liau
\date	3/02/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	Particle Effect is responsible for player moving effect.

\par	Changes: None
**************************************************************************/

#include "../header/ParticleEffect.hpp"

ParticleEffect::ParticleEffect()
	: _changeX{}, _changeY{}, _x{}, _y{}
{
	PRINT("Effect Created\n");
}

bool ParticleEffect::isObjectMoving(AEVec2 pos) {
	if (_x != pos.x || _y != pos.y) {
		_changeX = pos.x == _x ? 0.f : (pos.x < _x ? 25.f : -25.f);
		_changeY = pos.y == _y ? 0.f : (pos.y < _y ? 25.f : -25.f);
		_x = pos.x;
		_y = pos.y;
		return true;
	}
	return false;
}

AEVec2 ParticleEffect::Update_Particle(AEVec2 pos)
{
	return { _x + _changeX, _y + _changeY };

}

bool ParticleEffect::Change_Position()
{
	if (_changeX > 0 || _changeY > 0 || _changeX < 0 || _changeY < 0) {
		_changeX -= _changeX > 0 ? 1 : 0;
		_changeY -= _changeY > 0 ? 1 : 0;
		_changeX += _changeX < 0 ? 1 : 0;
		_changeY += _changeY < 0 ? 1 : 0;
		return true;
	}
	return false;
}
