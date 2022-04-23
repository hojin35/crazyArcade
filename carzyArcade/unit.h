#include<chrono>

struct Point
{
	int x = 0;
	int y = 0;
};

class User
{
public:
	User();
	Point GetPosition();
	void UserMoveX(bool dir);
	void UserMoveY(bool dir);
private:
	Point p_;
	double speed_ = 1;
};

class Bomb
{
public:
	Bomb(int x,int y, std::chrono::time_point<std::chrono::high_resolution_clock>& now);
	Bomb(Point p, std::chrono::time_point<std::chrono::high_resolution_clock> &now);
	Point GetPosition();
	int GetPassedTime(std::chrono::time_point<std::chrono::high_resolution_clock> &end);
private:
	Point p_;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

class Box
{
private:
	int item_;
};