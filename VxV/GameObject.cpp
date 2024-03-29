#include "GameObject.h"

#include "SceneManager.h"




GameObject::GameObject(std::string name_) {
    name = name_;
    components.push_back(new Transform(this));
    components.push_back(new Script(this, "script"));


    Manager* manager = Manager::GetInstance();
    SceneManager* sceneManager = manager->GetManager<SceneManager>();
    currentScene = sceneManager->GetCurrentScene();


    std::vector<GameObject*> goList = currentScene->GetAllGameObjects();


    parent = nullptr;


    bool idFound = false;
    int i = 0;

    while (!idFound) {
        bool idFree = true;
        for (GameObject* go : goList) {
            if (go->GetId() == i) {
                idFree = false;
                break;
            }
        }
        if (idFree) {
            id = i;
            idFound = true;
        }
        else i++;

    }




    currentScene->AddGameObject(this);
}

void GameObject::Load(Json::Value root, GameObject* goParent) {


    GameObject* go = new GameObject();
    go->SetId(root["Id"].asInt());
    go->name = root["Name"].asString();
    go->isChild = root["Is Child"].asBool();

    for (const Json::Value compJson : root["Components"]) {
        go->LoadComponent(compJson);
    }

    if (root.isMember("GameObjectsChild")) {
        for (const Json::Value gameObjectJson : root["GameObjectsChild"]) {
            Load(gameObjectJson, go);
        }
    }

    if (go->isChild) {
        goParent->AddChild(go);
    }
}

void GameObject::Save(Json::Value& root) {
    Json::Value gameObjectJson;

    gameObjectJson["Id"] = id;
    gameObjectJson["Name"] = name;
    gameObjectJson["Is Child"] = isChild;


    gameObjectJson["Components"] = Json::Value(Json::arrayValue);

    for (Component* comp : components)
    {
        Json::Value compJson;
        comp->Save(compJson);
        gameObjectJson["Components"].append(compJson);
    }

    if (!childObjects.empty()) {
        gameObjectJson["GameObjectsChild"] = Json::Value(Json::arrayValue);

        for (GameObject* goChild : childObjects) {
            goChild->Save(gameObjectJson);
        }
    }


    if (isChild) {
        root["GameObjectsChild"].append(gameObjectJson);
    }
    else {
        root["GameObjects"].append(gameObjectJson);
    }


}

void GameObject::Delete(GameObject* go) {
    if (go->isChild) {
        go->GetParent()->GetChildByName(go->name);

    }
}



void GameObject::AddChild(GameObject* go)
{
    go->GetComponent<Transform>()->position.x -= GetComponent<Transform>()->position.x;
    go->GetComponent<Transform>()->position.y -= GetComponent<Transform>()->position.y;
    go->GetComponent<Transform>()->position.z -= GetComponent<Transform>()->position.z;
    go->origin = origin;

    go->isChild = true;
    go->AddParent(this);


    // Enlever de la liste des gameObjects (only parents)
    for (int i = currentScene->GetGameObjects().size() - 1; i >= 0; --i)
    {
        GameObject* goList = currentScene->GetGameObjects()[i];
        if (goList == go) {
            auto item = currentScene->GetGameObjects().begin() + i;
            currentScene->GetGameObjects().erase(item);
            break;
        }
    }

    childObjects.push_back(go);
}

std::vector<GameObject*> GameObject::GetChilds() {
    return childObjects;
}

GameObject* GameObject::GetChildByName(std::string name) {
    for (GameObject* go : childObjects)
    {
        if (go->name == name) {
            return go;
        }
    }

    return nullptr;
}

void GameObject::RemoveChild(GameObject* goChild) {
    for (int i = 0; i < childObjects.size(); i ++)
    {
        GameObject* go = childObjects[i];
        if (go == goChild) {
            go->parent = nullptr;
            childObjects.erase(childObjects.begin() + i);
            break;
        }
    }
}



template<typename T>
void GameObject::AddComponent() {

    static_assert(std::is_base_of<Component, T>::value, "T doit �tre un descendant de Component");

    // Ajoute un nouveau composant de type T
    components.push_back(new T(this));
}

void GameObject::LoadComponent(Json::Value compJson) {

    Component::Type type;
    std::string typeStr = compJson["Type"].asString();

    if (typeStr == "0") {
        type = Component::Type::Transform;
    }
    else if (typeStr == "1") {
        type = Component::Type::Mesh_renderer;
    }
    else if (typeStr == "2") {
        type = Component::Type::Script;
    }


    switch (type) {
    case Component::Type::Transform:
        GetComponent<Transform>()->Load(compJson);
        break;
    case Component::Type::Script:
        GetComponent<Script>()->Load(compJson);
        break;
    }
}

std::vector<Component*> GameObject::GetComponents() {
    return components;
}

template<typename T>
T* GameObject::GetComponent() {

    for (Component* comp : components) {
        T* typedComp = dynamic_cast<T*>(comp);
        if (typedComp != nullptr) {
            return typedComp; // Retourner le composant du type T s'il est trouv�
        }
    }

    return nullptr; // Aucun composant de type T trouv�
}


