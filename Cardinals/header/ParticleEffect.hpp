/**
 * @file ParticleEffect.hpp
 * @brief ParticleEffect is responsible for player and gravity particle spawn.
 * @details -
 * @author Shirley Liau
 * @date 20.01.2026
 */
#pragma once
#ifndef _Particle_Effect_Header_
#define _Particle_Effect_Header_

#include <AEEngine.h>
#include <TextureManager.hpp>
#include <GameObject.hpp>
#include <MeshGenerator.hpp>
 /**
  * @int MAX_PARTICLES
  * @brief restrict the total Particle in game state
  */
static const int MAX_PARTICLES = 300;

/**
 * @struct palette
 * @brief Color use for particles
 */
static const Color palette[] = {
    Colors::PastelPink,
    Colors::PastelOrange,
    Colors::PastelPurple,
    Colors::PastelBlue,
    Colors::PastelGreen,
    Colors::PastelYellow
};

/**
 * @struct ImageTexture
 * @brief Image use as particles
 */
struct ImageTexture {
    Texture coffee;
    Texture candycorn;
    Texture cottoncandy;
    Texture macaron;
    Texture sweet;
};

/**
 * @enum Type
 * @brief type of particles different type has different direction
 */
enum TYPE {
    GRAVITY,
    PLAYER
};

/**
 * @struct ParticleObjInst
 * @brief Hold the needed instance for a particle 
 */
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

/**
 * @class Overlay
 * @brief Manages particles creation, spawning position, clearing.
 */
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
    /**
    * @brief Construct a Particle object and initializes its default properties.
    */
    ParticleEffect();

    /**
    * @brief Returns the number of active particles of a given type.
    * @param Type type of particle gravity or player
    * @return Count of particles
    */
    int ActiveParticle(TYPE type);

    /**
    * @brief Initializes the particle system by setting up meshes, loading textures
    */
    void Init();

    /**
    * @brief Emits particles at a specified position with random direction close the the position given.
    * @param AEVec2 position of player or gravity tile
    */
    void Emit(AEVec2 position);
    
    /**
    * @brief Emits particles at a specified position close the the position given with fix direction depending of grav tag.
    * @param AEVec2 position of player or gravity tile
    * @param GameObjectTag type of gravity tile up, down, left, right
    */
    void Emit(AEVec2 position, GameObjectTag grav);
    
    /**
    * @brief Updates all particles based on delta time, position.
    */
    void Update(float dt, AEVec2 position, GameObjectTag grav = GameObjectTag::Static);
    
    /**
    * @brief Renders all active particles to the screen.
    */
    void DrawParticle();
    
    /**
    * @brief Clears and resets all particles in the system.
    */
    void Clear();
    
    /**
    * @brief Increase particle spawn if its gravity effects.
    */
    void ResizeGrav();

    /**
    * @brief Adjusts particle properties based on player movement.
    */
    void ResizePlayer(bool isSprint);
};

#endif // !_Particle_Effect_Header_