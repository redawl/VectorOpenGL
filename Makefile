COMMONFLAGS=-Iinclude -Wall
CFLAGS=$(COMMONFLAGS)
CXXFLAGS=$(COMMONFLAGS)
LDFLAGS=-lglfw -limgui
DESTDIR?=/usr
PREFIX?=/local
OBJECTS=src/main.o src/shader.o src/field.o src/point.o src/glad/gl.o src/shader.vs.o src/shader.fs.o src/shader.gs.o src/gui.o
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
