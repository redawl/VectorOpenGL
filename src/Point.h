#ifndef POINT_H
#define POINT_H
#include <iostream>
class Point {
public:
	Point(float x, float y);
	Point(float x, float y, float velocityX, float velocityY);
	Point();

	void setVelocity(float velocityX, float velocityY);
	void getCurrPos(float& x, float& y, float& time);
	void resetX();
	void resetY();
private:
	float x;
	float y;
	float velocityX;
	float velocityY;
	int time;
};
#endif