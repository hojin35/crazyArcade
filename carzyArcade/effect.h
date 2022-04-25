#pragma once
#include "unit.h"
class Wave
{
public:
	Wave(int x,int y, ch::time_point<ch::high_resolution_clock> start);
	Wave(Point p, ch::time_point<ch::high_resolution_clock> start);
	Point GetPosition();
	int GetPassedTime(ch::time_point<ch::high_resolution_clock>& end);

private:
	Point p_;
	ch::time_point<ch::high_resolution_clock> start_;
};

class Boom
{
public:
	Boom(int x,int y, ch::time_point<ch::high_resolution_clock> start);
	Boom(Point p, ch::time_point<ch::high_resolution_clock> start);
	Point GetPosition();
	int GetPassedTime(ch::time_point<ch::high_resolution_clock>& end);

private:
	Point p_;
	ch::time_point<ch::high_resolution_clock> start_;
};