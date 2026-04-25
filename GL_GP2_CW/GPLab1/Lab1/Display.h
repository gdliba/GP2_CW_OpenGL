#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
#include <cstdlib>


class Display
{
public:
	Display();
	~Display();
	void initDisplay();
	void swapBuffer();
	void clearDisplay(float r, float g, float b, float a);

	float getWidth() const;
	float getHeight() const;

private:

	void returnError(const std::string& errorString);
	
	SDL_GLContext glContext; //global variable to hold the context
	SDL_Window* sdlWindow; //holds pointer to out window
	float screenWidth;
	float screenHeight;
};

