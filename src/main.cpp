// sdl2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

// imgui
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl2.h"
#include "../include/imgui/imgui_impl_sdlrenderer2.h"

// stdlib
#include <vector>
#include <algorithm>
#include <string>

// internal
#include "spline.hpp"
#include "vectors.hpp"
#include "canvas.hpp"
#include "utils.hpp"
#include "main.hpp"
#include "tool_types.hpp"
#include "tool_manager.hpp"

SDL_Window *window;
SDL_Renderer *renderer;

ImGuiIO *imgui_io;

Canvas canvas;

float canvas_color_input[4] = {0.1, 0.1, 0.1, 1};
int canvas_color[4];

float min_canvas_scale = 0.2;
float max_canvas_scale = 5;

float brush_color_input[4] = {1, 1, 1, 1};
int brush_color[4];

float line_thickness = 1;

ToolManager tool_manager;

int last_mouse_x, last_mouse_y;
Vector2 mouse_position;
Vector2 pan_start;

bool app_active = true;
int screen_w, screen_h;

void Init()
{
    InitSDL();
    InitImGui();

    SDL_GetMouseState(&last_mouse_x, &last_mouse_y);
    SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);

    canvas = Canvas(renderer);
    tool_manager = ToolManager(&canvas);

    // converting and setting default brush and canvas colors
    DenormalizeRGBA(brush_color_input, brush_color);
    DenormalizeRGBA(canvas_color_input, canvas_color);
}

void InitSDL()
{
    // SDL init
    int res = SDL_Init(SDL_INIT_EVERYTHING);
    if (res != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    window = SDL_CreateWindow("Blackboard", // creates a window
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              1920, 1080,
                              SDL_WINDOW_RESIZABLE);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imgui_io = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void QuitApp()
{
    // ImGui shutdown
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // SDL shutdown
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();
}

void BuildGui()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();

    // MainMenuBar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Text("this is main menu");
            if (ImGui::Button("Open file"))                
                popen("zenity --file-selection", "r"); // probably only works for linux, will port to windows later
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Controls");
    if (ImGui::ColorEdit3("Brush color", brush_color_input))
    {
        DenormalizeRGBA(brush_color_input, brush_color);
        tool_manager.brush_color = ArrayToColor(brush_color);
    }

    if (ImGui::SliderFloat("Canvas scale", &canvas.scale, min_canvas_scale, max_canvas_scale))
        canvas.offset += Vector2(screen_w, screen_h) / 2 - canvas.ScreenToWorld(Vector2(screen_w, screen_h) / 2);

    if (ImGui::ColorEdit3("Background color", canvas_color_input))
        DenormalizeRGBA(canvas_color_input, canvas_color);

    ImGui::SliderFloat("Line thickness", &tool_manager.brush_size, 0.1, 25); 

    // Tool selection
    bool *toolbox_states = tool_manager.GetToolboxStates();
    for (size_t i = 0; i < ToolType::TOOL_COUNT; i++)
        if (ImGui::RadioButton(ToolTypeToString(ToolType(i)).c_str(), toolbox_states[i]))
            tool_manager.SetTool(ToolType(i));

    if (ImGui::Button("Recenter"))  // Recenter
    {
        canvas.offset = Vector2::GetZeroVector();
        canvas.scale = 1;
    }

    ImGui::End();
    ImGui::Render();
}

void HandleInputs()
{
    int mouse_x, mouse_y;
    int mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_position = Vector2(mouse_x, mouse_y);
    Vector2 original_mouse = canvas.ScreenToWorld(mouse_position);

    SDL_Event event;

    // ! this looks ugly af, need to do something about it
    while (SDL_PollEvent(&event))
    {
        if (event.type != SDL_MOUSEWHEEL) // imgui lags when using infinite scroll wheel and is stuck processing so many scrollwheen inputs, so i just block it
            ImGui_ImplSDL2_ProcessEvent(&event);

        // processing tool inputs
        if (!imgui_io->WantCaptureMouse)
            tool_manager.ExecuteTool(event);

        // app controls, for now just quitting
        switch (event.type)
        {
        case SDL_QUIT:
            app_active = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                app_active = false;
            break;

        default:
            break;
        }

        // canvas controls
        switch (event.type)
        {
        case SDL_KEYDOWN:   // UNDO
            if (event.key.keysym.sym != SDLK_z || !(event.key.keysym.mod & KMOD_LCTRL) || canvas.splines.size() == 0)
                break;

            canvas.splines.pop_back();
            break;

        // Panning setup
        case SDL_MOUSEBUTTONDOWN:   
            // if (imgui_io->WantCaptureMouse || mouse_buttons != 2)
            //     break;
            // printf("pan start\n");
            pan_start = mouse_position;
            break;
        
        // canvas zoom
        case SDL_MOUSEWHEEL:    
            canvas.scale *= 1 + 0.01 * event.wheel.y;
            canvas.scale = SDL_clamp(canvas.scale, min_canvas_scale, max_canvas_scale);
            canvas.offset += original_mouse - canvas.ScreenToWorld(mouse_position);
            break;

        // panning
        case SDL_MOUSEMOTION:   
            if (imgui_io->WantCaptureMouse || mouse_buttons != 2)
                break;

            canvas.offset -= (mouse_position - pan_start) / canvas.scale;
            pan_start = mouse_position;
            break;

        default:
            break;
        }
    }
}

void Render()
{
    SDL_SetRenderDrawColor(renderer, canvas_color[0], canvas_color[1], canvas_color[2], canvas_color[3]);
    SDL_RenderClear(renderer);  // Clearing screen
    canvas.Render(); // rendering canvas
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData()); // imgui render
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
}

int main(int argc, char *argv[])
{
    Init();

    while (app_active)
    {
        HandleInputs();
        BuildGui();
        Render();
    }

    QuitApp();
    return 0;
}

