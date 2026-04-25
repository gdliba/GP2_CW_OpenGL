#pragma once
#include <glm\glm.hpp>
class Light
{
public:
	Light();
	~Light();
	void setPosition(float x, float y, float z);
	void setColor(float r, float g, float b);
	void toggleLight();
	void reset();
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getColor() const { return currentColor; }

private:
	glm::vec3 position;
	glm::vec3 currentColor, defaultColor;
	bool isOn;
};

