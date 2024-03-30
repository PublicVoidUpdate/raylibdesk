#include "raylib.h"
#include "raymath.h"
//#include <cstdio>

#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

using namespace std;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int taskbarwidth = GetMonitorWidth(0);
    const int taskbarheight = 50;
    const int desktopWidth = GetMonitorWidth(0);
    const int desktopHeight = GetMonitorHeight(0);
    const int startWidth = 300;
    const int startHeight = 400;

    // each window returns it's own context ID. You must save these to use them
    // each window must be inited separately
    int taskbarID = InitWindowPro(taskbarwidth, taskbarheight, "TASKBAR", FLAG_WINDOW_UNDECORATED+FLAG_WINDOW_TOPMOST);
    //int desktopID = 20;
    int desktopID = InitWindowPro(desktopWidth, desktopHeight, "raylib [core] example - window 2", FLAG_WINDOW_UNDECORATED+FLAG_WINDOW_MOUSE_PASSTHROUGH);
    int startmenuID = InitWindowPro(startWidth, startHeight, "START", FLAG_WINDOW_UNDECORATED+FLAG_WINDOW_TOPMOST);

    // before we can do anything we have to tell raylib what window context to use.
    // we will be swapping the active context all over the place.
    SetActiveWindowContext(taskbarID);
    SetWindowPosition(0, GetMonitorHeight(0) - taskbarheight);
    SetWindowFocused();

    Texture2D batpng = LoadTexture("./resources/bat.png");
    Texture2D startpng = LoadTexture("./resources/start.png");

    SetActiveWindowContext(desktopID);

    Texture2D wallpaper = LoadTexture("./resources/archtv.png");

    SetActiveWindowContext(startmenuID);
    SetWindowState(FLAG_WINDOW_HIDDEN);
    SetWindowFocused();



    bool taskbarOpen = true;
    bool desktopOpen = true;
    //true;

    bool startMenuOpen = true;

    bool toggleStart = false;

    Rectangle StartRec = {1, 1, 50, 50};

    // Main game loop

    while (taskbarOpen || desktopOpen || startMenuOpen) // run while any window is up
    {

        // Update



        // Draw
        // process windows backwards because the lowest window ID will call wait time.
        if (startMenuOpen)
        {
            SetActiveWindowContext(startmenuID);
            if (WindowShouldClose())
            {
                // we need to unload this texture before we kill the context
                CloseWindow();
                startMenuOpen = false;
            }
            else
            {
                if(toggleStart)
                {
                    

                    if(!IsWindowHidden())
                    {
                        SetWindowState(FLAG_WINDOW_HIDDEN);
                    }
                    else {
                        SetWindowPosition(0,GetMonitorHeight(0)-taskbarheight-startHeight);
                        ClearWindowState(FLAG_WINDOW_HIDDEN);
                        //SetWindowPosition(0,GetMonitorHeight(0)-taskbarheight-startHeight);
                    }
                    toggleStart = false;
                }
                BeginDrawing();

                ClearBackground(GREEN);
                EndDrawing();
            }
        }
        if (desktopOpen)
        {
            SetActiveWindowContext(desktopID);
            // do we want to close this window
            if (WindowShouldClose())
            {
                // we need to unload this texture before we kill the context
                UnloadTexture(wallpaper);
                CloseWindow();
                desktopOpen = false;
            }
            else
            {

                BeginDrawing();

                ClearBackground(BLACK);
                DrawTexturePro(wallpaper, {0, 0, wallpaper.width, wallpaper.height}, {0, 0, GetScreenWidth(), GetScreenHeight()}, {0, 0}, 0.0f, WHITE);
                DrawFPS(0,0);
                // DrawTexture(wallpaper, 0, 0, WHITE);

                DrawText("Desktop", 190, 200, 20, RAYWHITE);
                EndDrawing();
            }
        }

        // If window 1 is open, process it
        if (taskbarOpen)
        {
            SetActiveWindowContext(taskbarID);

            // check collisions against window 1 and the window size.
            // we need to do this in this context so it gets the correct window size.
            if (CheckCollisionPointRec(GetMousePosition(), StartRec))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){toggleStart = true;}
            }

            // do we want to close this window
            if (WindowShouldClose())
            {
                // we need to unload this texture before we kill the context
                UnloadTexture(batpng);
                UnloadTexture(startpng);
                CloseWindow();
                taskbarOpen = false;
            }
            else
            {
                // process this window
                BeginDrawing();

                ClearBackground(RAYWHITE);

                //Start button
                DrawTexturePro(startpng, {0,0,16,16}, StartRec, {0,0}, 0.0f, WHITE);
                //DrawRectangleRec(StartRec, BLACK);

                DrawRectangle(52,0,GetRenderWidth()-52-540,GetRenderHeight(),ORANGE);

                //time/date setup
                DrawRectangle(GetRenderWidth()-535,0,400,GetRenderHeight(),RED);
                auto end = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(end);
                DrawText(std::ctime(&end_time), GetRenderWidth()-530,GetRenderHeight()/2-15,30,BLACK);

                //battery text setup
                DrawRectangle(GetRenderWidth()-130,0,125,GetRenderHeight(),PURPLE);
                std::ifstream t("/sys/class/power_supply/BAT0/capacity");
                std::stringstream buffer;
                buffer << t.rdbuf();
                DrawText((buffer.str().erase(buffer.str().length()-1)+"%").c_str(), GetRenderWidth()-125, GetRenderHeight()/2-15, 30, BLACK);
                //bat image set
                int bat = stoi(buffer.str().erase(buffer.str().length()-1));
                int widthx = 0;
                if(bat > 24)
                {
                    if(bat > 49)
                    {if(bat > 74){if(bat =100){widthx = 4;}else{3;}} else{widthx = 2;}} else{widthx = 1;}
                } else{widthx = 0; }
                DrawTexturePro(batpng, {16*widthx,0,16,16}, {GetRenderWidth()-16*4 ,GetRenderHeight()/2-(16*4)/2,16*4,16*4}, {0,0}, 0.0f, WHITE);
                //DrawTexturePro(batpng, {16,0,16,16}, {GetRenderWidth()/2,0,16*4,16*4}, {0,0}, 0.0f, WHITE);
                //DrawTexture(batpng, 0,0,WHITE);
                // std::ifstream c("/sys/class/power_supply/BAT0/status");
                // std::stringstream buffer2;
                // buffer2 << c.rdbuf();
                // if((buffer2.str().erase(buffer2.str().length()-1)).c_str() != "Discharging")
                // {
                //     printf((buffer2.str().erase(buffer2.str().length()-1)).c_str());
                // }

                                
                EndDrawing();
            }
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // we shut down each window when it closed, so we have nothing left to cleanup

    return 0;
}