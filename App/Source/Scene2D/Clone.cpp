/**
 CClone
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Clone.h"

#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"
// Include Mesh Builder
#include "Primitives/MeshBuilder.h"

// Include GLEW
#include <GL/glew.h>

// Include ImageLoader
#include "System\ImageLoader.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"
// Include math.h
#include <math.h>

#include "InventoryManager.h"

#include "InventoryItem.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CClone::CClone(void)
	: bIsActive(false)
	, cMap2D(NULL)
	, cSettings(NULL)
	, cPlayer2D(NULL)
	, sCurrentFSM(FSM::IDLE)
	, iFSMCounter(0)
	, animatedSprites(NULL)
{
	transform = glm::mat4(1.0f);	// make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	i32vec2Index = glm::i32vec2(0);

	// Initialise vecNumMicroSteps
	i32vec2NumMicroSteps = glm::i32vec2(0);

	// Initialise vec2UVCoordinate
	vec2UVCoordinate = glm::vec2(0.0f);

	//i32vec2Destination = glm::i32vec2(0, 0);	// Initialise the iDestination
	//i32vec2Direction = glm::i32vec2(0, 0);		// Initialise the iDirection
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CClone::~CClone(void)
{

	if (animatedSprites)
	{
		delete animatedSprites;
		animatedSprites = NULL;
	}
	// We won't delete this since it was created elsewhere
	cPlayer2D = NULL;

	// We won't delete this since it was created elsewhere
	cMap2D = NULL;

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

/**
  @brief Initialise this instance
  */
bool CClone::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();


	//create monster 2D CLASS
	//derive from Enemy 2D
	//override the Init()
	//override update function

	// Get the handler to the CMap2D instance

	cPlayer2D = CPlayer2D::GetInstance();
	cMap2D = CMap2D::GetInstance();
	// Find the indices for the player in arrMapInfo, and assign it to cPlayer2D
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	if (cMap2D->FindValue(304, uiRow, uiCol) == false)
		return false;	// Unable to find the start position of the player, so quit this game
	
	//if (cMap2D->FindValue(301, uiRow, uiCol) == false)
	//	return false;	// Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set the start position of the Player to iRow and iCol
	i32vec2Index = glm::i32vec2(uiCol, uiRow);
	// By default, microsteps should be zero
	i32vec2NumMicroSteps = glm::i32vec2(0, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	quadMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);

	// Load the enemy2D texture
	if (LoadTexture("Image/player3.tga", iTextureID) == false)
	{
		std::cout << "Failed to load enemy2D tile texture" << std::endl;
		return false;
	}

	

	cInventoryManager = CInventoryManager::GetInstance();
	animatedSprites = CMeshBuilder::GenerateSpriteAnimation(4, 6,
		cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);

	
	respawnToClone = false;
	canRespawnToClone = false;

	animatedSprites->AddAnimation("idle", 0, 5);
	animatedSprites->AddAnimation("right", 6, 11);
	animatedSprites->AddAnimation("left", 12, 17);
	animatedSprites->AddAnimation("jump", 18, 20);
	//CS: Play the "idle animation as default
	animatedSprites->PlayAnimation("idle", -1, 1.0f);

	//CS: Init the color to white
	currentColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

	// Set the Physics to fall status by default
	cPhysics2D.Init();
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);





	// If this class is initialised properly, then set the bIsActive to true
	
	bIsActive = true;
	
	return true;
}

/**
 @brief Update this instance
 */
void CClone::Update(const double dElapsedTime)
{

	if (cPlayer2D->clone == true && cPlayer2D->canUseClone == false)
	{
		
		i32vec2OldIndex = i32vec2Index;
		canRespawnToClone = true;
	
		
		
		//std::cout << "hello" << std::endl;
	}

	if (canRespawnToClone == true)
	{
		i32vec2RespawnIndex = i32vec2OldIndex;
	}

	if (cPlayer2D->clone == false && cPlayer2D->canUseClone == true)
	{
		
		i32vec2Index = cPlayer2D->i32vec2Index; // follow the player
	}

	//cMap2D->SetMapInfo(i32vec2Index.x, i32vec2Index.y, 147);
	

		
	//animatedSprites = cPlayer2D->animatedSprites;


	//UpdatePosition();

	//Update Jump or Fall
	UpdateJumpFall(dElapsedTime);

	animatedSprites->Update(dElapsedTime);

	// Update the UV Coordinates
	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, i32vec2Index.x, false, i32vec2NumMicroSteps.x*cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, i32vec2Index.y, false, i32vec2NumMicroSteps.y*cSettings->MICRO_STEP_YAXIS);

	
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CClone::PreRender(void)
{
	if (!bIsActive)
		return;

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
void CClone::Render(void)
{
	if (!bIsActive)
		return;

	glBindVertexArray(VAO);
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	unsigned int colorLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "runtime_color");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(vec2UVCoordinate.x,
		vec2UVCoordinate.y,
		0.0f));
	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(colorLoc, 1, glm::value_ptr(currentColor));

	// Get the texture to be rendered
	
		glBindTexture(GL_TEXTURE_2D, iTextureID);
	

	
	//// Render the tile
	////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//quadMesh->Render();

	animatedSprites->Render();

	glBindVertexArray(0);

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CClone::PostRender(void)
{
	if (!bIsActive)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}

