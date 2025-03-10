#include "GameObject.h"

#include <vector>

#include "PrefabManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "ScriptingComponent.h"
#include "Model.h"
#include "Light.h"

#include "Component.h"

GameObject::GameObject(std::string name_, bool PrefabLoading, Prefab* prefab_, bool copying)
{

	parent = nullptr;
	prefab = prefab_;

	if (!PrefabLoading)
	{

		Manager* manager = Manager::GetInstance();
		SceneManager* sceneManager = manager->GetManager<SceneManager>();
		currentScene = sceneManager->GetCurrentScene();
		std::vector<GameObject*> goList = currentScene->GetAllGameObjects();

		bool idFound = false;
		int i = 0;

		while (!idFound)
		{
			bool idFree = true;
			for (GameObject* go : goList)
			{
				if (go->GetId() == i)
				{
					idFree = false;
					break;
				}
			}
			if (idFree)
			{
				id = i;
				idFound = true;
			}
			else
				i++;
		}

		currentScene->AddGameObject(this);
	}

	if (prefab == nullptr)
	{
		name = name_;
		AddComponent<Transform>();
	}

	else if (!copying)
	{
		AddComponent<Transform>();
		Prefab::Copy(this, (prefab_->getGameObject()));
	}
}

GameObject::~GameObject()
{

}

void GameObject::Load(Json::Value root, GameObject* goParent, bool PrefabLoading) {
	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	GameObject* go = sceneManager->gameObjectPool.CreateGoFromPool("", PrefabLoading, nullptr, true);
	go->SetId(root["Id"].asInt());
	go->name = root["Name"].asString();
	go->isChild = root["Is Child"].asBool();

	if (root.isMember("PrefabName"))
	{
		go->prefabName = root["PrefabName"].asString();
		/*for (Prefab* prefab_ : Manager::GetInstance()->GetManager<PrefabManager>()->GetPrefabs())
		{
			if (prefab_->name == root["PrefabName"].asString())
			{
				go->prefab = prefab_;
			}
		}*/
	}

	for (const Json::Value compJson : root["Components"])
	{
		go->LoadComponent(compJson, go);
	}

	if (root.isMember("GameObjectsChild"))
	{
		for (const Json::Value gameObjectJson : root["GameObjectsChild"])
		{
			Load(gameObjectJson, go);
		}
	}

	if (go->isChild)
	{
		goParent->AddChild(go);
	}
}

void GameObject::Save(Json::Value& root)
{
	Json::Value gameObjectJson;

	gameObjectJson["Id"] = id;
	gameObjectJson["Name"] = name;
	gameObjectJson["Is Child"] = isChild;

	if (prefab != nullptr)
	{
		gameObjectJson["PrefabName"] = prefab->name;
	}

	gameObjectJson["Components"] = Json::Value(Json::arrayValue);

	for (Component* comp : components)
	{
		Json::Value compJson;
		comp->Save(compJson);
		gameObjectJson["Components"].append(compJson);
	}

	if (!childObjects.empty())
	{
		gameObjectJson["GameObjectsChild"] = Json::Value(Json::arrayValue);

		for (GameObject* goChild : childObjects)
		{
			goChild->Save(gameObjectJson);
		}
	}

	if (isChild)
	{
		root["GameObjectsChild"].append(gameObjectJson);
	}
	else
	{
		root["GameObjects"].append(gameObjectJson);
	}
}

void GameObject::Delete()
{
	GameObject* go = this;
	if (go->isChild)
	{
		go->GetParent()->RemoveChild(go);
	}
	for (Component* comp : components)
	{
		delete comp;
	}
	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();
	currentScene = sceneManager->GetCurrentScene();
	currentScene->RemoveGameObject(id);
}

void GameObject::AddChild(GameObject* go)
{
	glm::vec3 position;

	position.x = go->GetComponent<Transform>()->GetPosition().x - GetComponent<Transform>()->GetPosition().x;
	position.y = go->GetComponent<Transform>()->GetPosition().y - GetComponent<Transform>()->GetPosition().y;
	position.z = go->GetComponent<Transform>()->GetPosition().z - GetComponent<Transform>()->GetPosition().z;

	go->GetComponent<Transform>()->SetPosition(position);
	go->origin = origin;

	go->isChild = true;
	go->AddParent(this);

	// Enlever de la liste des gameObjects (only parents)
	for (int i = currentScene->GetGameObjects().size() - 1; i >= 0; --i)
	{
		GameObject* goList = currentScene->GetGameObjects()[i];
		if (goList == go)
		{
			auto item = currentScene->GetGameObjects().begin() + i;
			currentScene->GetGameObjects().erase(item);
			break;
		}
	}

	childObjects.push_back(go);
}

std::vector<GameObject*> GameObject::GetChilds()
{
	return childObjects;
}

GameObject* GameObject::GetChildByName(std::string name)
{
	for (GameObject* go : childObjects)
	{
		if (go->name == name)
		{
			return go;
		}
	}

	return nullptr;
}

void GameObject::RemoveChild(GameObject* goChild)
{
	for (int i = 0; i < childObjects.size(); i++)
	{
		GameObject* go = childObjects[i];
		if (go == goChild)
		{
			go->parent = nullptr;
			childObjects.erase(childObjects.begin() + i);
			break;
		}
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::LoadComponent(Json::Value compJson, GameObject* parentGo)
{
	switch (compJson["Type"].asInt())
	{
	case 0: break;
	case 1:
		GetComponent<Transform>()->Load(compJson, parentGo);
		break;
	case 2:
		AddComponent<Model>();
		GetComponent<Model>()->Load(compJson, parentGo);
		break;
	case 3: AddComponent<ScriptingComponent>();
		GetComponent<ScriptingComponent>()->Load(compJson, parentGo);
		break;
	case 4: AddComponent<Light>();
		break;
	}
}

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}

void GameObject::Update(){
	
	this->GetComponent<Transform>()->Rotate(0.0f, 5.0f, 0.0f);

}
