#ifndef FIELD_H
#define FIELD_H
#include "Point.h"
#include <iostream>

class Field {
public:
	Field(int size, std::string x, std::string y);
	~Field();
	void Generate(float *& vertices);
	void SetEquations(std::string x, std::string y);
	void SetPointPos(Point & a_point, float & time);
private:
	Point ** pointField;
	exprtk::expression<float> expressionX;
	exprtk::expression<float> expressionY;
	float x;
	float y;
	int fieldSize;
};
#endif
