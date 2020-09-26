#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include "exprtk.hpp"

class Point {
public:
	Point();

	void getCurrPos(float& x, float& y, float& time);
	void setCurrPos(float x, float y);
	void CoolDown(float& x, float& y, float& time, float & fade);
	bool checkIfCooling();
	void resetX();
	void resetY();
private:
	float x;
	float y;
	int time;
	float fade;
	bool isCoolingDown;
};
#endif