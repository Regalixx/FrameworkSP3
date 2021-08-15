// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "ControlState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include Mesh Builder
#include "Primitives/MeshBuilder.h"
// Include ImageLoader
#include "System\ImageLoader.h"
// Include Shader Manager
#include "RenderControl\ShaderManager.h"

 // Include shader
#include "RenderControl\shader.h"

// Include CSettings
#include "GameControl/Settings.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CControlState::CControlState(void)
	: background(NULL)
{

}

/**
 @brief Destructor
 */
CControlState::~CControlState(void)
{
}

/**
 @brief Init this class instance
 */
bool CControlState::Init(void)
{
	cout << "CControlState::Init()\n" << endl;

	// Include Shader Manager
	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	//Create Background Entity
	background = new CBackgroundEntity("Image/controls.png");
	background->SetShader("2DShader");
	background->Init();

	return true;
}

/**
 @brief Update this class instance
 */
bool CControlState::Update(const double dElapsedTime)
{
	//cout << "CControlState::Update()\n" << endl;
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_B))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
	//	cout << "Loading MenuState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		return true;
	}

	return true;
}

/**
 @brief Render this class instance
 */
void CControlState::Render()
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);

	//Draw the background
 	background->Render();
}

/**
 @brief Destroy this class instance
 */
void CControlState::Destroy(void)
{
	// Delete the background
	if (background)
	{
		delete background;
		background = NULL;
	}

	cout << "CControlState::Destroy()\n" << endl;
}