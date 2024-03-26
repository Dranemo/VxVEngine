#include "SceneManager.h"
#include <json.h>

#include <fstream>



SceneManager* SceneManager::instance = nullptr;


SceneManager::SceneManager() {
	instance = this;


	//Rechercher les scenes
	//Les foutre dans la liste
	// 

	
	// Ajouter une scene s'il n'en existe pas d�j�
	if (scenes.empty()) {
		new Scene;
	}

	currentScene = scenes.front();

	for (Scene* sc : scenes) {
		std::string str = sc->name;
		std::cout << str << std::endl;
	}

}

void SceneManager::SaveScene() {

	Json::Value root;

	


	std::ofstream outputFile("output.json");
	if (outputFile.is_open())
	{
		std::vector<GameObject*> listGO = currentScene->GetGameObjects();
		outputFile << currentScene->name << std::endl;

		for (GameObject* go : currentScene->GetGameObjects()) {
			outputFile << go->name << std::endl;
			outputFile << go->GetId() << std::endl;

			for (Component* comp : go->GetComponents()) {
				outputFile << comp->Save();
			}
		}

	
		outputFile.close();
		std::cout << "Les donn�es ont �t� �crites dans le fichier output.json avec succ�s." << std::endl;
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
	}


}