#include "Light.h"

Light::Light()
{
	position = glm::vec3(0.0f);
	currentColor = glm::vec3(1.0f);
	defaultColor = currentColor;
	isOn = true;
}

Light::~Light()
{
}

void Light::setPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void Light::setColor(float r, float g, float b)
{
	currentColor = glm::vec3(r, g, b);
	defaultColor = currentColor;
}

void Light::toggleLight()
{
	if (isOn)
	{
		currentColor = glm::vec3(0.0f);
		isOn = false;
	}
	else
	{
		currentColor = defaultColor;
		isOn = true;
	}
}

void Light::reset()
{
	currentColor = defaultColor;
	isOn = true;
}