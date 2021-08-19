/**
 CScene2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

//Include SingletonTemplate
#include "DesignPatterns/SingletonTemplate.h"
 // Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// GUI
#include "GUI/GUI.h"

#include "GUI_Scene2D.h"
#include "Map2D.h"

//Include CPlayer2D
#include "Player2D.h"

//Game Manager
#include "GameManager.h"

#include "Primitives/Entity2D.h"



//Include SoundController
#include "..\SoundController\SoundController.h"

//Include CEnemey2D
#include "Enemy2D.h"


#include "../GameStateManagement/GameStateManager.h"

#include "Monster2D.h"


#include "Enemy3.h"

#include <vector>


#include "../Scene2D/BackgroundEntity.h"
#include "Clone.h"


struct GUI
{
	std::string fileName;
	unsigned textureID;
};


class CScene2D : public CSingletonTemplate<CScene2D>
{
	friend CSingletonTemplate<CScene2D>;
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

protected:

	CMap2D* cMap2D;
	CPlayer2D* cPlayer2D;

	CMesh* quadMesh;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// Constructor
	CScene2D(void);
	// Destructor
	virtual ~CScene2D(void);

	// GUI
	CGUI_Scene2D* cGUI_Scene2D;

	float MapLevel;

	bool enemyCleared = false;

	CGameManager* cGameManager;

	//Handler to the CSoundController
	CSoundController* cSoundController;

	GUI healthbar;

	vector<CEntity2D*> enemyVector; //for type1 enemy
	vector<CEntity2D*> enemyVector2; //for type2 enemy
	vector<CEntity2D*> enemyVector3; //for type3 enemy
	vector<CEntity2D*> cloneVector; //for type3 enemy

	CBackgroundEntity* background;
	CBackgroundEntity* dimensionstate;
	
	
};

