#ifndef GUI_H
#define GUI_H
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class Gui {
    public:
        Gui(GLFWwindow * window, int windowWidth, int windowHeight);
        ~Gui();

        bool functionBox(char * dx, char * dy);
        bool optionsBox(float & pixelSize, int & pixelCount, float & speed);
        void infoBox();
        void begin();
        void render();

    private:
        ImVec2 functionWindowPos;
        ImVec2 functionWindowSize;
        ImVec2 optionsWindowPos;
        ImVec2 optionsWindowSize;
        ImVec2 infoWindowPos;
        ImVec2 infoWindowSize;
        int fontSize = 0;
};
#endif

