#include "Point.h"

Point::Point() {
	x = ((float)(rand()) / 500.0f) - 5.0f;
	y = ((float)(rand()) / 500.0f) - 5.0f;

	velocityX = 0;
	velocityY = 0;
	time = 1;
	isCoolingDown = false;

	symbol_table.add_variable("x", x);
	symbol_table.add_variable("y", y);
	expressionX.register_symbol_table(symbol_table);
	expressionY.register_symbol_table(symbol_table);
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
	float tempX = expressionX.value();
	float tempY = expressionY.value();
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

void Point::setEquations(std::string x, std::string y) {
	equationX = x;
	equationY = y;
	parser.compile(equationX, expressionX);
	parser.compile(equationY, expressionY);
}