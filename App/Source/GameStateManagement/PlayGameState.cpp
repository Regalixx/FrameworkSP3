// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "PlayGameState.h"
// Include CGameStateManager
#include "GameStateManager.h"
// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include <iostream>
#include <System\filesystem.h>
using namespace std;

/**
 @brief Constructor
 */
CPlayGameState::CPlayGameState(void)
	: cScene2D(NULL)
{

}

/**
 @brief Destructor
 */
CPlayGameState::~CPlayGameState(void)
{

}

/**
 @brief Init this class instance
 */
bool CPlayGameState::Init(void)
{
	cout << "CPlayGameState::Init()\n" << endl;

	// Initialise the cScene2D instance
	cScene2D = CScene2D::GetInstance();
	if (cScene2D->Init() == false)
	{
		cout << "Failed to load Scene2D" << endl;
		return false;
	}
	cGameManager = CGameManager::GetInstance();
	return true;

	cSoundController = CSoundController::GetInstance();

	cSoundController->LoadSound(FileSystem::getPath("Sounds\\bg_music2.ogg"), 6, true);
	cSoundController->PlaySoundByID(6);

	
}

/**
 @brief Update this class instance
 */
bool CPlayGameState::Update(const double dElapsedTime)
{
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
	{
		
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading MenuState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		CGameStateManager::GetInstance()->OffPauseGameState();
		return true;

	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_G))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();
		// Load the menu state
		cout << "Loading PauseState" << endl;
		CGameStateManager::GetInstance()->SetPauseGameState("PauseState");
	}

	// Call the cScene2D's Update method

	if (cGameManager->bPlayerWon == true)
	{
		CKeyboardController::GetInstance()->Reset();
		// Load the menu state
		cout << "Loading VictoryState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("VictoryState");
	}

	cScene2D->Update(dElapsedTime);

	return true;
}

/**
 @brief Render this class instance
 */
void CPlayGameState::Render(void)
{
	//cout << "CPlayGameState::Render()\n" << endl;

	// Call the cScene2D's Pre-Render method
	cScene2D->PreRender();

	// Call the cScene2D's Render method
	cScene2D->Render();

	// Call the cScene2D's PostRender method
	cScene2D->PostRender();
}

/**
 @brief Destroy this class instance
 */
void CPlayGameState::Destroy(void)
{
	cout << "CPlayGameState::Destroy()\n" << endl;

	// Destroy the cScene2D instance
	if (cScene2D)
	{
		cScene2D->Destroy();
		cScene2D = NULL;
	}
}