#pragma once

#include <iostream>
#include <list>
#include <string>


#include "Component.h"


class GameObject 
{
public:
	std::string name; // Nom du GO

	virtual void Init() {}; // Est appel� au d�but 
	virtual void Start() {}; // Est appel� � la premi�re frame
	virtual void Update() {}; // Est appel� � chaque frame

	const int GetId() { return id; } // Renvoie l'Id du GameObject
	
	GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject

	template<typename T>
	T* GetComponent(); // Rechercher un component

	template<typename T>
	void AddComponent();


private:
	int id; // ID du GO


	std::list<Component*> components; // Liste des components
	std::list<GameObject*> childObjects; // Liste des enfants
};