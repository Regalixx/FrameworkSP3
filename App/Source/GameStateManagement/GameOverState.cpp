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

#include "GameOverState.h"

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
#include <System\filesystem.h>
using namespace std;

/**
 @brief Constructor
 */
CGameOverState::CGameOverState(void)
	: background(NULL)
{

}

/**
 @brief Destructor
 */
CGameOverState::~CGameOverState(void)
{

}

/**
 @brief Init this class instance
 */
bool CGameOverState::Init(void)
{
	cout << "CGameOverState::Init()\n" << endl;

	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);
	cGameManager = CGameManager::GetInstance();

	//Create Background Entity
	background = new CBackgroundEntity("Image/GameOver.png");
	background->SetShader("2DShader");
	background->Init();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(CSettings::GetInstance()->pWindow, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load the images for buttons
	CImageLoader* il = CImageLoader::GetInstance();
	retryButtonData.fileName = "Image\\GUI\\retry.png";
	retryButtonData.textureID = il->LoadTextureGetID(retryButtonData.fileName.c_str(), false);
	MenuButtonData.fileName = "Image\\GUI\\Menu.png";
	MenuButtonData.textureID = il->LoadTextureGetID(MenuButtonData.fileName.c_str(), false);
	exitButtonData.fileName = "Image\\GUI\\ExitButton2.png";
	exitButtonData.textureID = il->LoadTextureGetID(exitButtonData.fileName.c_str(), false);

	cSoundController = CSoundController::GetInstance();

//	cSoundController->LoadSound(FileSystem::getPath("Sounds\\gamevictory.ogg"), 8, true);

	return true;
}

/**
 @brief Update this class instance
 */
bool CGameOverState::Update(const double dElapsedTime)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;

	float buttonWidth = 220;
	float buttonHeight = 120;

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		// Create a window called "Hello, world!" and append into it.
		ImGui::Begin("Main Menu", NULL, window_flags);
		ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->iWindowWidth/2.0 - buttonWidth/2.0, 
			CSettings::GetInstance()->iWindowHeight/3.0));				// Set the top-left of the window at (10,10)
		ImGui::SetWindowSize(ImVec2(CSettings::GetInstance()->iWindowWidth, CSettings::GetInstance()->iWindowHeight));

		//Added rounding for nicer effect
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 50.0f;
		
		// Add codes for Start button here
		if (ImGui::ImageButton((ImTextureID)retryButtonData.textureID, 
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{
			// Reset the CKeyboardController
			cSoundController->SetVolumeTo0();
			CKeyboardController::GetInstance()->Reset();
			cSoundController->PlaySoundByID(6);
			// Load the menu state
	//		cout << "Loading PlayGameState" << endl;
			CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		}

		if (ImGui::ImageButton((ImTextureID)MenuButtonData.textureID,
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{
			CKeyboardController::GetInstance()->Reset();



			// Load the menu state
			cSoundController->SetVolumeTo0();
			CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
			cSoundController->PlaySoundByID(8);
		}

		// Add codes for Exit button here
		if (ImGui::ImageButton((ImTextureID)exitButtonData.textureID,
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{
			// Reset the CKeyboardController
			CKeyboardController::GetInstance()->Reset();

			// Load the menu state
		//	cout << "Quitting the game from MenuState" << endl;

			return false;
		}

		

		ImGui::End();

	}

	//For keyboard controls
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_SPACE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
	//	cout << "Loading PlayGameState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
	//	cout << "Quitting the game from MenuState" << endl;
		return false;
	}

	return true;
}

/**
 @brief Render this class instance
 */
void CGameOverState::Render(void)
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);

	//Render Background
	background->Render();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	cGameManager->bPlayerLost = true;
	cout << "CGameOverState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void CGameOverState::Destroy(void)
{
	// Delete the background
	if (background)
	{
		delete background;
		background = NULL;
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	cout << "CGameOverState::Destroy()\n" << endl;
}
