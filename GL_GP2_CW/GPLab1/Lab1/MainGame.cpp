#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	counter = 0.0f;
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

/// <summary>
/// Initialises the scene lights.
/// </summary>
void MainGame::defineLights()
{
	// Light Positions no longer need to be defined here as the lights are camera mounted 
	// and their positions will be updated each frame in loadLights() using the camera positions.

	// Define the light colours
	light1.setColor(1.0f, 1.0f, 1.0f);
	light2.setColor(0.0f, 0.0f, 0.3f);
}

/// <summary>
/// Loads the light properties into the shader uniforms. 
/// Called once per frame in drawGame(), before drawing any objects.
/// </summary>
void MainGame::loadLights()
{
	// Light 1 and 2 are camera mounted lights
	ADS.setVec3("lightPos1", camera2.getPos()); 
	ADS.setVec3("lightPos2", camera3.getPos());

	// Set the light colors uniform in the shader
	ADS.setVec3("lightColor1", light1.getColor());
	ADS.setVec3("lightColor2", light2.getColor());

	// used to set the view position uniform in ADS shader (for specular lighting calculations)
	ADS.setVec3("viewPos", activeCamera->getPos());
}

/// <summary>
/// Helper function to link the ADS shader with the transform of the object being drawn.
/// </summary>
/// <param name="t"></param>
void MainGame::linkADS(const Transform& t)
{
	// Set the model matrix uniform depending on the transform referenced by the parameter "t"
	ADS.setMat4("model", t.GetModel());
	ADS.Update(t, *activeCamera);
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 

	// Mouse input setup
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);

	defineLights();

	mesh1.loadModel("..\\res\\tree.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\car.obj");
	cubeMesh.loadModel("..\\res\\cube.obj");

	woodTxt.init("..\\res\\wood.jpg");
	waterTxt.init("..\\res\\Water.jpg");
	brickTxt.init("..\\res\\bricks.jpg");

	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag"); //new shader
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag"); //new shader
	ADS.Bind();
	ADS.setInt("diffuse", 0); // Set the diffuse texture to texture unit 0

	float aspect = (float)_gameDisplay.getWidth() / _gameDisplay.getHeight();
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);

	camera1.initCamera(glm::vec3(0, 0, -30), 70.0f, aspect, 0.01f, 1000.0f, origin);
	camera2.initCamera(glm::vec3(0, 0, 30), 70.0f, aspect, 0.01f, 1000.0f, origin);
	camera3.initCamera(glm::vec3(20.0f, -20.0f, 0.0f), 70.0f, aspect, 0.01f, 1000.0f, origin);

	cameras = { &camera1, &camera2, &camera3 };
	activeCameraIndex = 0;
	activeCamera = cameras[activeCameraIndex];
}

void MainGame::gameLoop()
{
	// used to track DeltaTime
	Uint32 lastTime = SDL_GetTicks();

	while (_gameState != GameState::EXIT)
	{
		// Track DeltaTime (the time that has passed since the last frame)
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		processInput(deltaTime);
		update(deltaTime);
		drawGame();
	}
}

void MainGame::update(float deltaTime)
{
	counter += deltaTime;
	updateTransforms();
}

/// <summary>
/// Updates the animated object transforms and the visible marker for the camera-mounted light.
/// </summary>
void MainGame::updateTransforms()
{
	monkeyTransform.SetPos(glm::vec3(-12.0f, 0.0f, 0.0f));
	monkeyTransform.SetRot(glm::vec3(0.0f, counter, 0.0f));
	monkeyTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	treeTransform.SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
	treeTransform.SetRot(glm::vec3(0.0f, -counter, 0.0f));
	treeTransform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	carTransform.SetPos(glm::vec3(12.0f, 0.0f, 0.0f));
	carTransform.SetRot(glm::vec3(0.0f, 0.0f, counter));
	carTransform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	cameraLight1Transform.SetPos(camera2.getPos());
	cameraLight1Transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
	cameraLight1Transform.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	cameraLight2Transform.SetPos(camera3.getPos());
	cameraLight2Transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
	cameraLight2Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

/// <summary>
/// Handles user input and update the camera position and orientation accordingly through cameraControl().
/// </summary>
/// <param name="deltaTime"></param>
void MainGame::processInput(float deltaTime)
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
		{
			// Toggle between the main camera and the camera-mounted light camera
			if (evnt.key.keysym.scancode == SDL_SCANCODE_C)
			{
				activeCameraIndex = (activeCameraIndex + 1) % cameras.size();
				activeCamera = cameras[activeCameraIndex];
			}
			if (evnt.key.keysym.scancode == SDL_SCANCODE_1)
				light1.toggleLight();
			if (evnt.key.keysym.scancode == SDL_SCANCODE_2)
				light2.toggleLight();
			break;
		}
		case SDL_MOUSEMOTION:
		{
			float mouseSensitivity = 0.0005f;

			activeCamera->RotateY(-evnt.motion.xrel * mouseSensitivity);
			activeCamera->Pitch(evnt.motion.yrel * mouseSensitivity);
			break;
		}
		}

	}
	keyboardInputs(deltaTime);
}

