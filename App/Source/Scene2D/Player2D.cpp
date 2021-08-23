/**
 Player2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Player2D.h"

#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include "Monster2D.h"
#include "Scene2D.h"


/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::CPlayer2D(void)
	: cMap2D(NULL)
	, cKeyboardController(NULL)
	, animatedSprites(NULL)
	//, cClone(NULL)
	, playerColour(glm::vec4(1.0f))
	, cSoundController(NULL)
{
	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	i32vec2Index = glm::i32vec2(0);

	// Initialise vecNumMicroSteps
	i32vec2NumMicroSteps = glm::i32vec2(0);

	// Initialise vec2UVCoordinate
	vec2UVCoordinate = glm::vec2(0.0f);
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::~CPlayer2D(void)
{

	//Delete the CAnimationSprites
	if (animatedSprites)
	{
		delete animatedSprites;
		animatedSprites = NULL;
	}
	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	// We won't delete this since it was created elsewhere
	cMap2D = NULL;


	// We won't delete this since it was created elsewhere
	//cClone = NULL;

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

/**
  @brief Initialise this instance
  */
bool CPlayer2D::Init(void)
{

	cooldownTimer = 0;
	freezeDuration = 0;
	switchesActivated = 0;
	

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	// Reset all keys since we are starting a new game
	cKeyboardController->Reset();

	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();


	cGameManager = CGameManager::GetInstance();

	//cClone = CClone::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();

	cClone = CClone::GetInstance();

	cPortal = CPortal::GetInstance();

	cSoundController = CSoundController::GetInstance();


	cInventoryManager = CInventoryManager::GetInstance();
	//Add a Lives icon as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Lives", "Image/Scene2D_Lives.tga", 3, 3);
	cInventoryItem->vec2Size = glm::vec2(25, 25);
	//cInventoryItem = cInventoryManager->GetItem("Lives");
	//cInventoryItem->Add(3);

	//Add a Health icon as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Health", "Image/healthicon.png", 100, 100);
	cInventoryItem->vec2Size = glm::vec2(35, 35);

	cInventoryItem = cInventoryManager->Add("Stamina", "Image/stamina.png", 100, 100);
	cInventoryItem->vec2Size = glm::vec2(35, 35);

	cInventoryItem = cInventoryManager->Add("Ultimate", "Image/powerup.tga", 100, 0);
	cInventoryItem->vec2Size = glm::vec2(35, 35);

	cInventoryItem = cInventoryManager->Add("TimestopTimer", "Image/time_powerup.png", 6, 0);
	cInventoryItem->vec2Size = glm::vec2(35, 35);

	cInventoryItem = cInventoryManager->Add("ClonePowerup", "Image/clone_powerup.png", 6, 0);
	cInventoryItem->vec2Size = glm::vec2(35, 35);

	cInventoryItem = cInventoryManager->Add("background", "Image/dimension2.png", 0, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	cInventoryItem = cInventoryManager->Add("border", "Image/dimensionborder.png", 0, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	cInventoryItem = cInventoryManager->Add("dimensiontext", "Image/dimensiontext.png", 0, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	cooldownTimer = 0;
	freezeDuration = 0;
	cloneDuration = 0;
	switchesActivated = 0;
	fallTimer = 3;
	liftTimer = 2;
	
	// Find the indices for the player in arrMapInfo, and assign it to cPlayer2D
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	if (cMap2D->FindValue(200, uiRow, uiCol) == false)
		return false;	// Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set the start position of the Player to iRow and iCol
	i32vec2Index = glm::i32vec2(uiCol, uiRow);
	// By default, microsteps should be zero
	i32vec2NumMicroSteps = glm::i32vec2(0, 0);

	/*cInventoryItem = cInventoryManager->GetItem("Lives");
	cInventoryItem->Add(1);*/
	

	//CS: Create the Quad Mesh using the mesh builder
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	quadMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);


	// Load the player texture
	/*if (LoadTexture("Image/Scene2D_PlayerTile.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture" << std::endl;
		return false;
	}*/

	if (LoadTexture("Image/player3.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture" << std::endl;
		return false;
	}

	jumppoweractive = false;
	speedboost = false;
	powerupActive = false;
	TimeStop = false;
	canUsepower = true;
	canUseClone = true;
	clone = false;
	respawn = true;
	


	//CS: Create the animated sprite and setup the animation
	//animatedSprites = CMeshBuilder::GenerateSpriteAnimation(4, 6,
		//0.1, 0.13);

	animatedSprites = CMeshBuilder::GenerateSpriteAnimation(4, 6,
		cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);

	animatedSprites->AddAnimation("idle", 0, 5);
	animatedSprites->AddAnimation("right", 6, 11);
	animatedSprites->AddAnimation("left", 12, 17);
	animatedSprites->AddAnimation("jump", 18, 20);
	//CS: Play the "idle animation as default
	animatedSprites->PlayAnimation("idle", -1, 1.0f);

	playerColour = glm::vec4(1.0, 1.0, 1.0, 1.0);


	powerupActive = false;
	speedboost = false;
	pitfallReset = false;


	//Set the physics to fall status by default
	cPhysics2D.Init();
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
}

/**
 @brief Update this instance
 */
void CPlayer2D::Update(const double dElapsedTime)
{
	// Store the old position
	i32vec2OldIndex = i32vec2Index;



	if (i32vec2Index.y <= 1 && pitfallReset == false) //&& (cInventoryItem->GetCount() > 0))
	{
		pitfallReset = true;
		CGameManager::GetInstance()->bGameToRestart = true;
		cInventoryItem = cInventoryManager->GetItem("Lives");
		cInventoryItem->Remove(1);
		//cSoundController->PlaySoundByID(9);
	}


	if (clone == true)
	{
		cloneDuration += dElapsedTime;
	}

	if (cloneDuration >= 5)
	{
		clone = false;
		cloneDuration = 0;
		canUseClone = true;
		cPortal->renderPortal = true;

	}




	
	if (TimeStop == true)
	{
		freezeDuration += dElapsedTime;
		if (freezeDuration >= 5)
		{
			TimeStop = false;
			freezeDuration = 0;
			canUsepower = false;
		}
	}

	if (canUsepower == false)
	{
		cooldownTimer += dElapsedTime;
	}
	

	if (cooldownTimer >= 6)
	{
		cooldownTimer = 0;
		canUsepower = true;

	}

	
	playerSprinting = false;


	cInventoryItem = cInventoryManager->GetItem("Stamina");
	if (!playerSprinting)
	{
		cInventoryItem->Add(0.1);
	}

	if (cKeyboardController->IsKeyDown(GLFW_KEY_LEFT_SHIFT) && cKeyboardController->IsKeyDown(GLFW_KEY_LEFT) && cInventoryItem->GetCount() > 0)
	{
		playerSprinting = true;
		cInventoryItem = cInventoryManager->GetItem("Stamina");
		cInventoryItem->Remove(1);

		if (i32vec2Index.x >= 0)
		{
			i32vec2NumMicroSteps.x -= 2.5;
			if (i32vec2NumMicroSteps.x < 0)
			{
				i32vec2NumMicroSteps.x = ((int)cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
				i32vec2Index.x--;
			}
		}

		Constraint(LEFT);

		
		
		if (CheckPosition(LEFT) == false)
		{

			i32vec2NumMicroSteps.x = 0;
			i32vec2Index = i32vec2OldIndex;
		}

		if (IsMidAir() == true)
		{
			if (cPhysics2D.GetStatus() != CPhysics2D::STATUS::JUMP) {
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
			}
		}

		//CS: Play the "left" animation
		animatedSprites->PlayAnimation("left", -1, 1.0f);
	}

	// Get keyboard updates
	else if (cKeyboardController->IsKeyDown(GLFW_KEY_LEFT))
	{
		// Calculate the new position to the left
		if (i32vec2Index.x >= 0)
		{
			i32vec2NumMicroSteps.x--;
			if (i32vec2NumMicroSteps.x < 0)
			{
				i32vec2NumMicroSteps.x = ((int)cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
				i32vec2Index.x--;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(LEFT);

		// If the new position is not feasible, then revert to old position

		if (CheckPosition(LEFT) == false)
		{
				
			i32vec2NumMicroSteps.x = 0;
			i32vec2Index = i32vec2OldIndex;
		}
		

		// Check if player is in mid-air, such as walking off a platform
		if (IsMidAir() == true)
		{
			if (cPhysics2D.GetStatus() != CPhysics2D::STATUS::JUMP) {
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
			}
		}

		//CS: Play the "left" animation
		animatedSprites->PlayAnimation("left", -1, 1.0f);
		

		//CS: Change Color
		//playerColour = glm::vec4(1.0, 0.0, 0.0, 1.0);
	}


	else if (cKeyboardController->IsKeyDown(GLFW_KEY_LEFT_SHIFT) && cKeyboardController->IsKeyDown(GLFW_KEY_RIGHT) && cInventoryItem->GetCount() > 0)
	{
		playerSprinting = true;
		cInventoryItem = cInventoryManager->GetItem("Stamina");
		cInventoryItem->Remove(1);

		if (i32vec2Index.x < (int)cSettings->NUM_TILES_XAXIS)
		{
			i32vec2NumMicroSteps.x += 2;

			if (i32vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
			{
				i32vec2NumMicroSteps.x  = 0;

				i32vec2Index.x++;
			}
		}

		Constraint(RIGHT);


		if (CheckPosition(RIGHT) == false)
		{
			i32vec2NumMicroSteps.x = 0;
		}

		if (IsMidAir() == true)
		{
			if (cPhysics2D.GetStatus() != CPhysics2D::STATUS::JUMP) {
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
			}
		}

		//CS: Play the "left" animation
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	}


	else if (cKeyboardController->IsKeyDown(GLFW_KEY_RIGHT))
	{
		// Calculate the new position to the right
		if (i32vec2Index.x < (int)cSettings->NUM_TILES_XAXIS)
		{
			i32vec2NumMicroSteps.x++;

			if (i32vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
			{
				i32vec2NumMicroSteps.x = 0;
				
				i32vec2Index.x ++;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(RIGHT);

		if (speedboost == true) {

			i32vec2NumMicroSteps.x += 1;


		}


		// If the new position is not feasible, then revert to old position

		if (CheckPosition(RIGHT) == false)
		{
			i32vec2NumMicroSteps.x = 0;
		}
		
		

		// Check if player is in mid-air, such as walking off a platform
		if (IsMidAir() == true)
		{
			if (cPhysics2D.GetStatus() != CPhysics2D::STATUS::JUMP) {
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
			}
		}
	

		//CS: Play the "right" animation
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	

		//CS: Change Color
		//playerColour = glm::vec4(1.0, 1.0, 0.0, 1.0);
	}

	
	
	else if (cKeyboardController->IsKeyDown(GLFW_KEY_S))
	{
		// Calculate the new position down
		if (i32vec2Index.y >= 0)
		{
			i32vec2NumMicroSteps.y--;
			if (i32vec2NumMicroSteps.y < 0)
			{
				i32vec2NumMicroSteps.y = ((int)cSettings->NUM_STEPS_PER_TILE_YAXIS) - 1;
				i32vec2Index.y--;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(DOWN);


		// If the new position is not feasible, then revert to old position
		if (CheckPosition(DOWN) == false)
		{
			i32vec2Index = i32vec2OldIndex;
			i32vec2NumMicroSteps.y = 0;
		}

		//CS: Play the "idle" animation
		animatedSprites->PlayAnimation("idle", -1, 1.0f);
		

		//CS: Change Color
		//playerColour = glm::vec4(1.0, 0.0, 1.0, 0.5);
	}
	if (cKeyboardController->IsKeyDown(GLFW_KEY_SPACE))
	{
		if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::JUMP);
			cPhysics2D.SetInitialVelocity(glm::vec2(0.0f, 3.5f));
			//Play a  sound for jump
			cSoundController->PlaySoundByID(3);
			animatedSprites->PlayAnimation("jump", -1, 1.0f);
		

			if (jumppoweractive == true)
			{
				cPhysics2D.SetInitialVelocity(glm::vec2(0.0f, 10.0f));
			}
		}
	}

	if (cKeyboardController->IsKeyDown(GLFW_KEY_T))
	{
		
		cInventoryItem = cInventoryManager->GetItem("TimestopTimer");
	
		if (canUsepower == true && cInventoryItem->GetCount() > 0)
		{
			cSoundController->PlaySoundByID(11);
			TimeStop = true;
			cInventoryItem->Remove(1);

		}
		//cooldownTimer += dElapsedTime
	}


	if (cKeyboardController->IsKeyReleased(GLFW_KEY_C))
	{
		cInventoryItem = cInventoryManager->GetItem("ClonePowerup");

		if (canUseClone == true && cInventoryItem->GetCount() > 0)
		{
			cSoundController->PlaySoundByID(16);
			clone = true;
			canUseClone = false;
			cPortal->respawnPoint = false;
			cPortal->renderPortal = false;
			cInventoryItem->Remove(1);

		}
	}
	


	if (cKeyboardController->IsKeyDown(GLFW_KEY_4) && isRemote) //Teleporting
	{
		if (cMap2D->GetLevel() == 2)
		{
			i32vec2Index.x = 30;
			i32vec2Index.y = 21;
			i32vec2NumMicroSteps.x = 0;
		}
	}


	

	if (IsMidAir() == true)
	{
		if (cPhysics2D.GetStatus() != CPhysics2D::STATUS::JUMP) {
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}
	}
		
	
	// Update Jump or Fall
	//CS: Will cause error when debugging. Set to default elapsed time
	UpdateJumpFall(dElapsedTime);


	// Interact with the Map
	InteractWithMap();

	UpdateHealthLives();

	dimension();

	//update platform
	IsPlaformStepped(dElapsedTime);

	//lift controls
	IsLiftMoving(dElapsedTime);
	IsLiftSwitchStepped();

	//CS: Update the animated sprite
	animatedSprites->Update(dElapsedTime);

	// Update the UV Coordinates
	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, i32vec2Index.x, false, i32vec2NumMicroSteps.x * cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, i32vec2Index.y, false, i32vec2NumMicroSteps.y * cSettings->MICRO_STEP_YAXIS);

}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CPlayer2D::PreRender(void)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}
/**
 @brief Render this instance
 */
void CPlayer2D::Render(void)
{
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	unsigned int colorLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "runtime_color");

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(vec2UVCoordinate.x,
													vec2UVCoordinate.y,
													0.0f));
	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(colorLoc, 1, glm::value_ptr(playerColour));

	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	glBindVertexArray(VAO);
	// Render the tile
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//CS: Render the tile
	//quadMesh->Render();
	animatedSprites->Render();

	glBindVertexArray(0);

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

/**
@brief Load a texture, assign it a code and store it in MapOfTextureIDs.
@param filename A const char* variable which contains the file name of the texture
*/
bool CPlayer2D::LoadTexture(const char* filename, GLuint& iTextureID)
{
	// Variables used in loading the texture
	int width, height, nrChannels;
	
	// texture 1
	// ---------
	glGenTextures(1, &iTextureID);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CImageLoader* cImageLoader = CImageLoader::GetInstance();
	unsigned char *data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}
	// Free up the memory of the file data read in
	free(data);

	return true;
}

/**
 @brief Constraint the player's position within a boundary
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void CPlayer2D::Constraint(DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		if (i32vec2Index.x < 0)
		{
			i32vec2Index.x = 0;
			i32vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == RIGHT)
	{
		if (i32vec2Index.x >= ((int)cSettings->NUM_TILES_XAXIS) - 1)
		{
			i32vec2Index.x = ((int)cSettings->NUM_TILES_XAXIS) - 1;
			i32vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == UP)
	{
		if (i32vec2Index.y >= ((int)cSettings->NUM_TILES_YAXIS) - 1)
		{
			i32vec2Index.y = ((int)cSettings->NUM_TILES_YAXIS) - 1;
			i32vec2NumMicroSteps.y = 0;
		}
	}
	else if (eDirection == DOWN)
	{
		if (i32vec2Index.y < 0)
		{
			i32vec2Index.y = 0;
			i32vec2NumMicroSteps.y = 0;
		
		}
	}
	else

	{
		cout << "CPlayer2D::Constraint: Unknown direction." << endl;
	}
}

/**
 @brief Check if a position is possible to move into
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
bool CPlayer2D::CheckPosition(DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		// If the new position is fully within a row, then check this row only
		if (i32vec2NumMicroSteps.y == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 rows, then check both rows as well
		else if (i32vec2NumMicroSteps.y != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) >= 100) ||
				(cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) >= 100))
			{
				return false;
			}
		}
	}
	else if (eDirection == RIGHT)
	{
		// If the new position is at the top row, then return true
		if (i32vec2Index.x >= cSettings->NUM_TILES_XAXIS - 1)
		{
			i32vec2NumMicroSteps.x = 0;
			return true;
		}

		// If the new position is fully within a row, then check this row only
		if (i32vec2NumMicroSteps.y == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 rows, then check both rows as well
		else if (i32vec2NumMicroSteps.y != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) >= 100) ||
				(cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) >= 100))
			{
				return false;
			}
		}

	}
	else if (eDirection == UP)
	{
		// If the new position is at the top row, then return true
		if (i32vec2Index.y >= cSettings->NUM_TILES_YAXIS - 1)
		{
			i32vec2NumMicroSteps.y = 0;
			return true;
		}

		// If the new position is fully within a column, then check this column only
		if (i32vec2NumMicroSteps.x == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 columns, then check both columns as well
		else if (i32vec2NumMicroSteps.x != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) >= 100) ||
				(cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) >= 100))
			{
				return false;
			}
		}
	}
	else if (eDirection == DOWN)
	{
		// If the new position is fully within a column, then check this column only
		if (i32vec2NumMicroSteps.x == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 columns, then check both columns as well
		else if (i32vec2NumMicroSteps.x != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) >= 100) ||
				(cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) >= 100))
			{
				return false;
			}
		}
	}
	else
	{
		cout << "CPlayer2D::CheckPosition: Unknown direction." << endl;
	}

	return true;
}

bool CPlayer2D::IsMidAir(void)
{
	//if the player is at the bottom row, then he is not in mid air for sure
	if (i32vec2Index.y == 0)
		return false;
	//Check if the tile below the player's current position is empty
	if ((i32vec2NumMicroSteps.x == 0) &&
		(cMap2D->GetMapInfo(i32vec2Index.y - 1, i32vec2Index.x) < 100))
	{
		return true;
	}
	
	return false;
}

void CPlayer2D::IsLiftMoving(double dt)
{
	if (cMap2D->GetMapInfo(i32vec2Index.y - 1, i32vec2Index.x) == 140)
	{
		liftTimer -= dt; //countdown timer
		if (cMap2D->GetMapInfo(i32vec2Index.y - 1, i32vec2Index.x + 1) >= 100)
		{
			if (liftTimer <= 1)
			{
				i32vec2Index.y++;
				cMap2D->SetMapInfo(i32vec2Index.y - 1, i32vec2Index.x, 140); //replace air with lift block
				cMap2D->SetMapInfo(i32vec2Index.y - 2, i32vec2Index.x, 0); //delete prev block
			}
			if (liftTimer <= 0)
			{
				liftTimer = 2;
			}
		}
		else
		{
			cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
			cMap2D->SetMapInfo(i32vec2Index.y - 1, i32vec2Index.x, 100);
		}
	}
	//return false;
}

bool CPlayer2D::IsLiftSwitchStepped(void)
{
	//for how high the lift goes
	for (int i = 1; i < 8; i++)
	{
		//if pressure plate activated reset lift
		if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) == 13)
		{
			//if the right side of player has lift or changedblock, rsets lift block and airblocks
			if ((cMap2D->GetMapInfo(i32vec2Index.y + i, i32vec2Index.x + 1) == 100) || (cMap2D->GetMapInfo(i32vec2Index.y + i, i32vec2Index.x + 1) == 140))
			{
				cMap2D->SetMapInfo(i32vec2Index.y + i, i32vec2Index.x + 1, 0);
				cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x + 1, 140);
			}
		}
	}
	return false;
}

bool CPlayer2D::dimension(void)
{
	dimensionchange();

	
	return false;
}

void CPlayer2D::dimensionchange()
{
	////to change into dimension mode
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	//if (TimeStop == true)
	if (TimeStop == true)
	{
		while (cMap2D->FindValue(100, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 120);
		}
		while (cMap2D->FindValue(101, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 121);
		}
		while (cMap2D->FindValue(102, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 122);
		}
		while (cMap2D->FindValue(103, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 123);
		}
		while (cMap2D->FindValue(104, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 124);
		}
		while (cMap2D->FindValue(105, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 125);
		}
		while (cMap2D->FindValue(106, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 126);
		}
		while (cMap2D->FindValue(107, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 127); //changes tile to dimensiontile
		}
		while (cMap2D->FindValue(108, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 128);
		}
		while (cMap2D->FindValue(109, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 129);
		}
		while (cMap2D->FindValue(110, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 130);
		}
		while (cMap2D->FindValue(111, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 131);
		}
		while (cMap2D->FindValue(112, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 132);
		}
		while (cMap2D->FindValue(113, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 133);
		}
		while (cMap2D->FindValue(114, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 134);
		}
		while (cMap2D->FindValue(115, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 135);
		}
		while (cMap2D->FindValue(116, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 136);
		}
		while (cMap2D->FindValue(117, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 137);
		}
		while (cMap2D->FindValue(118, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 138);
		}
		while (cMap2D->FindValue(119, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 139);
		}
	}
	if (TimeStop ==false)
	{
		while (cMap2D->FindValue(120, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 100);
		}
		while (cMap2D->FindValue(121, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 101);
		}
		while (cMap2D->FindValue(122, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 102);
		}
		while (cMap2D->FindValue(123, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 103);
		}
		while (cMap2D->FindValue(124, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 104);
		}
		while (cMap2D->FindValue(125, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 105);
		}
		while (cMap2D->FindValue(126, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 106);
		}
		while (cMap2D->FindValue(127, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 107); //changes tile to dimensiontile
		}
		while (cMap2D->FindValue(128, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 108);
		}
		while (cMap2D->FindValue(129, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 109);
		}
		while (cMap2D->FindValue(130, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 110);
		}
		while (cMap2D->FindValue(131, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 111);
		}
		while (cMap2D->FindValue(132, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 112);
		}
		while (cMap2D->FindValue(133, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 113);
		}
		while (cMap2D->FindValue(134, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 114);
		}
		while (cMap2D->FindValue(135, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 115);
		}
		while (cMap2D->FindValue(136, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 116);
		}
		while (cMap2D->FindValue(137, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 117);
		}
		while (cMap2D->FindValue(138, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 118);
		}
		while (cMap2D->FindValue(139, uiRow, uiCol) == true)
		{
			cMap2D->SetMapInfo(uiRow, uiCol, 119);
		}
	}

}

bool CPlayer2D::ResetMap()
{

	pitfallReset = false;
	clone == false;

	unsigned int uiRow = -1;
	unsigned int uiCol = -1;

	if (cPortal->respawnPoint == true) {
		std::cout << "hello" << std::endl;
		i32vec2Index = cPortal->i32vec2RespawnIndex;
	}

	else if (clone == false) {
		i32vec2Index = glm::i32vec2(uiCol, uiRow);
		std::cout << i32vec2Index.y << std::endl;
	}
	
	
	if (cMap2D->FindValue(200, uiRow, uiCol) == false)
		return false;    // Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	//i32vec2Index = glm::i32vec2(uiCol, uiRow);

	// Set the start position of the Player to iRow and iCol
//	i32vec2Index = glm::i32vec2(uiCol, uiRow);

	
	// By default, microsteps should be zero
	i32vec2NumMicroSteps = glm::i32vec2(0, 0);


	//Set it to fall upon entering new level
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);

	//CS: Reset double jump
	//jumpCount = 0;

	//CS: Play the "idle" animation as default
	animatedSprites->PlayAnimation("idle", -1, 1.0f);

	//CS: Init the color to white
	playerColour = glm::vec4(1.0, 1.0, 1.0, 1.0);


}

void CPlayer2D::IsPlaformStepped(double dt)
{
	if (cMap2D->GetMapInfo(i32vec2Index.y - 1, i32vec2Index.x) == 142)
	{

		fallTimer -= dt;
		if ((fallTimer <= 1.5) && (fallTimer > 0))
		{
			cMap2D->SetMapInfo(i32vec2Index.y - 1, i32vec2Index.x, 0);
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);

		}

	}
	else
	{
		fallTimer = 3; //platform will not fall if player keeps moving as timer remains at start
	}
}

void CPlayer2D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics2D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics2D.Update();
		// Get the displacement from the physics engine
		glm::vec2 v2Displacement = cPhysics2D.GetDisplacement();

		// Store the current i32vec2Index.y
		int iIndex_YAxis_OLD = i32vec2Index.y;

		int iDisplacement_MicroSteps = (int)(v2Displacement.y / cSettings->MICRO_STEP_YAXIS); //DIsplacement divide by distance for 1 microstep
		if (i32vec2Index.y < (int)cSettings->NUM_TILES_YAXIS)
		{
			i32vec2NumMicroSteps.y += iDisplacement_MicroSteps;
			if (i32vec2NumMicroSteps.y > cSettings->NUM_STEPS_PER_TILE_YAXIS)
			{
				i32vec2NumMicroSteps.y -= cSettings->NUM_STEPS_PER_TILE_YAXIS;
				if (i32vec2NumMicroSteps.y < 0)
					i32vec2NumMicroSteps.y = 0;
				i32vec2Index.y++;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(UP);

		// Iterate through all rows until the proposed row
		// Check if the player will hit a tile; stop jump if so.
		int iIndex_YAxis_Proposed = i32vec2Index.y;
		for (int i = iIndex_YAxis_OLD; i <= iIndex_YAxis_Proposed; i++)
		{
			// Change the player's index to the current i value
			i32vec2Index.y = i;
			// If the new position is not feasible, then revert to old position
			if (CheckPosition(UP) == false)
			{
				// Set the Physics to fall status
				i32vec2NumMicroSteps.y = 0;
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
				break;
			}
		}

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics2D.GetStatus() == CPhysics2D::STATUS::JUMP) && (cPhysics2D.GetInitialVelocity().y <= 0.0f))
		{
			// Set status to fall
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}
	}
	else if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics2D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics2D.Update();
		// Get the displacement from the physics engine
		glm::vec2 v2Displacement = cPhysics2D.GetDisplacement();

		// Store the current i32vec2Index.y
		int iIndex_YAxis_OLD = i32vec2Index.y;

		// Translate the displacement from pixels to indices
		int iDisplacement_MicroSteps = (int)(v2Displacement.y / cSettings->MICRO_STEP_YAXIS);

		if (i32vec2Index.y >= 0)
		{
			i32vec2NumMicroSteps.y -= fabs(iDisplacement_MicroSteps);
			if (i32vec2NumMicroSteps.y < 0)
			{
				i32vec2NumMicroSteps.y = ((int)cSettings->NUM_STEPS_PER_TILE_YAXIS) - 1;
				i32vec2Index.y--;
			}
		}


		// Constraint the player's position within the screen boundary
		Constraint(DOWN);

		// Iterate through all rows until the proposed row
		// Check if the player will hit a tile; stop fall if so.
		int iIndex_YAxis_Proposed = i32vec2Index.y;
		for (int i = iIndex_YAxis_OLD; i >= iIndex_YAxis_Proposed; i--)
		{
			// Change the player's index to the current i value
			i32vec2Index.y = i;
			// If the new position is not feasible, then revert to old position
			if (CheckPosition(DOWN) == false)
			{
				// Revert to the previous position
				if (i != iIndex_YAxis_OLD)
					i32vec2Index.y = i + 1;
				// Set the Physics to idle status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::IDLE);
				i32vec2NumMicroSteps.y = 0;
				break;
			}
		}
	}

	//Store the current i32vec2Index.y

}

/**
 @brief Let player interact with the map. You can add collectibles such as powerups and health here.
 */
void CPlayer2D::InteractWithMap(void)
{
	
	switch (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x))
	{
	case 1:
		//keys
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		//Increase the tree by 1
		cInventoryItem = cInventoryManager->GetItem("Tree");
		cInventoryItem->Add(1);
		cSoundController->PlaySoundByID(12);
		//Play a bell sound
		//cSoundController->PlaySoundByID(1);
		if (cInventoryItem->GetCount() == 1)
		{
			activateDoor = true;
		}
		break;
	case 2:
		//fastermovement powerup
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		speedboost = true;
		cSoundController->PlaySoundByID(4);
		break;
	case 3:
		//coins
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		cInventoryItem = cInventoryManager->GetItem("Coins");
		cInventoryItem->Add(1);
		cSoundController->PlaySoundByID(4);
		break;
	//case 4:
	//	//lives
	//	cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
	//	//Increase the lives by 1
	//	cInventoryItem = cInventoryManager->GetItem("Lives");
	//	cInventoryItem->Add(1);
	//	break;
	case 5:
		// invisible powerup
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		cInventoryItem = cInventoryManager->GetItem("Ultimate");
		cInventoryItem->Add(10);
	//	cSoundController->PlaySoundByID(5);
		break;
	//case 6:
	//	// higher jump
	//	cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
	//	jumppoweractive = true;
	//	playerColour = glm::vec4(1.0, 1.0, 0.0, 1.0);
	//	cSoundController->PlaySoundByID(5);
	//	break;
	case 7:
		// teleport next level
		if (activateDoor == true) {
			cGameManager->bLevelCompleted = true;
			cSoundController->PlaySoundByID(10);
			cInventoryItem = cInventoryManager->GetItem("Health");
			cInventoryItem->Add(100);
			cInventoryItem = cInventoryManager->GetItem("Tree");
			cInventoryItem->Remove(5);
			powerupActive = false;
			jumppoweractive = false;
		}
		break;
	case 8:
		// Switches.
		switchesActivated += 1;
		if (cMap2D->GetLevel() == 1)
		{
			if (switchesActivated == 1)
			{
				//First Gate blocking the player from dropping down
				cMap2D->SetMapInfo(16, 15, 0);
				cMap2D->SetMapInfo(16, 16, 0);
				cMap2D->SetMapInfo(17, 15, 0);
				cMap2D->SetMapInfo(17, 16, 0);
				cMap2D->SetMapInfo(18, 15, 0);
				cMap2D->SetMapInfo(18, 16, 0);
				//Second Gate to the left
				cMap2D->SetMapInfo(15, 3, 0);
				cMap2D->SetMapInfo(14, 3, 0);
				cMap2D->SetMapInfo(13, 3, 0);
				cMap2D->SetMapInfo(12, 3, 0);
				//Third Gate to the right
				cMap2D->SetMapInfo(15, 22, 0);
				cMap2D->SetMapInfo(14, 22, 0);
				cMap2D->SetMapInfo(13, 22, 0);
				cMap2D->SetMapInfo(12, 22, 0);
			}
			if (switchesActivated == 2)
			{
				cMap2D->SetMapInfo(11, 15, 0);
				cMap2D->SetMapInfo(11, 16, 0);
			}
			if (switchesActivated == 3)
			{
				cMap2D->SetMapInfo(10, 15, 0);
				cMap2D->SetMapInfo(10, 16, 0);
			}
		}
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 9);
		break;
	case 12: //Remote
		cSoundController->PlaySoundByID(12);
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		cMap2D->SetMapInfo(21, 30, 11);
		isRemote = true;
		break;
	case 13:
		cGameManager->bLevelCompleted = true;
		break;
	case 15:
		cSoundController->PlaySoundByID(12);
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		cInventoryItem = cInventoryManager->GetItem("TimestopTimer");
		cInventoryItem->Add(1);
		break;
	case 16:
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(1);
		playerColour = glm::vec4(1.0, 0.0, 0.0, 1.0);
		break;
	case 17:
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(1);
		playerColour = glm::vec4(1.0, 0.0, 0.0, 1.0);
		break;
	case 18:
		cSoundController->PlaySoundByID(12);
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		cInventoryItem = cInventoryManager->GetItem("ClonePowerup");
		cInventoryItem->Add(1);
		break;

		/*
	case 20:
		// Decrease the health by 1
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(1);
		speedboost = false;
		playerColour = glm::vec4(1.0, 0.0, 0.0, 1.0);
		break;
	case 21:
		// Increase the health by 1
		isHealing = true;
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Add(1);
		//cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		playerColour = glm::vec4(0.0, 1.0, 0.0, 1.0);
		break;
	case 22:
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(1);
		playerColour = glm::vec4(1.0, 0.0, 0.0, 1.0);
		break;
		*/
	

	default:
		if (speedboost == true)
		{
			playerColour = glm::vec4(0, 0, 1.0, 1.0);
		}
		if (powerupActive == true)
		{
			playerColour = glm::vec4(1.0, 1.0, 1.0, 1.0);
		}

		if (jumppoweractive == true)
		{
			playerColour = glm::vec4(1.0, 1.0, 0.0, 1.0);
		}

		if (speedboost == false && powerupActive == false && jumppoweractive == false) {
			playerColour = glm::vec4(1.0, 1.0, 1.0, 1.0);
			isHealing = false;
			break;
		}
	}
	
}

void CPlayer2D::UpdateHealthLives(void)
{

	//Update health and lives
	cInventoryItem = cInventoryManager->GetItem("Health");
	//Check if a life is lost
	if (cInventoryItem->GetCount() <= 0)
	{
		CGameManager::GetInstance()->bGameToRestart = true;
		//Reset the health to max value
		cInventoryItem->iItemCount = cInventoryItem->GetMaxCount();
		//But we  redeuce the lives by 1.
		cInventoryItem = cInventoryManager->GetItem("Lives");
		cInventoryItem->Remove(1);
		//cSoundController->PlaySoundByID(9);

		//Check if there is no lives left..
		if (cInventoryItem->GetCount() <= 0)
		{
			//Player loses the game
			CGameManager::GetInstance()->bPlayerLost = true;
			cSoundController->PlaySoundByID(2);

			cInventoryItem = cInventoryManager->GetItem("Tree");
			cInventoryItem->Remove(5);

			powerupActive = false;
			jumppoweractive = false;
			speedboost = false;

			/*if (cMap2D->GetLevel() == 0) {

				ResetMap();
			}
			if (cMap2D->GetLevel() == 1) {

				ResetMap();
			}*/

		}
	}

	
}
