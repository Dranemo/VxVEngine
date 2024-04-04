#include "EngineGUI.h"
#include "FileExplorer.h"
#include "WindowGui.h"
#include <imgui.h>

void EngineGUI::UpdateGui()
{
	ImGui::ShowDemoWindow();

 // Fen�tre pour l'acc�s aux fichiers
    ShowFileExplorer(); 
	ShowMenuBar();

}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
