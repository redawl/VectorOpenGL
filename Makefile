COMMONFLAGS=-Iinclude
CFLAGS=$(COMMONFLAGS)
CXXFLAGS=$(COMMONFLAGS)
LDFLAGS=-lglfw -limgui
OBJECTS=src/main.o src/Shader.o src/Field.o src/Point.o src/glad/glad.o
SHADER_OBJECTS=shader.vs.o shader.fs.o shader.gs.o
EXE=vog

.PHONY: clean

shader.%.o: src/shader.%.S
	$(CC) -c $< -o $@

$(EXE): $(OBJECTS) $(SHADER_OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(SHADER_OBJECTS) -o $(EXE) $(LDFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXE)

