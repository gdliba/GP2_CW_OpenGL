#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Light.h"
#include "Camera.h"
#include <array>

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();
	void linkADS(const Transform& t);

	void run();

private:

	void initSystems();
	void loadLights();
	void defineLights();
	void updateTransforms();
	void keyboardInputs(float deltaTime);
	void processInput(float deltaTime);
	void gameLoop();
	void update(float deltaTime);
	void drawGame();
	void resetScene();

	Display _gameDisplay;
	GameState _gameState;
	float counter;

	Light light1, light2;
	Mesh mesh1, mesh2, mesh3, cubeMesh;
	Texture woodTxt, brickTxt, waterTxt; 
	Shader shader, ADS;
	Transform monkeyTransform;
	Transform treeTransform;
	Transform carTransform;
	Transform cameraLight1Transform, cameraLight2Transform;

	Camera camera1, camera2, camera3;
	Camera* activeCamera;
	std::array<Camera*, 3> cameras;
	int activeCameraIndex = 0;
};

