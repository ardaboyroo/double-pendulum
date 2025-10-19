#pragma once

#include "raylib.h"
#include "Settings.h"

class Trail
{
public:
	Trail();
	~Trail();

	void Draw();
	void DrawLine(Vector2 start, Vector2 end);
	void ResizeTextureArea(Vector2 size);

private:
	Settings& s;
	float previousWindowDiameter;
	RenderTexture trailTexture;
};