/**
@brief Set the indices of the enemy2D
@param iIndex_XAxis A const int variable which stores the index in the x-axis
@param iIndex_YAxis A const int variable which stores the index in the y-axis
*/
//void CClone::Seti32vec2Index(const int iIndex_XAxis, const int iIndex_YAxis)
//{
//	this->i32vec2Index.x = iIndex_XAxis;
//	this->i32vec2Index.y = iIndex_YAxis;
//}
//
///**
//@brief Set the number of microsteps of the enemy2D
//@param iNumMicroSteps_XAxis A const int variable storing the current microsteps in the X-axis
//@param iNumMicroSteps_YAxis A const int variable storing the current microsteps in the Y-axis
//*/
//void CClone::Seti32vec2NumMicroSteps(const int iNumMicroSteps_XAxis, const int iNumMicroSteps_YAxis)
//{
//	this->i32vec2NumMicroSteps.x = iNumMicroSteps_XAxis;
//	this->i32vec2NumMicroSteps.y = iNumMicroSteps_YAxis;
//}
//
///**
// @brief Set the handle to cPlayer to this class instance
// @param cPlayer2D A CPlayer2D* variable which contains the pointer to the CPlayer2D instance
// */
//void CClone::SetPlayer2D(CPlayer2D* cPlayer2D)
//{
//	this->cPlayer2D = cPlayer2D;
//
//	// Update the enemy's direction
//	UpdateDirection();
//}


/**
@brief Load a texture, assign it a code and store it in MapOfTextureIDs.
@param filename A const char* variable which contains the file name of the texture
*/
bool CClone::LoadTexture(const char* filename, GLuint& iTextureID)
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
	unsigned char* data = cImageLoader->Load(filename, width, height, nrChannels, true);
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
 @brief Constraint the enemy2D's position within a boundary
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void CClone::Constraint(DIRECTION eDirection)
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
		if (i32vec2Index.x >= (int)cSettings->NUM_TILES_XAXIS - 1)
		{
			i32vec2Index.x = ((int)cSettings->NUM_TILES_XAXIS) - 1;
			i32vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == UP)
	{
		if (i32vec2Index.y >= (int)cSettings->NUM_TILES_YAXIS - 1)
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
		cout << "CClone::Constraint: Unknown direction." << endl;
	}
}

/**
 @brief Check if a position is possible to move into
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
bool CClone::CheckPosition(DIRECTION eDirection)
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

// Check if the enemy2D is in mid-air
bool CClone::IsMidAir(void)
{
	// if the player is at the bottom row, then he is not in mid-air for sure
	if (i32vec2Index.y == 0)
		return false;

	// Check if the tile below the player's current position is empty
	if ((i32vec2NumMicroSteps.x == 0) &&
		(cMap2D->GetMapInfo(i32vec2Index.y - 1, i32vec2Index.x) == 0))
	{
		return true;
	}

	return false;
}

// Update Jump or Fall
void CClone::UpdateJumpFall(const double dElapsedTime)
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
				// Align with the row
				i32vec2NumMicroSteps.y = 0;
				// Set the Physics to fall status
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
}

/**
 @brief Let enemy2D interact with the player.
 */
