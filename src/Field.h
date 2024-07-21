#ifndef FIELD_H
#define FIELD_H
#include "Point.h"

class Field {
public:
	Field(int size, std::string x, std::string y);
	~Field();
	void Generate(float *& vertices, float scalingFactor);
	bool SetEquations(std::string x, std::string y);
	void SetPointPos(Point & a_point, float & time, float scalingFactor);
private:
	Point ** pointField;
	exprtk::expression<float> expressionX;
	exprtk::expression<float> expressionY;
	float x;
	float y;
	int fieldSize;
};
#endif

