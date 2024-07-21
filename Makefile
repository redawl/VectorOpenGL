COMMONFLAGS=-Iinclude
CFLAGS=$(COMMONFLAGS)
CXXFLAGS=$(COMMONFLAGS)
LDFLAGS=-lglfw -limgui
DESTDIR?=/usr
PREFIX?=/local
OBJECTS=src/main.o src/Shader.o src/Field.o src/Point.o src/glad/glad.o shader.vs.o shader.fs.o shader.gs.o
EXE=vog
INSTALL=install

.PHONY: clean install

shader.%.o: src/shader.%.S
	$(CC) -c $< -o $@

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXE)

install: $(EXE)
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	$(INSTALL) -m 0755 $(EXE) "$(DESTDIR)$(PREFIX)/bin"
