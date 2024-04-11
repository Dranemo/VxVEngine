#include "EngineGUI.h"
#include <imgui.h>
#include "FileExplorer.h"
#include <imgui.h>
#include "WindowGui.h"

#include "imgui_impl_opengl3.h"

void EngineGUI::UpdateGui()
{
	//ImGui::ShowDemoWindow();

	MainMenuBar();

 // Fen�tre pour l'acc�s aux fichiers
    ShowFileExplorer(); 
	//ShowMenuBar();
	ShowInfo();
}

void EngineGUI::RenderGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
