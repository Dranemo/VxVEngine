#pragma once

#include "APIopenGL.hpp"
#include "EngineGUI.h"
#include "ModelManager.h"
#include "PrefabManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "EngineState.h"
#include <iostream>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imfilebrowser.h"

#include "Debug.h"


/**
 * Setup the engine.
 */
class Engine {
private:
	static Engine* m_instance;
	SceneManager* m_sceneManager;
	EngineState* m_engineState = EngineState::GetInstance();
	float m_lastTime = glfwGetTime();
	std::vector<GameObject*> m_goList;
	EngineGUI* m_gui;
	APIopenGL* m_apiGraphic;
public:
	Manager* manager;

	static Engine* GetInstance()
	{
		if (!m_instance)
			m_instance = new Engine();
		return m_instance;
	}


	Engine(){}

	/**
	 * Initialize all the managers and the engine.
	 */
	void InitEngine();

	void Start();

	void Update();

	void Stop(APIopenGL* _apiGraphic);

	//Load and Init all Graphic API
	void Startup(EngineGUI* _gui, APIopenGL* _apiGraphic);
	//Draw the loop
	void ApiDrawLoopSetup(APIopenGL* _apiGraphic);
	//Check if the window is closed
	void CheckCloseWindow(APIopenGL* _apiGraphic, Engine* _engine);

	/**
	* First instance on play
	* @return void
	*/
	void PlayInit();
	/**
	* @brief Second instance on play
	*/
	void PlayStart();
	/**
	* @brief Update every frames after Start
	*/
	void PlayUpdate();
	/**
	* @brief Last instance before stop
	*/
	void PlayStop();

	//Get the EngineState instance
	EngineState* GetEngineState()
	{
		return m_engineState;
	}

	/**
	* @brief Get the play state of the engine
	* @return EngineState::ActiveState Return the play state of the engine
	*/
	EngineState::ActiveState GetActiveState()
	{
		return m_engineState->GetActiveState();
	}

	/** 
	* @brief Get the booting state of the engine
	* @return EngineState::BootingState Return the booting state of the engine
	*/
	EngineState::BootingState GetBootingState()
	{
		return m_engineState->GetBootingState();
	}

	void UpdateGameObjectList() 
	{
		m_goList = m_sceneManager->GetCurrentScene()->GetAllGameObjects();
	}

	double getLastTime()
	{
		return m_lastTime;
	}
	void setLastTime(double _lastTime)
	{
		m_lastTime = _lastTime;
	}
};
