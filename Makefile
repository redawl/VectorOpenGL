COMMONFLAGS=-Iinclude -Wall
CFLAGS=$(COMMONFLAGS)
CXXFLAGS=$(COMMONFLAGS)
LDFLAGS=-lglfw -limgui
DESTDIR?=/usr
PREFIX?=/local
OBJECTS=src/main.o src/Shader.o src/Field.o src/Point.o src/glad/gl.o shader.vs.o shader.fs.o shader.gs.o
EXE=vog
INSTALL=install

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS)

shader.%.o: src/shader.%.S src/VectorShaders/shader.%
	$(CC) -c $< -o $@

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(EXE)

install: $(EXE)
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	$(INSTALL) -m 0755 $(EXE) "$(DESTDIR)$(PREFIX)/bin"

.PHONY: clean install
