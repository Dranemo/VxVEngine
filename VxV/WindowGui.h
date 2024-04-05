#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>


static void ShowExampleMenuFile()
{
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("test");
		ImGui::MenuItem("test2");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("test3");
			ImGui::MenuItem("test4");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Border);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputInt("Input", &n, 1);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}



	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		static bool b = true;
		ImGui::Checkbox("Checked option", &b);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	ImGui::Separator();
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}
static void MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void ShowInfo() {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Informations", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Obtenez le nombre de FPS
	float fps = ImGui::GetIO().Framerate;

	// Affichez le nombre de FPS
	ImGui::Text("FPS: %.1f", fps);

	// Obtenez la position de la souris
	ImVec2 mousePos = ImGui::GetIO().MousePos;

	// Affichez la position de la souris
	ImGui::Text("Position de la souris: (%.1f, %.1f)", mousePos.x, mousePos.y);

	// Terminez la fen�tre
	ImGui::End();
}

void ShowHelp() {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Aide", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le texte d'aide
	ImGui::Text("Utilisez les touches fl�ch�es pour d�placer la cam�ra.");
	ImGui::Text("Utilisez la molette de la souris pour zoomer.");
	ImGui::Text("Utilisez la touche 'R' pour r�initialiser la cam�ra.");
	ImGui::Text("Utilisez la touche 'H' pour afficher/cacher cette fen�tre.");

	// Terminez la fen�tre
	ImGui::End();
}

void ShowAbout() {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("� propos", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le texte d'information
	ImGui::Text("Ce programme a �t� cr�� par:");
	ImGui::Text("  - Alexandre Boucher");
	ImGui::Text("  - Gabriel Lemyre");
	ImGui::Text("  - Samuel Lavoie");

	// Terminez la fen�tre
	ImGui::End();
}

void ShowMenu() {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Ajoutez un bouton pour afficher les informations
	if (ImGui::Button("Informations")) {
		ShowInfo();
	}

	// Ajoutez un bouton pour afficher l'aide
	if (ImGui::Button("Aide")) {
		ShowHelp();
	}

	// Ajoutez un bouton pour afficher les informations
	if (ImGui::Button("� propos")) {
		ShowAbout();
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowError(const std::string& message) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Erreur", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'erreur
	ImGui::Text(message.c_str());

	// Terminez la fen�tre
	ImGui::End();
}

void ShowSuccess(const std::string& message) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Succ�s", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message de succ�s
	ImGui::Text(message.c_str());

	// Terminez la fen�tre
	ImGui::End();
}


void ShowWarning(const std::string& message) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Avertissement", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'avertissement
	ImGui::Text(message.c_str());

	// Terminez la fen�tre
	ImGui::End();
}


void ShowConfirmation(const std::string& message, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Confirmation", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message de confirmation
	ImGui::Text(message.c_str());

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}



void ShowInputInt(const std::string& message, int& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::InputInt("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputFloat(const std::string& message, float& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::InputFloat("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputDouble(const std::string& message, double& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::InputDouble("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputVec2(const std::string& message, glm::vec2& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::InputFloat2("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputVec3(const std::string& message, glm::vec3& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::InputFloat3("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputVec4(const std::string& message, glm::vec4& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::InputFloat4("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputColor(const std::string& message, glm::vec4& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::ColorEdit4("", &input.x);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowInputBool(const std::string& message, bool& input, bool& show) {
	// Cr�ez une nouvelle fen�tre ImGui
	ImGui::Begin("Entr�e", &show, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	// Affichez le message d'entr�e
	ImGui::Text(message.c_str());

	// Ajoutez un champ de texte pour l'entr�e
	ImGui::Checkbox("", &input);

	// Ajoutez un bouton pour confirmer
	if (ImGui::Button("Confirmer")) {
		show = false;
	}

	// Ajoutez un bouton pour annuler
	if (ImGui::Button("Annuler")) {
		show = false;
	}

	// Terminez la fen�tre
	ImGui::End();
}

void ShowMenuBar() {
	// Cr�ez une nouvelle barre de menu ImGui
	if (ImGui::BeginMainMenuBar()) {
		// Ajoutez un menu pour les options
		if (ImGui::BeginMenu("Options")) {
			// Ajoutez un �l�ment pour afficher les informations
			if (ImGui::MenuItem("Informations")) {
				ShowInfo();
			}

			// Ajoutez un �l�ment pour afficher l'aide
			if (ImGui::MenuItem("Aide")) {
				ShowHelp();
			}

			// Ajoutez un �l�ment pour afficher les informations
			if (ImGui::MenuItem("� propos")) {
				ShowAbout();
			}

			// Terminez le menu
			ImGui::EndMenu();
		}

		// Terminez la barre de menu
		ImGui::EndMainMenuBar();
	}
}