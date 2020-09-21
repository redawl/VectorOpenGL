#include "Field.h"

Field::Field(int size) {
	pointField = new Point*[size];
	for (int i = 0; i < size; i++) {
		pointField[i] = new Point[size];
	}
	fieldSize = size;
}

Field::~Field() {
	for (int i = 0; i < fieldSize; i++){
		delete [] pointField[i];
	}
	delete [] pointField;
}

void Field::Generate(float *& vertices) {
	if (vertices == NULL)
		vertices = new float[fieldSize * fieldSize * 3];
	int index = 0;
	for (int i = 0; i < fieldSize; i++) {
		for (int j = 0; j < fieldSize; j++) {
			
			float x = 0;
			float y = 0;
			float time = 0;
			if (!pointField[i][j].checkIfCooling())
				pointField[i][j].getCurrPos(x, y, time);
			else
				pointField[i][j].CoolDown(x, y, time);
			vertices[index] = x;
			index++;
			vertices[index] = y;
			index++;
			vertices[index] = time;
			index++;
		}
	}
}

void Field::SetEquations(std::string x, std::string y) {
	equationX = x;
	equationY = y;
	for (int i = 0; i < fieldSize; i++) {
		for (int j = 0; j < fieldSize; j++) {
			pointField[i][j].setEquations(x, y);
		}
	}
}