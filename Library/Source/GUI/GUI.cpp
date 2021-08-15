/**
 CGUI
 By: Toh Da Jun
 Date: Apr 2021
 */
#include "GUI.h"

 // Include Shader Manager
#include "..\RenderControl\ShaderManager.h"

// Include ImageLoader
#include "..\System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI::CGUI(void)
	: cTextRenderer(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI::~CGUI(void)
{
	if (cTextRenderer)
	{
		cTextRenderer->Destroy();
		cTextRenderer = NULL;
	}
}

/**
  @brief Initialise this instance
  */
bool CGUI::Init(void)
{
	// Create and initialise the Map 2D
	cTextRenderer = CTextRenderer::GetInstance();
	// Set a shader to this class
	cTextRenderer->SetShader("textShader");
	// Initialise the instance
	if (cTextRenderer->Init() == false)
	{
		cout << "Failed to load CTextRenderer" << endl;
		return false;
	}

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	return true;
}

/**
 @brief Update this instance
 */
void CGUI::Update(const double dElapsedTime)
{
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI::PreRender(void)
{
	// Call the Map2D's PreRender()
	cTextRenderer->PreRender();
}

/**
 @brief Render this instance
 */
void CGUI::Render(void)
{
	// Call the CTextRenderer's Render()
	cTextRenderer->Render("DM2213 2D Game Creation", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render FPS info
	cTextRenderer->Render(cFPSCounter->GetFrameRateString(),
		10.0f, 580.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI::PostRender(void)
{
	// Call the Map2D's PostRender()
	cTextRenderer->PostRender();
}
