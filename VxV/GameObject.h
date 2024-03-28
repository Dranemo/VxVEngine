#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <json.h>
#include <GLM/glm.hpp>

#include "Transform.h"
#include "Script.h"

class Scene;

class GameObject {
public:
    Scene* currentScene;


    std::vector<Component*> components;

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

    std::vector<GameObject*> GetChilds();
    GameObject* GetParent();



    void AddChild(GameObject* go);
    void AddParent(GameObject* go);

    void RemoveChild(std::string name);

    // Mettre des enable if
    template<typename T>
    T* GetComponent(); // Rechercher un component

    template<typename T>
    void AddComponent();

    void LoadComponent(Json::Value compJson);


    static void Delete(GameObject* go);


private:
    int id; // ID du GO
    std::vector<GameObject*> childObjects; // Liste des enfants
    GameObject* parent;
};
