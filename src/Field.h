#ifndef FIELD_H
#define FIELD_H
#include "Point.h"

class Field {
public:
	Field(int size);
	~Field();
	void Generate(float *& vertices);
private:
	Point ** pointField;
	int fieldSize;
};
#endif
