#ifndef MAIN_H
#define MAIN_H
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl2.h"
#include "../include/imgui/imgui_impl_sdlrenderer2.h"

void Init();

void InitSDL();

void InitImGui();

void QuitApp();

void HandleInputs();

void BuildGui();

std::string SelectFilePath();

int main(int argc, char *argv[]);



#endif