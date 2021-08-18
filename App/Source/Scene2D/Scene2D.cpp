#include "Scene2D.h"
#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

#include "System\filesystem.h"
#include "System\ImageLoader.h"
#include <Primitives\MeshBuilder.h>




/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene2D::CScene2D(void)
	: cMap2D(NULL)
	, cPlayer2D(NULL)
	, cKeyboardController(NULL)
	, cGUI_Scene2D(NULL)
	, cGameManager(NULL)
	, cSoundController(NULL)
	, background(NULL)
	
{
}

/**
 @brief Destructor
 */
CScene2D::~CScene2D(void)
{
	if (cGUI_Scene2D)
	{
		cGUI_Scene2D->Destroy();
		cGUI_Scene2D = NULL;
	}

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	if (cMap2D)
	{
		cMap2D->Destroy();
		cMap2D = NULL;
	}

	if (cPlayer2D)
	{
		cPlayer2D->Destroy();
		cPlayer2D = NULL;
	}

	if (cGameManager)
	{
		cGameManager->Destroy();
		cGameManager = NULL;
	}

	if (cSoundController)
	{
		cSoundController = NULL;
	}

	if (background)
	{
		delete background;
		background = NULL;
	}

	


	

	//Destroy the enemies
	for (int i = 0; i < enemyVector.size(); i++)
	{
		delete enemyVector[i];
		enemyVector[i] = NULL;
	}
	enemyVector.clear();

	for (int i = 0; i < enemyVector2.size(); i++)
	{
		delete enemyVector2[i];
		enemyVector2[i] = NULL;
	}
	enemyVector2.clear();

	for (int i = 0; i < enemyVector3.size(); i++)
	{
		delete enemyVector3[i];
		enemyVector3[i] = NULL;
	}
	enemyVector3.clear();
}

/**
@brief Init Initialise this instance
*/
bool CScene2D::Init(void)
{

	//// Include Shader Manager
	//CShaderManager::GetInstance()->Add("2DShader", "Shader//Scene2D.vs", "Shader//Scene2D.fs");
	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	

	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader("2DShader");
	// Initialise the instance
	if (cMap2D->Init(5, 24, 32) == false)
	{
		cout << "Failed to load CMap2D" << endl;
		return false;
	}



	//Level 1
	


	// Load the map into an array
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_Test.csv") == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_Test2.csv", 1) == false)

	{
		// The loading of a map has failed. Return false
		return false;
	}

	//Level 2



	//// Load the map into an array
	//if (cMap2D->LoadMap("Maps/DM2213_Map_Level_02.csv", 1) == false)
	//{
	//	// The loading of a map has failed. Return false
	//	return false;
	//}


	//if (cMap2D->LoadMap("Maps/DM2213_Map_Level_03.csv", 2) == false)
	//{
	//	// The loading of a map has failed. Return false
	//	return false;
	//}

	//if (cMap2D->LoadMap("Maps/WinScreen.csv", 3) == false)
	//{
	//	// The loading of a map has failed. Return false
	//	return false;
	//}

	background = new CBackgroundEntity("Image/mapbackground3.png");
	background->SetShader("2DShader");
	background->Init();




	CImageLoader* il = CImageLoader::GetInstance();
	healthbar.fileName = "Image/healthbar.png";
	healthbar.textureID = il->LoadTextureGetID(healthbar.fileName.c_str(), false);
	

	
