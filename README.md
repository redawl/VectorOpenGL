# VectorOpenGL

VectorOpenGL models vector fields, based on an x differential equation, and a y differential equation. With these two inputs, it can create beautiful animated graphs:

![Image1](docs/imgs/graph.png)

## Dependencies
- glfw3
- opengl3
- imgui with glfw support enabled

## Building
With the above dependencies installed, building should be as simple as:
```bash
$ make clean vog
```

Then run with:
```bash
$ ./vog
```

## Credits
This project uses [dearImgui](https://github.com/ocornut/imgui) for the user interface, [GLAD](https://glad.dav1d.de/) as the OpenGL loader, and [exprTk](http://www.partow.net/programming/exprtk/) for equation parsing. 
