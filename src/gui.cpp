#include "gui.h"

Gui::Gui(GLFWwindow * window, int windowWidth, int windowHeight){
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Don't create imgui.ini
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    int padding = windowWidth / 216;

    int xPanelOffset = windowHeight + padding;
    int xPanelSize   = windowWidth - xPanelOffset - padding;

    this->functionWindowPos  = ImVec2(xPanelOffset, padding);
    this->functionWindowSize = ImVec2(xPanelSize, (windowHeight / 8) - padding - padding);

    this->optionsWindowPos   = ImVec2(xPanelOffset, this->functionWindowSize.y + padding + padding);
    this->optionsWindowSize  = ImVec2(xPanelSize, this->functionWindowSize.y * 2);

    this->infoWindowPos      = ImVec2(xPanelOffset, this->optionsWindowPos.y + this->optionsWindowSize.y + padding);
    this->infoWindowSize     = ImVec2(xPanelSize, windowHeight - this->functionWindowSize.y - this->optionsWindowSize.y - (padding * 4));

    this->fontSize = windowHeight / 1080;
}

Gui::~Gui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool Gui::functionBox(char * dx, char * dy){
    ImGui::Begin("Enter Functions (ESC to Close)");
    ImGui::SetWindowSize(functionWindowSize);
    ImGui::SetWindowPos(functionWindowPos);
    ImGui::SetWindowFontScale(fontSize);

    ImGui::InputText("dx", dx, 200);
    ImGui::InputText("dy", dy, 200);
    bool ret = ImGui::Button("Save");
    ImGui::End();
    return ret;
}

bool Gui::optionsBox(float & pixelSize, int & pixelCount, float & speed){
    ImGui::Begin("Options");
    ImGui::SetWindowSize(optionsWindowSize);
    ImGui::SetWindowPos(optionsWindowPos);
    ImGui::SetWindowFontScale(fontSize);
    ImGui::InputFloat("Pixel Size (Max 100)", &pixelSize, 0, 0);
    ImGui::InputInt("Number of Pixels (Max 40000)", &pixelCount, 0, 0);
    ImGui::InputFloat("Speed", &speed, 0, 0);

    bool ret = ImGui::Button("Update");

    ImGui::End();
    return ret;
}

void Gui::infoBox(){
    ImGui::Begin("Info");
    ImGui::SetWindowSize(infoWindowSize);
    ImGui::SetWindowPos(infoWindowPos);
    ImGui::SetWindowFontScale(fontSize);
    ImGui::End();
}

void Gui::begin(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}

void Gui::render(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
