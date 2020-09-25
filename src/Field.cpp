#include "Field.h"

Field::Field(int size, std::string x, std::string y) {
	pointField = new Point*[size];
	for (int i = 0; i < size; i++) {
		pointField[i] = new Point[size];
	}
	fieldSize = size;
	this->x = 0;
	this->y = 0;
	SetEquations(x, y);
}

Field::~Field() {
	for (int i = 0; i < fieldSize; i++){
		delete [] pointField[i];
	}
	delete [] pointField;
}

void Field::Generate(float *& vertices, float scalingFactor) {
	if (vertices == NULL)
		vertices = new float[fieldSize * fieldSize * 4];
	int index = 0;
	for (int i = 0; i < fieldSize; i++) {
		for (int j = 0; j < fieldSize; j++) {
			float time = 0;
			float fade = 1;
			if (!pointField[i][j].checkIfCooling())
				SetPointPos(pointField[i][j], time, scalingFactor);
			else
				pointField[i][j].CoolDown(x, y, time, fade);
			vertices[index] = x;
			index++;
			vertices[index] = y;
			index++;
			vertices[index] = time;
			index++;
			vertices[index] = fade;
			index++;
		}
	}
}

void Field::SetEquations(std::string x, std::string y) {
	exprtk::symbol_table<float> symbol_table;
	symbol_table.add_variable("x", this->x);
	symbol_table.add_variable("y", this->y);
	expressionX.register_symbol_table(symbol_table);
	expressionY.register_symbol_table(symbol_table);
	exprtk::parser<float> parser;
	parser.compile(x, expressionX);
	parser.compile(y, expressionY);
}

void Field::SetPointPos(Point& a_point, float & time, float scalingFactor) {
	a_point.getCurrPos(x, y, time);
	float tempX = scalingFactor * expressionX.value();
	float tempY = scalingFactor * expressionY.value();
	
	a_point.setCurrPos(tempX, tempY, time);
}