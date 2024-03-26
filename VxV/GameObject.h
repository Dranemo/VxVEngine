#pragma once
#include <memory>
#include "Component.h"  // Assurez-vous de d�finir une classe ou une structure de base Component
#include "Transform.h"
#include <iostream>
#include <vector>
#include <string>

class SceneManager;
class Scene;


class GameObject {
public:

    std::vector<Component*> components;
    SceneManager* sceneManager;
    Scene* currentScene;

    GameObject(std::string name_ = "GO");

    glm::vec3 origin;
    bool isChild = false;


    std::string name; // Nom du GO
    virtual void Init() {}; // Est appel� au d�but 
    virtual void Start() {}; // Est appel� � la premi�re frame
    virtual void Update() {}; // Est appel� � chaque frame

    const int GetId() { return id; } // Renvoie l'Id du GameObject
    void SetId(int id_) { id = id_; }

    GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject
    std::vector<Component*> GetComponents();
    std::vector<int> GetChilds();


    void AddChild(GameObject* go);


    // Mettre des enable if
    template<typename T>
    T* GetComponent(); // Rechercher un component

    template<typename T>
    void AddComponent();

    void LoadComponent(Json::Value compJson);


private:
    int id; // ID du GO
    std::vector<int> childObjects; // Liste des enfants
};
