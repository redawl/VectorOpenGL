#include "Point.h"

Point::Point() {
	x = ((float)rand()) / (RAND_MAX / 20.0f) - 10.0f;
	y = ((float)rand()) / (RAND_MAX / 20.0f) - 10.0f;

	time = 1;
	isCoolingDown = false;
	fade = 1;
}

void Point::resetX() {
	x = ((float)rand()) / (RAND_MAX / 20.0f) - 10.0f;
	time = 1;
	fade = 1;
}

void Point::resetY() {
	y = ((float)rand()) / (RAND_MAX / 20.0f) - 10.0f;
	time = 1;
	fade = 1;
}

void Point::getCurrPos(float& x, float& y, float& time) {
	x = this->x;
	y = this->y;

	if (rand() % 110 == 10) {
		isCoolingDown = true;
	}

	time = (float)this->time;
	
	this->time++;
}

void Point::setCurrPos(float x, float y) {
	this->x += x;
	if (this->x > 10.0f || this->x < -10.0f) {
		resetX();
	}
	this->y += y;
	if (this->y > 10.0f || this->y < -10.0f) {
		resetY();
	}
}

void Point::CoolDown(float &x, float &y, float & time, float & fade) {
	time = this->time;
	if (this->time != 0) {
		this->time--;
		x = this->x;
		y = this->y;
		time = this->time;
	}
	else {
		resetX();
		resetY();
		isCoolingDown = false;
		this->time++;
	}
	x = this->x;
	y = this->y;
	fade = this->fade;
	this->fade *= .98;
}

bool Point::checkIfCooling() {
	return isCoolingDown;
}

