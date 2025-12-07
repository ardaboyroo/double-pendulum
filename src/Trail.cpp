#include "Trail.h"

Trail::Trail()
	: s(Settings::Get()),
	previousWindowDiameter(s.windowDiameter)
{
}

Trail::~Trail()
{
}

void Trail::Draw()
{
	int count = (int)trail.size();

	for (int i = 0; i < count; i++)
	{
		float t = (float)i / count;
		unsigned char alpha = (unsigned char)(t * (s.trailColor[3] * 255.f));

		Color faded = {
			s.trailColor[0] * 255.f,
			s.trailColor[1] * 255.f,
			s.trailColor[2] * 255.f,
			alpha
		};

		DrawLineEx(
			trail[i].start,
			trail[i].end,
			s.trailThickness,
			faded
		);
	}
}

// TODO: make it framerate independent, probably through minimum trail segment length
void Trail::AddLine(Vector2 start, Vector2 end)
{
	trail.push_back({ start, end });

	if ((int)trail.size() > s.maxTrailSegments)
		trail.pop_front();
}