/// <summary>
/// Controls camera movement and rotation based on keyboard input.
/// Keeps processInput() cleaner and more organized.
/// </summary>
void MainGame::keyboardInputs(float deltaTime)
{
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	float speedFactor = (keyStates[SDL_SCANCODE_LSHIFT]) ? 2.0f : 1.0f; // Hold shift to move faster
	float cameraSpeed = 10.0f * deltaTime * speedFactor;
	float cameraTurnSpeed = 1.5f * deltaTime * speedFactor;

	if (keyStates[SDL_SCANCODE_ESCAPE])
		_gameState = GameState::EXIT;

	if (keyStates[SDL_SCANCODE_W])
		activeCamera->MoveForward(cameraSpeed);

	if (keyStates[SDL_SCANCODE_S])
		activeCamera->MoveBack(cameraSpeed);

	if (keyStates[SDL_SCANCODE_A])
		activeCamera->MoveLeft(cameraSpeed);

	if (keyStates[SDL_SCANCODE_D])
		activeCamera->MoveRight(cameraSpeed);

	if (keyStates[SDL_SCANCODE_LCTRL])
		activeCamera->MoveDown(cameraSpeed);

	if (keyStates[SDL_SCANCODE_SPACE])
		activeCamera->MoveUp(cameraSpeed);

	if (keyStates[SDL_SCANCODE_LEFT])
		activeCamera->RotateY(cameraTurnSpeed);

	if (keyStates[SDL_SCANCODE_RIGHT])
		activeCamera->RotateY(-cameraTurnSpeed);

	if (keyStates[SDL_SCANCODE_UP])
		activeCamera->Pitch(-cameraTurnSpeed);

	if (keyStates[SDL_SCANCODE_DOWN])
		activeCamera->Pitch(cameraTurnSpeed);

	if (keyStates[SDL_SCANCODE_F])
		counter = 0.0f;

	if (keyStates[SDL_SCANCODE_R])
		resetScene();
}

/// <summary>
/// Resets the scene to the initial state. Called when the user presses the "R" key.
/// </summary>
void MainGame::resetScene()
{
	camera1.Reset();
	camera2.Reset();
	camera3.Reset();
	light1.reset();
	light2.reset();
	counter = 0.0f;

	activeCameraIndex = 0;
	activeCamera = cameras[activeCameraIndex];
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	ADS.Bind();
	loadLights(); // extracted light calculation to once per frame, instead of once per object
	ADS.setVec3("emissive", glm::vec3(0.0f, 0.0f, 0.0f));

	brickTxt.Bind(0);
	linkADS(monkeyTransform);
	mesh2.draw();

	// shares the same shader and texture as the monkey, so no need to bind again
	linkADS(treeTransform);
	mesh1.draw();

	woodTxt.Bind(0);
	linkADS(carTransform);
	mesh3.draw();

	ADS.setVec3("emissive", light1.getColor() * 5.0f);
	linkADS(cameraLight1Transform);
	cubeMesh.draw();

	waterTxt.Bind(0);
	ADS.setVec3("emissive", light2.getColor() * 1.2f);
	linkADS(cameraLight2Transform);
	cubeMesh.draw();

	_gameDisplay.swapBuffer();
}