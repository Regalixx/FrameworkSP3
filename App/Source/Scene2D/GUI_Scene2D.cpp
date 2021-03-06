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

	/*
	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2((float)cSettings->iWindowWidth, (float)cSettings->iWindowHeight));
	ImGui::SetWindowFontScale(1.5f);
	//Display the FPS
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS: %d", cFPSCounter->GetFrameRate());
	ImGui::End();
	*/
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 1.0f, 0.25f));

	ImGuiWindowFlags healthWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;


	ImGui::Begin("Health", NULL, healthWindowFlags);
	ImGui::SetWindowPos(ImVec2(15.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
	cInventoryItem = cInventoryManager->GetItem("Health");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(200.0f, 20.0f));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::End();

	ImGuiWindowFlags staminaWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Stamina", NULL, staminaWindowFlags);
	ImGui::SetWindowPos(ImVec2(15.0f, 50.0f));
	ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
	cInventoryItem = cInventoryManager->GetItem("Stamina");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(200.0f, 20.0f));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::End();

	ImGuiWindowFlags ultimateBar = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Ultimate", NULL, ultimateBar);
	ImGui::SetWindowPos(ImVec2(15.0f, 100.0f));
	ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
	cInventoryItem = cInventoryManager->GetItem("Ultimate");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(200.0f, 20.0f));
	ImGui::PopStyleColor();
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
			ImGui::SetWindowPos(ImVec2(0.f, 0.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("border");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(1920, 1080),
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
			ImGui::SetWindowPos(ImVec2(120.f, 40.f));
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

		if (CPlayer2D::GetInstance()->isDamaged == true)
		{
			ImGuiWindowFlags dimensionborderWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;



			ImGui::Begin("damageborder", NULL, dimensionborderWindowFlags);
			ImGui::SetWindowPos(ImVec2(0.f, 0.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("damageborder");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(1920, 1080),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::End();
		}

		if (CPlayer2D::GetInstance()->isPoisoned == true)
		{
			ImGuiWindowFlags dimensionborderWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;



			ImGui::Begin("PoisonedBorder", NULL, dimensionborderWindowFlags);
			ImGui::SetWindowPos(ImVec2(0.f, 0.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("PoisonedBorder");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(1920, 1080),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::End();
		}

		if (CPlayer2D::GetInstance()->isStunned == true)
		{
			ImGuiWindowFlags dimensionborderWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;



			ImGui::Begin("StunnedBorder", NULL, dimensionborderWindowFlags);
			ImGui::SetWindowPos(ImVec2(0.f, 0.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("StunnedBorder");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(1920, 1080),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::End();
		}

		if (CPlayer2D::GetInstance()->isElectrified == true)
		{
			ImGuiWindowFlags dimensionborderWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;



			ImGui::Begin("ElectrifyBorder", NULL, dimensionborderWindowFlags);
			ImGui::SetWindowPos(ImVec2(0.f, 0.f));
			ImGui::SetWindowSize(ImVec2(25.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("ElectrifyBorder");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(1920, 1080),
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
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("Lives", NULL, livesWindowFlags);
			ImGui::SetWindowPos(ImVec2(600.0f, 0.0f));
			ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("Lives");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(cInventoryItem->vec2Size.x * 1.2, cInventoryItem->vec2Size.y * 1.2),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::SetWindowFontScale(2.f);
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d",
				cInventoryItem->GetCount());
			ImGui::End();

			ImGuiWindowFlags timerWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("TimestopTimer", NULL, timerWindowFlags);
			ImGui::SetWindowPos(ImVec2(1600.0f, 0.0f));
			ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("TimestopTimer");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::SetWindowFontScale(2.f);
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d / %d",
				cInventoryItem->GetCount(), cInventoryItem->GetMaxCount());
			ImGui::End();

			//Render the clone powerup
			ImGuiWindowFlags cloneWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("ClonePowerup", NULL, cloneWindowFlags);
			ImGui::SetWindowPos(ImVec2(1740.0f, 0.0f));
			ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
			cInventoryItem = cInventoryManager->GetItem("ClonePowerup");
			ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
				ImVec2(cInventoryItem->vec2Size.x, cInventoryItem->vec2Size.y),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::SetWindowFontScale(2.f);
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d",
				cInventoryItem->GetCount());
			ImGui::End();

	//Render Coins Collected
		ImGuiWindowFlags CoinsWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("Coins", NULL, CoinsWindowFlags);
		ImGui::SetWindowPos(ImVec2(680,  0.0f));
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

	cInventoryItem = cInventoryManager->GetItem("Tree"); //Time pieces
	ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Image", NULL, inventoryWindowFlags);

	//ImGui::Begin("Image");
		ImGui::SetWindowPos(ImVec2(780.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2(100.0f, 25.0f));
		ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
			ImVec2(cInventoryItem->vec2Size.x * 1.2f, cInventoryItem->vec2Size.y * 1.2f),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::SetWindowFontScale(2.f);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Pieces: %d / %d",
		cInventoryItem->GetCount(), cInventoryItem->GetMaxCount());
	//ImGui::End();
	ImGui::PopStyleColor();
	ImGui::End();

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
