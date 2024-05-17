#ifndef MAIN_H
#define MAIN_H
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl2.h"
#include "../include/imgui/imgui_impl_sdlrenderer2.h"


// init methods
void Init();

void InitSDL();

ImGuiIO &InitImGui();

// quit
void QuitApp();

void SelectTool(int toolIndex);

void HandleInputs();

void BuildGui(int screen_w, int screen_h);

int main(int argc, char *argv[]);

#endif