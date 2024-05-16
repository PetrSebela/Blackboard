#ifndef MAIN_H
#define MAIN_H
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl2.h"
#include "../include/imgui/imgui_impl_sdlrenderer2.h"

void InitSDL();
ImGuiIO &InitImGui();
void HandleInputs(ImGuiIO &imgui_io);
int main(int argc, char *argv[]);

#endif MAIN_H