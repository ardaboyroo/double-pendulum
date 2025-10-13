#pragma once

#include "raylib.h"

// Singleton
class Settings
{
private:
	Settings();

	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;
	Settings(Settings&&) = delete;
	Settings& operator=(Settings&&) = delete;

public:
	~Settings();

	static Settings& Get();

	void Init();
	void RenderImgui();
	void SetWindowSize(Vector2 size);

private:
	void LoadSettingsFromFile();
	void SaveSettingsToFile();

public:
	// App Settings
	bool running = true;

	// Window Settings
	bool isOpen = false;
	bool alwaysOnTop = true;
	int targetFps = 60;
	float windowDiameter = 640.f;

	// Pendulum Settings
	float L1 = 150.0f;
	float L2 = 150.0f;
	float m1 = 10.0f;
	float m2 = 10.0f;
	float a1 = PI / 2; // TODO: randomize init position
	float a2 = PI / 2 - 0.1f; // TODO: randomize init position

	float L1Thick = 2.f;
	float L2Thick = 2.f;
	float bob1Thick = 20.f;
	float bob2Thick = 20.f;
	float bob3Thick = 20.f;

	// Physics settings
	bool pausePhysics = false;
	float g = 9.81f;

	// ImGui Settings
	Vector2 settingsSize{500, 500};

private:
	bool runOnStartup = false;
};

