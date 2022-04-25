#pragma once
#include<chrono>
#include<SFML/Graphics.hpp>
#include<vector>
namespace ch = std::chrono;
class Point
{
public:
	int x = 0;
	int y = 0;
	bool operator==(const Point& p1)
	{
		if (p1.x != x)
			return false;
		if (p1.y != y)
			return false;
		return true;
	}
};
class User
{
public:
	User();
	Point GetPosition();
	void UserMoveX(bool dir);
	void UserMoveY(bool dir);
	int GetExplosionRange();
	void SetPosition(Point& p);
	void GainItems(int item);
private:
	Point p_;
	double speed_ = 1;
	int explosion_range_ = 1;
};

class Bomb
{
public:
	Bomb(int x, int y, std::chrono::time_point<ch::high_resolution_clock>& now, int explosion_range);
	Point GetPosition();
	int GetPassedTime(std::chrono::time_point<ch::high_resolution_clock>& end);
	int GetExplosionRange();
private:
	Point p_;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
	int explosion_range_ = 1;
};

class Box
{
private:
	int item_;
};