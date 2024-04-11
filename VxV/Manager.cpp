#include "Manager.h"
#include "SceneManager.h"
#include "PrefabManager.h"
#include "ScriptManager.h"
#include "MeshManager.h"


Manager* Manager::instance = nullptr;

void Manager::Init() {
	new SceneManager(this);
	new MeshManager(this);
	new ScriptManager(this);
	new PrefabManager(this);
}