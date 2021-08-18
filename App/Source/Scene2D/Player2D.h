/**
 CPlayer2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include Singleton template
#include "DesignPatterns\SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CEntity2D
#include "Primitives/Entity2D.h"

// Include Mesh
#include "Primitives/Mesh.h"

//Include Physics2D
#include "Physics2D.h"

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

//Include AnimatedSprites
#include "Primitives/SpriteAnimation.h"

#include "GUI_Scene2D.h"

//Include InventoryManager
#include "InventoryManager.h"


#include "InventoryItem.h"

#include "GameManager.h"
#include "../SoundController/SoundController.h"




class CPlayer2D : public CSingletonTemplate<CPlayer2D>, public CEntity2D
{
	friend CSingletonTemplate<CPlayer2D>;
public:

	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	bool ResetMap();

	glm::i32vec2 i32vec2OldIndex;

	glm::vec4 playerColour;

	bool TimeStop;
	

protected:
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};

	// Store the old indices


	// Handler to the CMap2D instance
	CMap2D* cMap2D;
	CMap2D* cMap2D2;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	//CS: The quadMesh for drawing the tiles
	CMesh* quadMesh;

	//CS: Animated Sprite
	CSpriteAnimation* animatedSprites;

	//Player's color

	//Physics
	CPhysics2D cPhysics2D;

	//InventoryManager
	CInventoryManager* cInventoryManager;

	//InventoryItem
	CInventoryItem* cInventoryItem;

	CGameManager* cGameManager;


	//Handler to the CSoundController
	CSoundController* cSoundController;

	bool activateDoor;
	bool powerupActive;
	bool speedboost;
	bool isHealing;
	bool slowMovementSpeed;
	bool jumppoweractive;

	float cooldownTimer;
	float freezeDuration;
	bool canUsepower;
	int switchesActivated;

	//Remote Bool
	bool isRemote = false;
	
	
	// Constructor
	CPlayer2D(void);

	// Destructor
	virtual ~CPlayer2D(void);

	// Load a texture
	bool LoadTexture(const char* filename, GLuint& iTextureID);

	// Constraint the player's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);

	// Check if a position is possible to move into
	bool CheckPosition(DIRECTION eDirection);

	// Check if the player is in mid-air
	bool IsMidAir(void);

	//move lift
	bool IsLiftMoving(void);
	bool IsLiftSwitchStepped(void);

	//for timestop dimension
	bool dimension(void);
	void dimensionchange();

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime = 0.0166666666666667);

	// Let player interact with the map
	void InteractWithMap(void);

	void UpdateHealthLives(void);
};

