#include "field.h"

Field::Field(int size, std::string x, std::string y) {
	pointField = new Point[size];
	fieldSize = size;
	this->x = 0;
	this->y = 0;
	SetEquations(x, y);
}

Field::~Field() {
	delete [] pointField;
}

void Field::Generate(float *& vertices, float speedFactor) {
	if (vertices == NULL)
		vertices = new float[fieldSize * 4];
	int index = 0;
	for (int i = 0; i < fieldSize; i++) {
        float time = 0;
        float fade = 1;
        if (pointField[i].checkIfCooling()) {
            pointField[i].CoolDown(x, y, time, fade);
        } else {
            SetPointPos(pointField[i], time, speedFactor);
        }
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

bool Field::SetEquations(std::string x, std::string y) {
	exprtk::symbol_table<float> symbol_table;
	symbol_table.add_variable("x", this->x);
	symbol_table.add_variable("y", this->y);
	expressionX.register_symbol_table(symbol_table);
	expressionY.register_symbol_table(symbol_table);
	exprtk::parser<float> parser;
	return parser.compile(x, expressionX) && parser.compile(y, expressionY);
}

void Field::SetPointPos(Point & a_point, float & time, float speedFactor) {
	a_point.getCurrPos(x, y, time);
	float tempX = speedFactor * expressionX.value();
	float tempY = speedFactor * expressionY.value();
	
	a_point.setCurrPos(tempX, tempY);
}

