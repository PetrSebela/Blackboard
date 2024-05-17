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

// internal
#include "spline.hpp"
#include "vectors.hpp"
#include "canvas.hpp"
#include "utils.hpp"


// sdl variables
SDL_Window *window;
SDL_Renderer *renderer;

// canvas
Canvas canvas;
std::vector<Spline> canvas_objects;

// colors
float canvas_color_input[4] = {0.1, 0.1, 0.1, 1};
int canvas_color[4];

float brush_color_input[4] = {1, 1, 1, 1};
int brush_color[4];

float line_thickness = 1;

// mouse
int last_mouse_x, last_mouse_y;
Vector2 mouse_position;
Vector2 pan_start;

// app
bool app_active = true;

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
}

ImGuiIO &InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &imgui_io = ImGui::GetIO();
    (void)imgui_io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    imgui_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Enable docking
    imgui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    return imgui_io;
}

void HandleInputs(ImGuiIO &imgui_io)
{
    Vector2 original_mouse = canvas.ScreenToWorld(mouse_position);
    SDL_Event event;


    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            app_active = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                app_active = false;

            else if (event.key.keysym.sym == SDLK_z && event.key.keysym.mod & KMOD_LCTRL)
            {
                printf("Undo\n");
                if (canvas_objects.size() > 0)
                {
                    canvas_objects.pop_back();
                }   
            }
            
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (imgui_io.WantCaptureMouse)
                break;

            if (event.button.button == 1 )
            {
                Spline sp;
                sp.setColor(brush_color);
                sp.thickness = line_thickness;
                canvas_objects.push_back(sp);
            }
            
            else if (event.button.button == 2)
                pan_start = Vector2(event.motion.x, event.motion.y);
            break;

        case SDL_MOUSEBUTTONUP:
            // if (canvas_objects[canvas_objects.size()-1].points.size() <= 1)
                // canvas_objects.pop_back();
            
            break;

        case SDL_MOUSEWHEEL:
            canvas.scale *= 1 + 0.01 * event.wheel.y;
            canvas.offset += original_mouse - canvas.ScreenToWorld(mouse_position);
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                // window is rezised
                continue;
            break;
        }
    }       
}

int main(int argc, char *argv[])
{
    InitSDL();
    ImGuiIO &imgui_io = InitImGui();


    SDL_GetMouseState(&last_mouse_x, &last_mouse_y);
    
    canvas = Canvas(renderer);
    
    int screen_w, screen_h;
    SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);
    DenormalizeRGBA(brush_color_input, brush_color);
    DenormalizeRGBA(canvas_color_input, canvas_color);

    while (app_active)
    {
        // Events handeling
        int mouse_x, mouse_y;
        int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        mouse_position = Vector2(mouse_x, mouse_y);        

        HandleInputs(imgui_io);

        // canvas panning
        if (buttons == 2 && !imgui_io.WantCaptureMouse)
        {
            canvas.offset -= (mouse_position - pan_start) / canvas.scale ;
            pan_start = Vector2(mouse_x, mouse_y);
        }

        // draw line
        else if (buttons == 1 && !imgui_io.WantCaptureMouse)
        {
            Vector2 point(mouse_x, mouse_y);
            point = canvas.ScreenToWorld(point);
            canvas_objects[canvas_objects.size() - 1].addPoint(point);
        }

        // building imgui gui
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();  

        // main menu
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                ImGui::Text("this is main menu");
                if (ImGui::Button("Open file"))
                {
                    // probably only works for linux, will port to windows later
                    popen("zenity --file-selection","r");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("Controls");
        if(ImGui::ColorEdit3("Brush color", brush_color_input))
            DenormalizeRGBA(brush_color_input, brush_color);

        if(ImGui::SliderFloat("Canvas scale", &canvas.scale, 0.1, 2))
            canvas.offset += Vector2(screen_w, screen_h) / 2 - canvas.ScreenToWorld(Vector2(screen_w, screen_h) / 2);
        
        if(ImGui::ColorEdit3("Background color", canvas_color_input))
            DenormalizeRGBA(canvas_color_input, canvas_color);

        ImGui::SliderFloat("Line thickness", &line_thickness, 0.001, 100);
        
        ImGui::End();
        ImGui::Render();

        
        // --- Rendering ---
        // Clear screen
        SDL_SetRenderDrawColor(renderer, canvas_color[0], canvas_color[1], canvas_color[2], canvas_color[3]);
        SDL_RenderClear(renderer);

        // draw canvas_objects
        for (Spline s : canvas_objects)
            s.render(&canvas);

        // draw ImGui
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        // push to screen
        SDL_RenderPresent(renderer);

        // delay to cap framerate to 60FPS ( add dynamic later )
        SDL_Delay(1000 / 60);
    }


    // ImGui shutdown
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // SDL shutdown
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();

    return 0;
}