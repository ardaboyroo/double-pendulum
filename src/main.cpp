#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"
#include "imgui.h"

#include "Settings.h"
#include "Pendulum.h"

#include <print>
#include <cmath>
#include <iostream>

void UpdateDraggableWindow();
bool IsMouseDoubleClicked(int button, float maxDelay = 0.3f);

int main()
{
	Settings& s = Settings::Get();
	s.Init();

	Pendulum myPendulum;

	float fullPendulumRadius = s.L1 + s.L2 + s.bob2Thick;
	s.SetWindowSize({ fullPendulumRadius * 2, fullPendulumRadius * 2 });

	while (!WindowShouldClose() && s.running)
	{
		BeginDrawing();
		ClearBackground(Color{ 0, 0, 0, 0 });

		/////////////////
		// Main
		/////////////////
		myPendulum.Update();
		myPendulum.Draw();

		if (IsKeyPressed(KEY_SPACE) || (IsMouseDoubleClicked(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) && !s.isOpen)
			Settings::Get().isOpen = !Settings::Get().isOpen;

		if (sqrt(GetMousePosition().x * (s.windowDiameter / 2.f) + GetMousePosition().y * (s.windowDiameter / 2.f)) <= s.bob1Thick)
			SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);

		UpdateDraggableWindow();

		float pendulumRadius = s.L1 + s.L2 + s.bob2Thick;

		if ((pendulumRadius * 2.f) + 50.0f > s.windowDiameter + 40.f)
		{
			// pendulum too big -> increase window
			float radius = std::fmax(s.settingsSize.x / 2.0f, pendulumRadius + 25.0f);
			s.SetWindowSize({ radius * 2.0f, radius * 2.0f });
		}
		else if (s.windowDiameter - (pendulumRadius * 2.f) > 80.0f && s.windowDiameter > s.settingsSize.x)
		{
			// pendulum comfortably smaller -> decrease window
			float radius = std::fmax(s.settingsSize.x / 2.0f, pendulumRadius + 25.0f);
			s.SetWindowSize({ radius * 2.0f, radius * 2.0f });
		}

		/////////////////
		// ImGui overlay
		/////////////////
		rlImGuiBegin();
		Settings::Get().RenderImgui();
		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
	return 0;
}

void UpdateDraggableWindow()
{
	static bool dragging = false;
	static Vector2 dragOffset = { 0 };
	static Vector2 startPos = { 0 };

	Vector2 mouse = GetMousePosition();
	Vector2 screenCenter = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	float distance = Vector2Distance(mouse, screenCenter);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (!Settings::Get().isOpen && distance <= 20 || mouse.y < GetScreenHeight()/2 - 230))
	{
		Vector2 windowPos = GetWindowPosition();
		Vector2 mouse = GetMousePosition();

		// convert mouse to screen space
		Vector2 mouseScreen = { windowPos.x + mouse.x, windowPos.y + mouse.y };

		dragOffset = Vector2Subtract(mouseScreen, windowPos);
		dragging = true;
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		dragging = false;

	if (dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Vector2 windowPos = GetWindowPosition();
		Vector2 mouse = GetMousePosition();
		Vector2 mouseScreen = { windowPos.x + mouse.x, windowPos.y + mouse.y };

		Vector2 newPos = Vector2Subtract(mouseScreen, dragOffset);
		SetWindowPosition((int)newPos.x, (int)newPos.y);
	}
}

// TODO: place in a util file

bool IsMouseDoubleClicked(int button, float maxDelay)
{
	static double lastClickTime[3] = { 0.0 }; // support for left/middle/right buttons

	if (IsMouseButtonPressed(button))
	{
		double now = GetTime();
		double delta = now - lastClickTime[button];

		lastClickTime[button] = now;

		if (delta < maxDelay)
			return true;
	}

	return false;
}