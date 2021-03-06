/**
 CClone
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CEntity2D
#include "Primitives/Entity2D.h"

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Settings
#include "GameControl\Settings.h"

// Include Physics2D
#include "Physics2D.h"

// Include Player2D
#include "Player2D.h"

// Include Clone
#include "Clone.h"

#include "InventoryItem.h"

#include "InventoryManager.h"

#include "Primitives//SpriteAnimation.h"

class CBlackhole : public CSingletonTemplate<CBlackhole>, public CEntity2D
{
public:
	// Constructor
	CBlackhole(void);

	// Destructor
	virtual ~CBlackhole(void);

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

	// Set the indices of the enemy2D

	// Set the handle to cPlayer to this class instanc
	// boolean flag to indicate if this enemy is active
	bool bIsActive;

	bool blackholePoint;
	bool renderBlackhole;

	glm::i32vec2 i32vec2RespawnIndex;

	bool canRespawnToClone;
	bool respawnToClone;

	//glm::i32vec2 i32vec2Index;
	

protected:
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};

	enum FSM
	{
		IDLE = 0,
		PATROL = 1,
		ATTACK = 2,
		NUM_FSM
	};


	glm::i32vec2 i32vec2OldIndex;

	


	CSpriteAnimation* animatedSprites;
	//CS: The quadMesh for drawing the tiles
	CMesh* quadMesh;

	// Handler to the CMap2D instance
	CMap2D* cMap2D;

	CClone* cClone;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// The i32vec2 which stores the indices of the enemy2D in the Map2D
	

	// The i32vec2 variable which stores The number of microsteps from the tile indices for the enemy2D. 
	// A tile's width or height is in multiples of these microsteps
	glm::i32vec2 i32vec2NumMicroSteps;

	// The vec2 variable which stores the UV coordinates to render the enemy2D
	glm::vec2 vec2UVCoordinate;

	// The i32vec2 which stores the indices of the destination for enemy2D in the Map2D
	glm::i32vec2 i32vec2Destination;
	// The i32vec2 which stores the direction for enemy2D movement in the Map2D
	glm::i32vec2 i32vec2Direction;

	// Settings
	CSettings* cSettings;

	// Physics
	CPhysics2D cPhysics2D;

	// Current color
	glm::vec4 currentColor;

	// Handle to the CPlayer2D
	CPlayer2D* cPlayer2D;

	// Current FSM
	FSM sCurrentFSM;



	//InventoryManager
	CInventoryManager* cInventoryManager;

	//InventoryItem
	CInventoryItem* cInventoryItem;

	//CSoundController
	CSoundController* cSoundController;


	// FSM counter - count how many frames it has been in this FSM
	int iFSMCounter;

	// Max count in a state
	const int iMaxFSMCounter = 60;

	// Load a texture
	bool LoadTexture(const char* filename, GLuint& iTextureID);

	// Constraint the enemy2D's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);

	// Check if a position is possible to move into
	bool CheckPosition(DIRECTION eDirection);

	// Check if the enemy2D is in mid-air
	bool IsMidAir(void);

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime = 0.0166666666666667);

};

