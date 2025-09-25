#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

int main()
{
    InitWindow(800, 600, "Double Pendulum");
    SetWindowState(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_TRANSPARENT);
    SetTargetFPS(60);

    rlImGuiSetup(true); // init ImGui with Raylib

    float pendulumLength = 5.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 245, 245, 245, 150 });

        // TODO: pendulum rendering here...

        // ImGui overlay
        rlImGuiBegin();
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Pendulum Length", &pendulumLength, 1.0f, 10.0f);
        ImGui::End();
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
