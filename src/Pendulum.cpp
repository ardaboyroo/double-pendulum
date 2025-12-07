#include "Pendulum.h"

#include "Trail.h"
#include "raylib.h"

#include <cmath>
#include <iostream>

Pendulum::Pendulum()
	: s(Settings::Get()),
	a1(s.a1), a2(s.a2),
	a1_v(0), a2_v(0)
{
}

Pendulum::~Pendulum()
{
}

void Pendulum::Update()
{
	prev_x2 = x2;
	prev_y2 = y2;

	if (!s.pausePhysics)
		RK4Step(GetFrameTime());

	x1 = GetScreenWidth() / 2 + s.L1 * sin(a1);
	y1 = GetScreenHeight() / 2 + s.L1 * cos(a1);
	x2 = x1 + s.L2 * sin(a2);
	y2 = y1 + s.L2 * cos(a2);
}

void Pendulum::Draw()
{
	if (firstFramePassed)
	{
		t.AddLine({ (float)prev_x2, (float)prev_y2 }, { (float)x2, (float)y2 });
		t.Draw();
	}
	else firstFramePassed = true;

	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, s.bob1Thick, WHITE);

	DrawLineEx({ GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, { (float)x1, (float)y1 }, s.L1Thick, WHITE);
	DrawLineEx({ (float)x1,(float)y1 }, { (float)x2, (float)y2 }, s.L2Thick, WHITE);

	DrawCircle(x1, y1, s.bob2Thick, DARKBLUE);
	DrawCircle(x2, y2, s.bob3Thick, DARKGREEN);
}

void Pendulum::RK4Step(double dt)
{
	double da1_1, da2_1, dv1_1, dv2_1;
	double da1_2, da2_2, dv1_2, dv2_2;
	double da1_3, da2_3, dv1_3, dv2_3;
	double da1_4, da2_4, dv1_4, dv2_4;

	// k1
	Derivatives(a1, a2, a1_v, a2_v, da1_1, da2_1, dv1_1, dv2_1);

	// k2
	Derivatives(a1 + 0.5 * dt * da1_1, a2 + 0.5 * dt * da2_1,
		a1_v + 0.5 * dt * dv1_1, a2_v + 0.5 * dt * dv2_1,
		da1_2, da2_2, dv1_2, dv2_2);

	// k3
	Derivatives(a1 + 0.5 * dt * da1_2, a2 + 0.5 * dt * da2_2,
		a1_v + 0.5 * dt * dv1_2, a2_v + 0.5 * dt * dv2_2,
		da1_3, da2_3, dv1_3, dv2_3);

	// k4
	Derivatives(a1 + dt * da1_3, a2 + dt * da2_3,
		a1_v + dt * dv1_3, a2_v + dt * dv2_3,
		da1_4, da2_4, dv1_4, dv2_4);

	// Combine
	a1 += dt / 6.0 * (da1_1 + 2 * da1_2 + 2 * da1_3 + da1_4);
	a2 += dt / 6.0 * (da2_1 + 2 * da2_2 + 2 * da2_3 + da2_4);
	a1_v += dt / 6.0 * (dv1_1 + 2 * dv1_2 + 2 * dv1_3 + dv1_4);
	a2_v += dt / 6.0 * (dv2_1 + 2 * dv2_2 + 2 * dv2_3 + dv2_4);
}

void Pendulum::Derivatives(double a1, double a2, double a1_v, double a2_v, double& da1, double& da2, double& da1_v, double& da2_v)
{
	double a1_a, a2_a;
	ComputeAccelerations(a1, a2, a1_v, a2_v, a1_a, a2_a);
	da1 = a1_v;
	da2 = a2_v;
	da1_v = a1_a;
	da2_v = a2_a;
}

void Pendulum::ComputeAccelerations(double a1, double a2,
	double a1_v, double a2_v,
	double& a1_a, double& a2_a)
{
	double dt = GetFrameTime();
	double g = s.g;
	L1 = s.L1 / 100;
	L2 = s.L2 / 100;
	double m1 = s.m1;
	double m2 = s.m2;

	double num1 = -g * (2 * m1 + m2) * sin(a1);
	double num2 = -m2 * g * sin(a1 - 2 * a2);
	double num3 = -2 * sin(a1 - a2) * m2;
	double num4 = a2_v * a2_v * L2 + a1_v * a1_v * L1 * cos(a1 - a2);
	double den = L1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	a1_a = (num1 + num2 + num3 * num4) / den;

	num1 = 2 * sin(a1 - a2);
	num2 = (a1_v * a1_v * L1 * (m1 + m2));
	num3 = g * (m1 + m2) * cos(a1);
	num4 = a2_v * a2_v * L2 * m2 * cos(a1 - a2);
	den = L2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	a2_a = (num1 * (num2 + num3 + num4)) / den;
}
