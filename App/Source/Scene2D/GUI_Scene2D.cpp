/**
 CGUI_Scene2D
 By: Toh Da Jun
 Date: May 2021
 */
#include "GUI_Scene2D.h"


#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene2D::CGUI_Scene2D(void)
	: cSettings(NULL)
	, window_flags(0)
	, m_fProgressBar(0.0f)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)

{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene2D::~CGUI_Scene2D(void)
{
	if (cInventoryManager)
	{
		cInventoryManager->Destroy();
		cInventoryManager = NULL;
	}
	// We won't delete this since it was created elsewhere
	cSettings = NULL;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


}

/**
  @brief Initialise this instance
  */
bool CGUI_Scene2D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(CSettings::GetInstance()->pWindow, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Define the window flags
	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	


	
	//Initialise the cInventoryManager
	cInventoryManager = CInventoryManager::GetInstance();
	//Add a Tree as one of the inventory items
	//cInventoryItem = cInventoryManager->Add("Tree", "Image/Scene2D_TreeTile.tga", 5, 0);
	cInventoryItem = cInventoryManager->Add("Tree", "Image/portalpiece.tga", 5, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);
	cInventoryItem = cInventoryManager->Add("Coins", "Image/coins.tga",100000, 0);
	cInventoryItem->vec2Size = glm::vec2(30, 30);
	




	return true;
}

/**
 @brief Update this instance
 */
void CGUI_Scene2D::Update(const double dElapsedTime)
{
	//Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2((float)cSettings->iWindowWidth, (float)cSettings->iWindowHeight));
	ImGui::SetWindowFontScale(1.5f);
	//Display the FPS
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS: %d", cFPSCounter->GetFrameRate());
	ImGui::End();
	


	

	//Render The Health
	ImGuiWindowFlags healthWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Health", NULL, healthWindowFlags);
	ImGui::SetWindowPos(ImVec2(25.0f, 25.0f));
	ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
	cInventoryItem = cInventoryManager->GetItem("Health");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(351, 90),
		ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::SameLine();
	


	ImGui::End();

	//Render The Health
	ImGuiWindowFlags progressFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Health", NULL, progressFlags);
	ImGui::SetWindowPos(ImVec2(65.0f, -35.0f));
	ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
	cInventoryItem = cInventoryManager->GetItem("Health");
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(250.0f, 10.0f));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	
	ImGui::End();
	//Render background
		if (CPlayer2D::GetInstance()->TimeStop == true)
		{
			ImGuiWindowFlags dimensionWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;


			ImGui::Begin("background", NULL, dimensionWindowFlags);
			ImGui::SetWindowPos(ImVec2(-10.f, -10.f));
			ImGui::SetWindowSize(ImVec2(150.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("background");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(2000, 1080),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::End();
		}

		//Render dimension border

		if (CPlayer2D::GetInstance()->TimeStop == true)
		{
			ImGuiWindowFlags dimensionborderWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;
				


			ImGui::Begin("border", NULL, dimensionborderWindowFlags);
			ImGui::SetWindowPos(ImVec2(-10.f, -10.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("border");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(1890, 980),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::End();
		}

		//Render dimension border

		if (CPlayer2D::GetInstance()->TimeStop == true)
		{
			ImGuiWindowFlags dimensionborderWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;



			ImGui::Begin("dimensiontext", NULL, dimensionborderWindowFlags);
			ImGui::SetWindowPos(ImVec2(60.f, 40.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("dimensiontext");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(254, 64),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::End();
		}

			//Render the Lives
			ImGuiWindowFlags livesWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("Lives", NULL, livesWindowFlags);
			ImGui::SetWindowPos(ImVec2(700.0f, 0.0f));
			ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("Lives");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::SetWindowFontScale(1.5f);
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%d / %d",
				cInventoryItem->GetCount(), cInventoryItem->GetMaxCount());
			ImGui::End();
		

	//Render Coins Collected
		



		ImGuiWindowFlags CoinsWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("Coins", NULL, CoinsWindowFlags);
		ImGui::SetWindowPos(ImVec2(700.0f,  30.0f));
		ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
		cInventoryItem = cInventoryManager->GetItem("Coins");
		ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
			ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::SetWindowFontScale(2.f);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d",
			cInventoryItem->GetCount(), cInventoryItem->GetMaxCount());
		ImGui::End();




	//Render the inventory items
	cInventoryItem = cInventoryManager->GetItem("Tree");
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Image", NULL, inventoryWindowFlags);

	ImGui::Begin("Image");
		ImGui::SetWindowPos(ImVec2(25.0f, 850.0f));
		ImGui::SetWindowSize(ImVec2(200.0f, 25.0f));
		ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
			ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SameLine();
		ImGui::SetWindowFontScale(1.5f);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Pieces: %d / %d",
		cInventoryItem->GetCount(), cInventoryItem->GetMaxCount());

		
	ImGui::End();
	ImGui::End();
	ImGui::PopStyleColor();

	
	
	

	
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI_Scene2D::PreRender(void)
{
}

/**
 @brief Render this instance
 */
void CGUI_Scene2D::Render(void)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI_Scene2D::PostRender(void)
{
}
