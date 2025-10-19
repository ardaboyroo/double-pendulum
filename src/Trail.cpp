#include "Trail.h"

#include <iostream>

Trail::Trail()
	: s(Settings::Get()),
	previousWindowDiameter(s.windowDiameter),
	trailTexture(LoadRenderTexture(s.windowDiameter, s.windowDiameter))
{
	BeginTextureMode(trailTexture);
	ClearBackground(BLANK);
	EndTextureMode();
}

Trail::~Trail()
{
	UnloadRenderTexture(trailTexture);
}

void Trail::Draw()
{
	DrawTextureRec(trailTexture.texture, 
		{0, 0, (float)trailTexture.texture.width, -(float)trailTexture.texture.height}, 
		{0, 0}, WHITE);

	BeginTextureMode(trailTexture);
	DrawRectangle(0, 0, s.windowDiameter, s.windowDiameter, Fade(BLACK, 0.02f));
	EndTextureMode();
}

void Trail::DrawLine(Vector2 start, Vector2 end)
{
	BeginTextureMode(trailTexture);
	DrawLineEx(start, end, 2.f, RED);
	EndTextureMode();
}

void Trail::ResizeTextureArea(Vector2 size)
{
	std::cout << "loading new texturesize: " << size.x;
	RenderTexture newTex = LoadRenderTexture(size.x, size.y);
	BeginTextureMode(newTex);
	ClearBackground(BLANK);
	DrawTextureRec(trailTexture.texture, 
		{ 0, 0, (float)trailTexture.texture.width, -(float)trailTexture.texture.height }, 
		{ (s.windowDiameter - previousWindowDiameter) / 2.f, (s.windowDiameter - previousWindowDiameter)/2.f}, WHITE);
	EndTextureMode();

	UnloadRenderTexture(trailTexture);
	trailTexture = newTex;
	previousWindowDiameter = s.windowDiameter;
}
