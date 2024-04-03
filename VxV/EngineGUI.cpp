#include "EngineGUI.h"

#include <imgui.h>

void EngineGUI::UpdateGui()
{
	//ImGui::ShowDemoWindow();

 // Fen�tre pour l'acc�s aux fichiers
    if (ImGui::Begin("Explorateur de fichiers")) {
        // Utiliser ici les fonctions de ImGui pour lister les fichiers et g�rer les interactions
        ImGui::Text("Contenu du r�pertoire : ...");
    }
    ImGui::End();



    // Fen�tre pour les GameObjects
    if (ImGui::Begin("GameObjects")) {
        // Affiche et permet la manipulation des GameObjects existants
    }
    ImGui::End();

    // Fen�tre pour les sc�nes
    if (ImGui::Begin("Sc�nes")) {
        // Liste les sc�nes disponibles et permet d'en s�lectionner une, la charger, etc.
    }
    ImGui::End();

    // Console
    if (ImGui::Begin("Console")) {
        // Tu peux ici impl�menter une console pour les logs ou les commandes
    }
    ImGui::End();
}

void EngineGUI::RenderGui()
{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
