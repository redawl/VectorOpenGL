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
			pointField[i][j].getCurrPos(x, y, time);
			vertices[index] = x;
			index++;
			vertices[index] = y;
			index++;
			vertices[index] = time;
			index++;
		}
	}
}