//	quadMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 5, 5);
	

	//Load Scene 2DColor into ShaderManager
	CShaderManager::GetInstance()->Add("2DColorShader", "Shader//Scene2DColor.vs", "Shader//Scene2DColor.fs");
	CShaderManager::GetInstance()->Use("2DColorShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);
	
	//Create and initialise the cPlayer2D
	cPlayer2D = CPlayer2D::GetInstance();
	//Pass  shader to cPlayer2D
	cPlayer2D->SetShader("2DColorShader");
	//Initialise the instance
	if (cPlayer2D->Init() == false)
	{
		cout << "Failed to load CPlayer2D" << endl;
		return false;
	}
	

	enemyVector.clear();


	while (cMap2D->GetLevel() == 0)
	{
		CEnemy2D* cEnemy2D = new CEnemy2D();
		
		//Pass shader to cEnemy2D
		cEnemy2D->SetShader("2DColorShader");
		//Initialise the instance
		if (cEnemy2D->Init() == true)
		{
			cEnemy2D->SetPlayer2D(cPlayer2D);
			enemyVector.push_back(cEnemy2D);
		}
		else
		{
			//Break out of this loop if the enemy has all been loaded
		//	cEnemy2D = NULL;
			break;
		
		}
	}


	// Setup the shaders
	CShaderManager::GetInstance()->Add("textShader", "Shader//text.vs", "Shader//text.fs");
	CShaderManager::GetInstance()->Use("textShader");

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the CGUI singleton instance here
	cGUI_Scene2D = CGUI_Scene2D::GetInstance();
	cGUI_Scene2D->Init();

	//Game Manager
	cGameManager = CGameManager::GetInstance();
	cGameManager->Init();

	//Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	//cSoundController->Init();

	//cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	//cSoundController->LoadSound(FileSystem::getPath("Sounds\\game_over.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_jump.wav"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\coin_sound.ogg"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_pickup.wav"), 5, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\music_background.ogg"), 6, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\music_victory.ogg"), 7, true);
	//cSoundController->LoadSound(FileSystem::getPath("Sounds\\gamevictory.ogg"), 8, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_die.ogg"), 9, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_nextlevel.wav"), 10, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_time.ogg"), 11, true);
	//Create and initialise the CEnemy2D
	
	return true;
}

