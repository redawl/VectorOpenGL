#ifndef FIELD_H
#define FIELD_H
#include "point.h"

class Field {
public:
	Field(int size, std::string x, std::string y);
	~Field();
	void Generate(float *& vertices, float speedFactor);
	bool SetEquations(std::string x, std::string y);
	void SetPointPos(Point & a_point, float & time, float speedFactor);
private:
	Point * pointField;
	exprtk::expression<float> expressionX;
	exprtk::expression<float> expressionY;
	float x;
	float y;
	int fieldSize;
};
#endif

