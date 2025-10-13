#pragma once

#include "Settings.h"

class Pendulum
{
public:
	Pendulum();
	~Pendulum();

	void Update();
	void Draw();

private:
	void RK4Step(double dt);

	void Derivatives(double a1, double a2, double a1_v, double a2_v,
		double& da1, double& da2, double& da1_v, double& da2_v);

	void ComputeAccelerations(double a1, double a2,
		double a1_v, double a2_v,
		double& a1_a, double& a2_a);

private:
	Settings& s;

	double a1, a2;	// angles of the pendulum bobs
	double a1_v, a2_v;	// angular velocities of the bobs

	double x1, y1;
	double x2, y2;

	double L1, L2;
};

