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
	, cClone(NULL)
	, cPortal(NULL)
	, cBlackhole(NULL)
	, cEnemy3P(NULL)
	, cEnemy3D(NULL)
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

	if (cClone)
	{
		cClone->Destroy();
		cClone = NULL;
	}

	if (cPortal)
	{
		cPortal->Destroy();
		cPortal = NULL;
	}

	if (cBlackhole)
	{
		cBlackhole->Destroy();
		cBlackhole = NULL;
	}

	if (cEnemy3P)
	{
		//cEnemy3P->Destroy();
		cEnemy3P = NULL;
	}

	if (cEnemy3D)
	{
		//cEnemy3P->Destroy();
		cEnemy3D = NULL;
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

	for (int i = 0; i < enemyVector4.size(); i++)
	{
		delete enemyVector4[i];
		enemyVector4[i] = NULL;
	}
	enemyVector3.clear();

	for (int i = 0; i < cloneVector.size(); i++)
	{
		delete cloneVector[i];
		cloneVector[i] = NULL;
	}
	cloneVector.clear();
}

bool CScene2D::LaserFireVertical(float row, float col, int blocks)
{
	if (cMap2D->GetMapInfo(row + blocks, col) == 0)
	{
		cMap2D->SetMapInfo(row + blocks, col, 16);
		return false;
	}
	else
	{
		return true;
	}
}

bool CScene2D::LaserFireHorizontal(float row, float col, int blocks)
{
	if (cMap2D->GetMapInfo(row, col + blocks) == 0)
	{
		cMap2D->SetMapInfo(row, col + blocks, 17);
		return false;
	}
	else
	{
		return true;
	}
}

void CScene2D::ResetLaser(int dir, float row, float col)
{
	int i = 0;

	while (true)
	{
		if (dir == 0)
		{
			if (cMap2D->GetMapInfo(row + i, col) == 16 || cMap2D->GetMapInfo(row + i, col) == 0)
			{
				cMap2D->SetMapInfo(row + i, col, 0);
			}
			else
			{
				break;
			}
		}
		else if (dir == 1)
		{
			if (cMap2D->GetMapInfo(row, col + i) == 17 || cMap2D->GetMapInfo(row, col + i) == 0)
			{
				cMap2D->SetMapInfo(row, col + i, 0);
			}
			else
			{
				break;
			}
		}
		i++;
	}
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

#pragma region LoadMaps
	// Load the maps into an array
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
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_Test3.csv", 2) == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_Test4.csv", 3) == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_Test5.csv", 4) == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}
#pragma endregion

	background = new CBackgroundEntity("Image/mapbackground3.png");
	background->SetShader("2DShader");
	background->Init();

	CImageLoader* il = CImageLoader::GetInstance();
	healthbar.fileName = "Image/healthbar.png";
	healthbar.textureID = il->LoadTextureGetID(healthbar.fileName.c_str(), false);

	//quadMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 5, 5);

	//Load Scene 2DColor into ShaderManager
	CShaderManager::GetInstance()->Add("2DColorShader", "Shader//Scene2DColor.vs", "Shader//Scene2DColor.fs");
	CShaderManager::GetInstance()->Use("2DColorShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	//Create and initialise the cPlayer2D
	cPlayer2D = CPlayer2D::GetInstance();
	cClone = CClone::GetInstance();
	cPortal = CPortal::GetInstance();
	cBlackhole = CBlackhole::GetInstance();

	//Pass  shader to cPlayer2D
	cPlayer2D->SetShader("2DColorShader");
	//Initialise the instance
	if (cPlayer2D->Init() == false)
	{
		cout << "Failed to load CPlayer2D" << endl;
		return false;
	}

	cClone->SetShader("2DColorShader");
	//Initialise the instance
	if (cClone->Init() == false)
	{
		cout << "Failed to load cClone" << endl;
		return false;
	}

	cPortal->SetShader("2DColorShader");
	//Initialise the instance
	if (cPortal->Init() == false)
	{
		cout << "Failed to load cPortal" << endl;
		return false;
	}

	cBlackhole->SetShader("2DColorShader");
	//Initialise the instance
	if (cBlackhole->Init() == false)
	{
		cout << "Failed to load cBlackhole" << endl;
		return false;
	}

	enemyVector.clear();
	while (cMap2D->GetLevel() == 0) //Running level 1 on start
	{
		CEnemy2D* cEnemy2D = new CEnemy2D();

		//Pass shader to cEnemy2D
		cEnemy2D->SetShader("2DColorShader");
		//Initialise the instance
		if (cEnemy2D->Init() == true)
		{
			cEnemy2D->SetPlayer2D(cPlayer2D);
			cEnemy2D->SetClone2D(cClone);
			enemyVector.push_back(cEnemy2D);

		}
		else
		{
			//Break out of this loop if the enemy has all been loaded
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

#pragma region LoadSounds
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\robot2.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\game_over.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_jump.wav"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\coin_sound.ogg"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_pickup.wav"), 5, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\music_background.ogg"), 6, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\music_victory.ogg"), 7, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\gamevictory.ogg"), 8, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_die.wav"), 9, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_nextlevel.wav"), 10, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_time.ogg"), 11, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_pickup.ogg"), 12, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\enemy1.ogg"), 13, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_clone.ogg"), 16, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\respawn_clone.ogg"), 17, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\laser.ogg"), 18, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\player_blackhole.ogg"), 19, true);
#pragma endregion

	return true;
}

