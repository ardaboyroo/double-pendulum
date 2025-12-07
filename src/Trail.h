#pragma once

#include "raylib.h"
#include "Settings.h"

#include <deque>

struct TrailLine
{
	Vector2 start;
	Vector2 end;
};

class Trail
{
public:
	Trail();
	~Trail();

	void Draw();
	void AddLine(Vector2 start, Vector2 end);

private:
	Settings& s;
	float previousWindowDiameter;
	std::deque<TrailLine> trail;
};
