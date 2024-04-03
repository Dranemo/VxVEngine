#pragma once

#include "SceneManager.h"
#include "ScriptManager.h"
#include "PrefabManager.h"
#include "InputManager.h"

class Engine {
private:


	enum EngineState {
		Off = 0,
		Initializing,
		Ready,
		Starting,
		Running,
		AsktoStop,
		Stopping,
		Stopped
	};


	static Engine* instance;
	EngineState state = Off;





public:
	Manager* manager;
	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	EngineState GetState() {
		return state;
	}

	Engine() {
		instance = this;
	}


	void Init() { 
		state = Initializing; 

		manager = Manager::GetInstance();
		manager->Init();
		// init les managers
		state = Ready;

		//init go
		Start();
	}

	void Start() {
		state = Starting;
		// start go
		for (Script* script : manager->GetManager<ScriptManager>()->GetScripts()) {

		script->Start();
		}


		Update();
	}
	void Update() 
	{ 
		state = Running; 
		manager = Manager::GetInstance();
		//std::vector<bool> inputState= manager->GetManager<InputManager>()->GetInputState(window);
		//manager->GetManager<InputManager>()->Update(inputState);
		for (Script* script : manager->GetManager<ScriptManager>()->GetScripts()) {

			script->Update();
		}
	}

};