/**
@brief Update Update this instance
*/
void CScene2D::Update(const double dElapsedTime)
{
	//pausing game = if the game is not paused
	if (CGameStateManager::GetInstance()->GetPauseGameState() == false)
	{
		//Call the cPlayer2D's update method
		cPlayer2D->Update(dElapsedTime);

		if (cPlayer2D->TimeStop != true)
		{
			LaserTimer += dElapsedTime;

			if (isFired == true)
			{
				bulletTime += dElapsedTime;
			}
		}

		////Call the cPlayer2D's update method
		//cPlayer2D->Update(dElapsedTime);

		cClone->Update(dElapsedTime);

		cPortal->Update(dElapsedTime);

		cBlackhole->Update(dElapsedTime);

		// Start the Dear ImGui frame
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

		for (int i = 0; i < enemyVector4.size(); i++)
		{
			enemyVector4[i]->Update(dElapsedTime);
		}

		for (int i = 0; i < cloneVector.size(); i++)
		{
			cloneVector[i]->Update(dElapsedTime);
		}

		for (int i = 0; i < cPlayer2D->bulletVector.size(); ++i)
		{
			cPlayer2D->bulletVector[i]->Update(dElapsedTime);
		}
		if (cMap2D->GetLevel() == 2)
		{
			for (int i = 0; i < cEnemy3P->bulletVector.size(); ++i)
			{
				cEnemy3P->bulletVector[i]->Update(dElapsedTime);
			}
			for (int i = 0; i < cEnemy3D->bulletVector.size(); ++i)
			{
				cEnemy3D->bulletVector[i]->Update(dElapsedTime);
			}
		}

		// Call the Map2D's update method
		cMap2D->Update(dElapsedTime);
	}

	//Call the cGUI_Scene2D's update method
	cGUI_Scene2D->Update(dElapsedTime);

	// Get keyboard updates
	/*
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
	*/

	//Debugging
	if (cKeyboardController->IsKeyPressed(GLFW_KEY_1))
	{
		cMap2D->SetLevel(cMap2D->GetLevel() - 1);
	}
	else if (cKeyboardController->IsKeyPressed(GLFW_KEY_2))
	{
		cMap2D->SetLevel(cMap2D->GetLevel() + 1);
	}

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
		}
		cPlayer2D->ResetMap();
		cSoundController->PlaySoundByID(9);
		cGameManager->bGameToRestart = false;
	}

	//Check if the game should go to the next level
	if (cGameManager->bLevelCompleted == true)
	{
		cMap2D->SetLevel(cMap2D->GetLevel() + 1);
		cPortal->respawnPoint = false;
		cPortal->renderPortal = false;
		cPlayer2D->clone = false;
		cPlayer2D->canUseClone = true;
		cGameManager->bLevelCompleted = false;
	}

	if (cMap2D->GetLevel() == 1) //Level 2
	{
		for (int i = 0; i < enemyVector.size(); i++)
		{
			delete enemyVector[i];
			enemyVector[i] = NULL;
		}
		enemyVector.clear();

		while (true) 
		{
			Monster2D* cMonster2D = new Monster2D();

			//Pass shader to cEnemy2D
			cMonster2D->SetShader("2DColorShader");
			//Initialise the instance
			if (cMonster2D->Init() == true)
			{
				//cout << "hello" << std::endl;
				cMonster2D->SetPlayer2D(cPlayer2D);
				cMonster2D->SetClone2D(cClone);
				enemyVector2.push_back(cMonster2D);
			}
			else
			{
				//Break out of this loop if the enemy has all been loaded
				break;
			}
		}
	}

	if (cMap2D->GetLevel() == 2) //Level 3
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
				if (cEnemy3P == NULL)
				{
					cEnemy3P = cEnemy3;
					cout << "Enemy3P Pointer going" << endl;
				}
				else if (cEnemy3D == NULL)
				{
					cEnemy3D = cEnemy3;
					cout << "Enemy3D Pointer going" << endl;
				}
				cEnemy3->SetPlayer2D(cPlayer2D);
				enemyVector3.push_back(cEnemy3);
			}
			else
			{
				//Break out of this loop if the enemy has all been loaded
				break;
			}
		}

		if (LaserTimer >= 5.f)
		{
			//Vertical
			ResetLaser(0, 20, 13);
			ResetLaser(0, 20, 16);
			ResetLaser(0, 20, 19);
			ResetLaser(0, 4, 13);
			ResetLaser(0, 4, 16);
			ResetLaser(0, 4, 19);
			//Horizontal
			ResetLaser(1, 20, 2);
			ResetLaser(1, 16, 2);
			ResetLaser(1, 12, 2);
			ResetLaser(1, 8, 2);
			LaserTimer = 0;
			blocks_0 = 0;
			blocks_1 = 0;
		}
		else if (LaserTimer >= 3.0f)
		{
			if (LaserFireVertical(21, 13, blocks_0) == false)
			{
				cSoundController->PlaySoundByID(18);
				LaserFireVertical(21, 13, blocks_0);
				LaserFireVertical(21, 16, blocks_0);
				LaserFireVertical(21, 19, blocks_0);
				LaserFireVertical(5, 13, blocks_0);
				LaserFireVertical(5, 16, blocks_0);
				LaserFireVertical(5, 19, blocks_0);
				if (cPlayer2D->TimeStop != true)
					blocks_0--;
			}

			if (LaserFireHorizontal(20, 2, blocks_1) == false)
			{
				cSoundController->PlaySoundByID(18);
				LaserFireHorizontal(20, 2, blocks_1);
				LaserFireHorizontal(16, 2, blocks_1);
				LaserFireHorizontal(12, 2, blocks_1);
				LaserFireHorizontal(8, 2, blocks_1);
				if (cPlayer2D->TimeStop != true)
					blocks_1++;
			}
		}
	}


	if (cMap2D->GetLevel() == 3) //Level 4
	{

		for (int i = 0; i < enemyVector3.size(); i++)
		{
			delete enemyVector3[i];
			enemyVector3[i] = NULL;
		}
		enemyVector3.clear();

		while (true)
		{
			Monster2D* cEnemy4 = new Monster2D();

			//Pass shader to cEnemy2D
			cEnemy4->SetShader("2DColorShader");
			//Initialise the instance
			if (cEnemy4->Init() == true)
			{
				//cout << "hello" << std::endl;
				cEnemy4->SetPlayer2D(cPlayer2D);
				cEnemy4->SetClone2D(cClone);
				enemyVector4.push_back(cEnemy4);
			}
			else
			{
				//Break out of this loop if the enemy has all been loaded
				break;
			}
		}

		if (CGameStateManager::GetInstance()->GetPauseGameState() == false)
		{
			if (LaserTimer >= 5.f)
			{
				//Vertical
				ResetLaser(0, 18, 5);
				ResetLaser(0, 18, 7);
				ResetLaser(0, 18, 9);
				ResetLaser(0, 18, 11);
				//Horizontal
				for (int i = 0; i < 13; ++i)
				{
					ResetLaser(1, 5 + i, 28);
				}

				LaserTimer = 0;
				blocks_0 = 0;
				blocks_1 = 0;
			}
			else if (LaserTimer >= 3.0f)
			{
				if (!LaserFireVertical(21, 5, blocks_0))
				{
					cSoundController->PlaySoundByID(18);
					LaserFireVertical(21, 5, blocks_0);
					LaserFireVertical(21, 7, blocks_0);
					LaserFireVertical(21, 9, blocks_0);
					LaserFireVertical(21, 11, blocks_0);
					if (cPlayer2D->TimeStop != true)
						blocks_0--;
				}

				if (LaserFireHorizontal(5, 29, blocks_1) == false)
				{
					cSoundController->PlaySoundByID(18);
					for (int i = 0; i < 13; ++i)
					{
						LaserFireHorizontal(5 + i, 29, blocks_1);
					}
					if (cPlayer2D->TimeStop != true)
						blocks_1--;
				}
			}
		}
	}

	if (cMap2D->GetLevel() == 4) //Level 5
	{

		//for (int i = 0; i < enemyVector3.size(); i++)
		//{
		//	delete enemyVector3[i];
		//	enemyVector3[i] = NULL;
		//}
		//enemyVector3.clear();


		//while (true)
		//{
		//	CEnemy3* cEnemy3 = new CEnemy3();

		//	//Pass shader to cEnemy2D
		//	cEnemy3->SetShader("2DColorShader");
		//	//Initialise the instance
		//	if (cEnemy3->Init() == true)
		//	{
		//		//cout << "hello" << std::endl;
		//		cEnemy3->SetPlayer2D(cPlayer2D);
		//		cEnemy3->SetClone2D(cClone);
		//		enemyVector3.push_back(cEnemy3);
		//	}
		//	else
		//	{
		//		//Break out of this loop if the enemy has all been loaded
		//		break;
		//	}
		//}

		if (CGameStateManager::GetInstance()->GetPauseGameState() == false)
		{
			if (LaserTimer >= 5.f)
			{
				//Vertical
				ResetLaser(0, 9, 26);
				ResetLaser(0, 9, 27);
				ResetLaser(0, 9, 28);
				ResetLaser(0, 2, 20);
				//Horizontal
				/*for (int i = 0; i < 13; ++i)
				{
					ResetLaser(1, 5 + i, 28);
				}*/
				ResetLaser(1, 19, 12);
				ResetLaser(1, 20, 12);
				ResetLaser(1, 10, 2);

				LaserTimer = 0;
				blocks_0 = 0;
				blocks_1 = 0;
			}
			else if (LaserTimer >= 3.0f)
			{
				if (!LaserFireVertical(21, 26, blocks_0))
				{
					cSoundController->PlaySoundByID(18);
					LaserFireVertical(21, 26, blocks_0);
					LaserFireVertical(21, 27, blocks_0);
					LaserFireVertical(21, 28, blocks_0);
					LaserFireVertical(14, 20, blocks_0);
					if (cPlayer2D->TimeStop != true)
						blocks_0--;
				}

				if (LaserFireHorizontal(19, 12, blocks_1) == false)
				{
					cSoundController->PlaySoundByID(18);
					LaserFireHorizontal(19, 12, blocks_1);
					LaserFireHorizontal(20, 12, blocks_1);
					LaserFireHorizontal(10, 2, blocks_1);
					if (cPlayer2D->TimeStop != true)
						blocks_1++;
				}
			}
		}
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

		for (int i = 0; i < cloneVector.size(); i++)
		{
			delete cloneVector[i];
			cloneVector[i] = NULL;
		}
		cloneVector.clear();

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

	for (int i = 0; i < cloneVector.size(); i++)
	{
		//Call the CEnemy2D's PreRender()
		cloneVector[i]->PreRender();
		// Call the CEnemy2D's Render()
		cloneVector[i]->Render();
		// Call the CEnemy2D's PostRender()
		cloneVector[i]->PostRender();
	}

	if (cPlayer2D->TimeStop == false) {
		//Render the tile=
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//CS: Render the tile
		 //quadMesh->Render();

		for (int i = 0; i < cPlayer2D->bulletVector.size(); ++i)
		{
			isFired = true;

			//Call the CEnemy2D's PreRender()
			cPlayer2D->bulletVector[i]->PreRender();
			// Call the CEnemy2D's Render()
			cPlayer2D->bulletVector[i]->Render();
			// Call the CEnemy2D's PostRender()
			cPlayer2D->bulletVector[i]->PostRender();

			if (bulletTime > 0.75f)
			{
				for (int i = 0; i < cPlayer2D->bulletVector.size(); i++)
				{
					delete cPlayer2D->bulletVector[i];
					cPlayer2D->bulletVector[i] = NULL;
				}

				bulletTime = 0;
				isFired = false;
				cPlayer2D->bulletVector.clear();
			}
		}

		if (cMap2D->GetLevel() == 2)
		{
			for (int i = 0; i < cEnemy3P->bulletVector.size(); ++i)
			{
				isFired = true;

				//Call the CEnemy2D's PreRender()
				cEnemy3P->bulletVector[i]->PreRender();
				// Call the CEnemy2D's Render()
				cEnemy3P->bulletVector[i]->Render();
				// Call the CEnemy2D's PostRender()
				cEnemy3P->bulletVector[i]->PostRender();

				if (bulletTime > 0.75f)
				{
					for (int i = 0; i < cEnemy3P->bulletVector.size(); i++)
					{
						delete cEnemy3P->bulletVector[i];
						cEnemy3P->bulletVector[i] = NULL;
					}

					bulletTime = 0;
					isFired = false;
					cEnemy3P->bulletVector.clear();
				}
			}
			for (int i = 0; i < cEnemy3D->bulletVector.size(); ++i)
			{
				isFired = true;

				//Call the CEnemy2D's PreRender()
				cEnemy3D->bulletVector[i]->PreRender();
				// Call the CEnemy2D's Render()
				cEnemy3D->bulletVector[i]->Render();
				// Call the CEnemy2D's PostRender()
				cEnemy3D->bulletVector[i]->PostRender();

				if (bulletTime > 0.75f)
				{
					for (int i = 0; i < cEnemy3D->bulletVector.size(); i++)
					{
						delete cEnemy3D->bulletVector[i];
						cEnemy3D->bulletVector[i] = NULL;
					}

					bulletTime = 0;
					isFired = false;
					cEnemy3D->bulletVector.clear();
				}
			}
		}


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

		for (int i = 0; i < enemyVector4.size(); i++)
		{
			//Call the CEnemy2D's PreRender()
			enemyVector4[i]->PreRender();
			// Call the CEnemy2D's Render()
			enemyVector4[i]->Render();
			// Call the CEnemy2D's PostRender()
			enemyVector4[i]->PostRender();
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

		if (cPlayer2D->useUltimate == true) {
			cBlackhole->PreRender();
			//Call the cPlayer2D's Render()
			cBlackhole->Render();
			//Call the CPlayer2D's PostRender()
			cBlackhole->PostRender();
		}

		if (cPortal->renderPortal == true)
		{

			cPortal->PreRender();
			//Call the cPlayer2D's Render()
			cPortal->Render();
			//Call the CPlayer2D's PostRender()
			cPortal->PostRender();
		}

		cClone->PreRender();
		//Call the cPlayer2D's Render()
		cClone->Render();
		//Call the CPlayer2D's PostRender()
		cClone->PostRender();

		//Call the cPlayer2D's PreRender()
		cPlayer2D->PreRender();
		//Call the cPlayer2D's Render()
		cPlayer2D->Render();
		//Call the CPlayer2D's PostRender()
		cPlayer2D->PostRender();
	}

	if (cPlayer2D->TimeStop == true)
	{

		// Call the cGUI's PreRender()
		cMap2D->PreRender();
		// Call the Map2D's Render()
		cMap2D->Render();
		// Call the Map2D's PostRender()
		cMap2D->PostRender();
		// Call the Map2D's PreRender()

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

		cBlackhole->PreRender();
		//Call the cPlayer2D's Render()
		cBlackhole->Render();
		//Call the CPlayer2D's PostRender()
		cBlackhole->PostRender();

		cPortal->PreRender();
		//Call the cPlayer2D's Render()
		cPortal->Render();
		//Call the CPlayer2D's PostRender()
		cPortal->PostRender();

		cClone->PreRender();
		//Call the cPlayer2D's Render()
		cClone->Render();
		//Call the CPlayer2D's PostRender()
		cClone->PostRender();

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