//bool CClone::InteractWithPlayer(void)
//{
//	glm::i32vec2 i32vec2PlayerPos = cPlayer2D->i32vec2Index;
//	
//	// Check if the enemy2D is within 1.5 indices of the player2D
//	if (((i32vec2Index.x >= i32vec2PlayerPos.x - 0.5) && 
//		(i32vec2Index.x <= i32vec2PlayerPos.x + 0.5))
//		&& 
//		((i32vec2Index.y >= i32vec2PlayerPos.y - 0.5) &&
//		(i32vec2Index.y <= i32vec2PlayerPos.y + 0.5)))
//	{
//		//cout << "Gotcha!" << endl;
//
//		cPlayer2D->playerColour = glm::vec4(1.0, 0.0, 0.0, 1.0);
//		cInventoryItem = cInventoryManager->GetItem("Health");
//		cInventoryItem->Remove(25);
//
//		// Since the player has been caught, then reset the FSM
//		sCurrentFSM = IDLE;
//		iFSMCounter = 0;
//
//
//		return true;
//	}
//	return false;
//}



/**
 @brief Update the enemy's direction.
 */
//void CClone::UpdateDirection(void)
//{
//	// Set the destination to the player
//	i32vec2Destination = cPlayer2D->i32vec2Index;
//
//	// Calculate the direction between enemy2D and player2D
//	i32vec2Direction = i32vec2Destination - i32vec2Index;
//
//	// Calculate the distance between enemy2D and player2D
//	float fDistance = cPhysics2D.CalculateDistance(i32vec2Index, i32vec2Destination);
//	if (fDistance >= 0.01f)
//	{
//		// Calculate direction vector.
//		// We need to round the numbers as it is easier to work with whole numbers for movements
//		i32vec2Direction.x = (int)round(i32vec2Direction.x / fDistance);
//		i32vec2Direction.y = (int)round(i32vec2Direction.y / fDistance);
//	}
//	else
//	{
//		// Since we are not going anywhere, set this to 0.
//		i32vec2Direction = glm::i32vec2(0);
//	}
//}

/**
 @brief Flip horizontal direction. For patrol use only
 */
//void CClone::FlipHorizontalDirection(void)
//{
//	i32vec2Direction.x *= -1;
//}

/**
@brief Update position.
*/
//void CClone::UpdatePosition(void)
//{
//
//	// Store the old position
//	i32vec2OldIndex = i32vec2Index;
//	
//	
//
//	if (i32vec2Direction.x < 0 && CheckPosition(LEFT) == true)
//	{
//		// Move left
//		const int iOldIndex = i32vec2Index.x;
//		if (i32vec2Index.x >= 0)
//		{
//			i32vec2NumMicroSteps.x--;
//			if (i32vec2NumMicroSteps.x < 0)
//			{
//				i32vec2NumMicroSteps.x = ((int)cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
//				i32vec2Index.x--;
//			}
//		}
//
//		// Constraint the enemy2D's position within the screen boundary
//		Constraint(LEFT);
//
//		//CS: play the "left animation
//		animatedSprites->PlayAnimation("left", -1, 1.0f);
//
//
//
//		// Find a feasible position for the enemy2D's current position
//		if (CheckPosition(LEFT) == false)
//		{
//			//FlipHorizontalDirection();
//			i32vec2Index = i32vec2OldIndex;
//			i32vec2NumMicroSteps.x = 0;
//		}
//
//		// Check if enemy2D is in mid-air, such as walking off a platform
//		if (IsMidAir() == true)
//		{
//			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
//		}
//
//		// Interact with the Player
//		//InteractWithPlayer();
//	}
//	else if (i32vec2Direction.x > 0 && CheckPosition(RIGHT) == true)
//	{
//		// Move right
//		const int iOldIndex = i32vec2Index.x;
//		if (i32vec2Index.x < (int)cSettings->NUM_TILES_XAXIS)
//		{
//			i32vec2NumMicroSteps.x++;
//
//			if (i32vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
//			{
//				i32vec2NumMicroSteps.x = 0;
//				i32vec2Index.x++;
//			}
//		}
//
//		// Constraint the enemy2D's position within the screen boundary
//		Constraint(RIGHT);
//		
//		animatedSprites->PlayAnimation("right", -1, 1.0f);
//
//		// Find a feasible position for the enemy2D's current position
//		if (CheckPosition(RIGHT) == false)
//		{
//			//FlipHorizontalDirection();
//			i32vec2Index = i32vec2OldIndex;
//			i32vec2NumMicroSteps.x = 0;
//		}
//
//		// Check if enemy2D is in mid-air, such as walking off a platform
//		if (IsMidAir() == true)
//		{
//			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
//		}
//
//	
//	}
//
//	
//
//
//}
