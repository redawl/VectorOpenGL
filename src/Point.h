#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include "exprtk.hpp"

const float scalingFactor = 0.001f;
class Point {
public:
	Point();

	void getCurrPos(float& x, float& y, float& time);
	void setCurrPos(float x, float y, float & time);
	void CoolDown(float& x, float& y, float& time);
	bool checkIfCooling();
	void resetX();
	void resetY();
private:
	float x;
	float y;
	float velocityX;
	float velocityY;
	int time;
	bool isCoolingDown;
};
#endif