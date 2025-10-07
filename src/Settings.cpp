#include "Settings.h"

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "win_wrapper.h"

#include <iostream>
#include <print>
#include <string>


Settings::Settings()
{
	LoadSettingsFromFile();
}

Settings::~Settings()
{
	SaveSettingsToFile();
}

Settings& Settings::Get()
{
	static Settings instance;
	return instance;
}

void Settings::Init()
{
	std::print("init");
	SetConfigFlags(
		(alwaysOnTop ? FLAG_WINDOW_TOPMOST : 0)
		| FLAG_WINDOW_TRANSPARENT
		| FLAG_WINDOW_UNDECORATED
		| FLAG_VSYNC_HINT
	);

	InitWindow(windowRadius * 2, windowRadius * 2, "Double Pendulum");
	rlImGuiSetup(true);

	Win_EnableLayered(GetWindowHandle());
	UpdateCachedFrame(LoadImageFromScreen, UnloadImage);
	SetWndProc(GetWindowHandle(), (PFN_LoadImgFromScrn)LoadImageFromScreen, (PFN_GetImgClr)GetImageColor, (PFN_UnloadImg)UnloadImage);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(10, 10, 10, .1f);
	style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
	style.Colors[ImGuiCol_Header] = ImVec4(0.11f, 0.22f, 0.34f, 1);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.49f, 0.8f, 1);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.1f, 0.1f, 1);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1);
}

void Settings::RenderImgui()
{
	if (!isOpen) return;

	auto& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	ImGui::SetNextWindowPos({ (io.DisplaySize.x - 500) * 0.5f,
		(io.DisplaySize.y - 500) * 0.5f });

	ImGui::Begin("Settings", &isOpen,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
	);

	ImGui::Text(("FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str());
	if(ImGui::Button("Exit App")) running = false;

	if (ImGui::CollapsingHeader("Pendulum Settings", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
	{
		ImGui::InputFloat("Length 1", &L1, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat("Length 2", &L2, 1.0f, 100.0f, "%.1f");

		ImGui::InputFloat("Mass 1", &m1, 1.0f, 20.0f, "%.1f");
		ImGui::InputFloat("Mass 2", &m2, 1.0f, 20.0f, "%.1f");

		ImGui::InputFloat("Length 1 Thickness", &L1Thick, 1.0f, 5.0f, "%.1f");
		ImGui::InputFloat("Length 2 Thickness", &L2Thick, 1.0f, 5.0f, "%.1f");

		ImGui::InputFloat("Bob 1 Thickness", &bob1Thick, 1.0f, 10.0f, "%.1f");
		ImGui::InputFloat("Bob 2 Thickness", &bob2Thick, 1.0f, 10.0f, "%.1f");
	}

	if (ImGui::CollapsingHeader("Physics Settings"))
	{
		ImGui::Checkbox("Pause Physics", &pausePhysics);
	}

	ImGui::End();
}

void Settings::SetWindowSize(Vector2 size)
{
	std::cout << "Before position: " << GetWindowPosition().x << "  " << GetWindowPosition().y << "\n";
	std::cout << "Before size: " << windowRadius << "  " << windowRadius << "\n";
	Vector2 displayPos = ::GetWindowPosition();
	Vector2 delta = { size.x - windowRadius, size.y - windowRadius };
	windowRadius = size.x;

	// Move window first
	//::SetWindowPosition(displayPos.x - delta.x / 2, displayPos.y - delta.y / 2);

	// Then resize
	::CustomSetWindowSize(GetWindowHandle(), size.x, size.y);
	std::cout << "After position: " << GetWindowPosition().x << "  " << GetWindowPosition().y << "\n";
	std::cout << "After size: " << size.x << "  " << size.y << "\n";
}

void Settings::LoadSettingsFromFile()
{
}

void Settings::SaveSettingsToFile()
{
}
