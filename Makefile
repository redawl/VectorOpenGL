COMMONFLAGS=-Iinclude
CFLAGS=$(COMMONFLAGS)
CXXFLAGS=$(COMMONFLAGS)
LDFLAGS=-lglfw -limgui
OBJECTS=src/main.o src/Shader.o src/Field.o src/Point.o src/glad/glad.o
EXE=vog

.PHONY: clean

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXE)
