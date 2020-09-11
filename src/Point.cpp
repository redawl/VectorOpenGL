#include "Point.h"

Point::Point(float x, float y) {
	this->x = x;
	this->y = y;
	velocityX = 0.0f;
	velocityY = 0.0f;
	time = 1;
}

Point::Point(float x, float y, float velocityX, float velocityY) {
	this->x = x;
	this->y = y;
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	time = 1;
}

Point::Point() {
	x = ((float)(rand()) / 500.0f) - 1.0f;
	y = ((float)(rand()) / 500.0f) - 1.0f;

	velocityX = 0;
	velocityY = 0;
	time = 1;
}

void Point::setVelocity(float velocityX, float velocityY) {
	this->velocityX = velocityX;
	this->velocityY = velocityY;
}

void Point::resetX() {
	x = ((float)(rand()) / 500.0f) - 1.0f;
	time = 1;
}

void Point::resetY() {
	y = ((float)(rand()) / 500.0f) - 1.0f;
	time = 1;
}

void Point::getCurrPos(float& x, float& y, float& time) {
	float tempX = /*(this->x + this->y);*/cos(4 * ((this->x * this->x) + (this->y * this->y)));
	float tempY = /*(this->y - this->x);*/((this->y * this->y) - (this->x * this->x));
	this->x += 0.001f * tempX;
	if (this->x > 1.0f || this->x < -1.0f) {
		resetX();
	}
	x = this->x;
	this->y += 0.001f * tempY;
	if (this->y > 1.0f || this->y < -1.0f)
		resetY();

	if (rand() % 110 == 10) {
		resetX();
		resetY();
	}

	time = (float)this->time;
	
	this->time++;

	y = this->y;
}