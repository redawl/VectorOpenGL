#include "Point.h"

Point::Point() {
	x = ((float)(rand()) / 500.0f) - 5.0f;
	y = ((float)(rand()) / 500.0f) - 5.0f;

	velocityX = 0;
	velocityY = 0;
	time = 1;
	isCoolingDown = false;
}

void Point::resetX() {
	x = ((float)(rand()) / 500.0f) - 5.0f;
	time = 1;
}

void Point::resetY() {
	y = ((float)(rand()) / 500.0f) - 5.0f;
	time = 1;
}

void Point::getCurrPos(float& x, float& y, float& time) {
	float tempX = (this->x + this->y);//cos(4 * ((this->x * this->x) + (this->y * this->y)));
	float tempY = (this->y - this->x);//((this->y * this->y) - (this->x * this->x));
	this->x += scalingFactor * tempX;
	if (this->x > 10.0f || this->x < -10.0f) {
		resetX();
	}
	x = this->x;
	this->y += scalingFactor * tempY;
	if (this->y > 10.0f || this->y < -10.0f)
		resetY();

	if (rand() % 110 == 10) {
		isCoolingDown = true;
	}

	time = (float)this->time;
	
	this->time++;

	y = this->y;
}

void Point::CoolDown(float &x, float &y, float & time) {
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
}

bool Point::checkIfCooling() {
	return isCoolingDown;
}