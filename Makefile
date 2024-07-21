COMMONFLAGS=-Iinclude
CFLAGS=$(COMMONFLAGS)
CXXFLAGS=$(COMMONFLAGS)
LDFLAGS=-lglfw -limgui
DESTDIR?=/usr/local/bin
OBJECTS=src/main.o src/Shader.o src/Field.o src/Point.o src/glad/glad.o shader.vs.o shader.fs.o shader.gs.o
EXE=vog

.PHONY: clean

shader.%.o: src/shader.%.S
	$(CC) -c $< -o $@

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXE)

install: $(EXE)
	install -m 0755 $(EXE) $(DESTDIR)
