#ifndef FIELD_H
#define FIELD_H
#include "Point.h"
#include <iostream>

class Field {
public:
	Field(int size);
	~Field();
	void Generate(float *& vertices);
	void SetEquations(std::string x, std::string y);
private:
	Point ** pointField;
	std::string equationX;
	std::string equationY;
	int fieldSize;
};
#endif