/**
@brief Update Update this instance
*/
void CScene2D::Update(const double dElapsedTime)
{
	
	//Call the cPlayer2D's update method
	cPlayer2D->Update(dElapsedTime);

	// Start the Dear ImGui frame

	//Call all the cEnemy2D's update method before Map2D
	// as we want to capture the updates before map2D update
	for (int i = 0; i < enemyVector.size(); i++)
	{
		enemyVector[i]->Update(dElapsedTime);
	}

	for (int i = 0; i < enemyVector2.size(); i++)
	{
		enemyVector2[i]->Update(dElapsedTime);
	}

	for (int i = 0; i < enemyVector3.size(); i++)
	{
		enemyVector3[i]->Update(dElapsedTime);
	}

	// Call the Map2D's update method
	cMap2D->Update(dElapsedTime);



	// Get keyboard updates
	if (cKeyboardController->IsKeyDown(GLFW_KEY_F6))
	{
		// Save the current game to a save file
		// Make sure the file is open
		try {
			if (cMap2D->SaveMap("Maps/DM2213_Map_Level_01_SAVEGAMEtest.csv", cMap2D->GetLevel()) == false)
			{
				throw runtime_error("Unable to save the current game to a file");
			}
		}
		catch (runtime_error e)
		{
			cout << "Runtime error: " << e.what();
			return;
		}
	}

	if (cKeyboardController->IsKeyPressed(GLFW_KEY_1))
	{
		cMap2D->SetLevel(cMap2D->GetLevel() - 1);
	}
	else if (cKeyboardController->IsKeyPressed(GLFW_KEY_2))
	{
		cMap2D->SetLevel(cMap2D->GetLevel() + 1);
	}


	
	//Call the cGUI_Scene2D's update method
	cGUI_Scene2D->Update(dElapsedTime);

	
	if (cGameManager->bPlayerLost == false) {
		cSoundController->PlaySoundByID(6);
	}


	if (cGameManager->bGameToRestart == true)
	{
		cMap2D->GetLevel();
		if (cMap2D->GetLevel() == 0)
		{
			cMap2D->SetLevel(0);
			cMap2D->LoadMap("Maps/DM2213_Map_Level_Test.csv") == true;
			//cMap2D->LoadMap("Maps/DM2213_Map_Level_02.csv", 1) == false;
		}
		cPlayer2D->ResetMap();
		cSoundController->PlaySoundByID(2);
		cGameManager->bGameToRestart = false;
	}

	//Check if the game should go to the next level
	if (cGameManager->bLevelCompleted == true)
	{
		cMap2D->SetLevel(cMap2D->GetLevel() + 1);
		cGameManager->bLevelCompleted = false;
	}

	if (cMap2D->GetLevel() == 4)
	{
		cGameManager->bPlayerWon = true;
		cSoundController->PlaySoundByID(7);
		std::cout << "game won" << std::endl;
	}

	if (cMap2D->GetLevel() == 1)
	{
		for (int i = 0; i < enemyVector.size(); i++)
		{
			delete enemyVector[i];
			enemyVector[i] = NULL;
		}
		enemyVector.clear();


		while (cMap2D->GetLevel() == 1)
		{

			
			CEnemy2D* cEnemy2D = new CEnemy2D();

			//Pass shader to cEnemy2D
			cEnemy2D->SetShader("2DColorShader");
			//Initialise the instance
			if (cEnemy2D->Init() == true)
			{
				cout << "hello" << std::endl;
				cEnemy2D->SetPlayer2D(cPlayer2D);
				enemyVector2.push_back(cEnemy2D);
			}
			else
			{
				//Break out of this loop if the enemy has all been loaded
				break;
			}
		}

	}

	if (cMap2D->GetLevel() == 2)
	{
	
		for (int i = 0; i < enemyVector2.size(); i++)
		{
			delete enemyVector2[i];
			enemyVector2[i] = NULL;
		}
		enemyVector2.clear();

		while (cMap2D->GetLevel() == 2)
		{


			CEnemy3* cEnemy3 = new CEnemy3();

			//Pass shader to cEnemy2D
			cEnemy3->SetShader("2DColorShader");
			//Initialise the instance
			if (cEnemy3->Init() == true)
			{
				cout << "hello" << std::endl;
				cEnemy3->SetPlayer2D(cPlayer2D);
				enemyVector3.push_back(cEnemy3);
			}
			else
			{
				//Break out of this loop if the enemy has all been loaded
				break;
			}
		}
	}

	if (cMap2D->GetLevel() == 2)
	{

		for (int i = 0; i < enemyVector2.size(); i++)
		{
			delete enemyVector2[i];
			enemyVector2[i] = NULL;
		}
		enemyVector2.clear();
	}


	if (cMap2D->GetLevel() == 3)
	{

		for (int i = 0; i < enemyVector3.size(); i++)
		{
			delete enemyVector3[i];
			enemyVector3[i] = NULL;
		}
		enemyVector3.clear();
	}

	//Check if the game has been won by the player
	if (cGameManager->bPlayerWon == true)
	{
		cSoundController->SetVolumeTo0();
		CGameStateManager::GetInstance()->SetActiveGameState("VictoryState");
		cSoundController->PlaySoundByID(7);
	}
	else if (cGameManager->bPlayerLost == true)
	{
		CGameStateManager::GetInstance()->SetActiveGameState("GameOverState");
		for (int i = 0; i < enemyVector.size(); i++)
		{
			delete enemyVector[i];
			enemyVector[i] = NULL;
		}
		enemyVector.clear();

		for (int i = 0; i < enemyVector2.size(); i++)
		{
			delete enemyVector2[i];
			enemyVector2[i] = NULL;
		}
		enemyVector2.clear();

		for (int i = 0; i < enemyVector3.size(); i++)
		{
			delete enemyVector3[i];
			enemyVector3[i] = NULL;
		}
		enemyVector3.clear();

		cMap2D->SetLevel(0);
		
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene2D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer

	/*if (cGameManager->bPlayerWon == false) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}*/

	

	glClear(GL_COLOR_BUFFER_BIT);

	// Enable 2D texture rendering
	glEnable(GL_TEXTURE_2D);
}

/**
 @brief Render Render this instance
 */
void CScene2D::Render(void)
{

	background->Render();
	
	if (cPlayer2D->TimeStop == false) {
	


		//Render the tile=
   //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   //CS: Render the tile
   //quadMesh->Render();


		for (int i = 0; i < enemyVector.size(); i++)
		{
			//Call the CEnemy2D's PreRender()
			enemyVector[i]->PreRender();
			// Call the CEnemy2D's Render()
			enemyVector[i]->Render();
			// Call the CEnemy2D's PostRender()
			enemyVector[i]->PostRender();
		}

		for (int i = 0; i < enemyVector2.size(); i++)
		{
			//Call the CEnemy2D's PreRender()
			enemyVector2[i]->PreRender();
			// Call the CEnemy2D's Render()
			enemyVector2[i]->Render();
			// Call the CEnemy2D's PostRender()
			enemyVector2[i]->PostRender();
		}

		for (int i = 0; i < enemyVector3.size(); i++)
		{
			//Call the CEnemy2D's PreRender()
			enemyVector3[i]->PreRender();
			// Call the CEnemy2D's Render()
			enemyVector3[i]->Render();
			// Call the CEnemy2D's PostRender()
			enemyVector3[i]->PostRender();
		}

		// Call the Map2D's PreRender()
		cMap2D->PreRender();
		// Call the Map2D's Render()
		cMap2D->Render();
		// Call the Map2D's PostRender()
		cMap2D->PostRender();



		// Call the cGUI's PreRender()
		cGUI_Scene2D->PreRender();
		// Call the cGUI's Render()
		cGUI_Scene2D->Render();
		// Call the cGUI's PostRender()
		cGUI_Scene2D->PostRender();



		//Call the cPlayer2D's PreRender()
		cPlayer2D->PreRender();
		//Call the cPlayer2D's Render()
		cPlayer2D->Render();
		//Call the CPlayer2D's PostRender()
		cPlayer2D->PostRender();

	}

		if (cPlayer2D->TimeStop == true)
		{
			

			
			cMap2D->PreRender();
			// Call the Map2D's Render()
			cMap2D->Render();
			// Call the Map2D's PostRender()
			cMap2D->PostRender();
			// Call the Map2D's PreRender()

			// Call the cGUI's PreRender()

			cGUI_Scene2D->PreRender();
			// Call the cGUI's Render()
			cGUI_Scene2D->Render();
			// Call the cGUI's PostRender()
			cGUI_Scene2D->PostRender();

		

			for (int i = 0; i < enemyVector.size(); i++)
			{
				//Call the CEnemy2D's PreRender()
				enemyVector[i]->PreRender();
				// Call the CEnemy2D's Render()
				enemyVector[i]->Render();
				// Call the CEnemy2D's PostRender()
				enemyVector[i]->PostRender();
			}

			//Call the cPlayer2D's PreRender()
			cPlayer2D->PreRender();
			//Call the cPlayer2D's Render()
			cPlayer2D->Render();
			//Call the CPlayer2D's PostRender()
			cPlayer2D->PostRender();

		

			for (int i = 0; i < enemyVector2.size(); i++)
			{
				//Call the CEnemy2D's PreRender()
				enemyVector2[i]->PreRender();
				// Call the CEnemy2D's Render()
				enemyVector2[i]->Render();
				// Call the CEnemy2D's PostRender()
				enemyVector2[i]->PostRender();
			}

			for (int i = 0; i < enemyVector3.size(); i++)
			{
				//Call the CEnemy2D's PreRender()
				enemyVector3[i]->PreRender();
				// Call the CEnemy2D's Render()
				enemyVector3[i]->Render();
				// Call the CEnemy2D's PostRender()
				enemyVector3[i]->PostRender();
			}

		


		}

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene2D::PostRender(void)
{
}