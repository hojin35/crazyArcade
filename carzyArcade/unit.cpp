#include "unit.h"
#include<chrono>
//User
User::User()
{
	p_.x = 10;
	p_.y = 10;
}
Point User::GetPosition()
{
	return User::p_;
}

void User::UserMoveX(bool dir)
{
	if (dir == 0)
		p_.x -= 10 * speed_;
	else
		p_.x += 10 * speed_;
}
void User::UserMoveY(bool dir)
{
	if (dir == 0)
		p_.y -= 10 * speed_;
	else
		p_.y += 10 * speed_;
}
// Bomb
Bomb::Bomb(int x,int y, std::chrono::time_point<std::chrono::high_resolution_clock>& now)
{
	p_.x = x;
	p_.y = y;
	start_ = now;
}
Bomb::Bomb(Point p,std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
	p_.x = p.x;
	p_.y = p.y;
	start_ = now;
}

Point Bomb::GetPosition()
{
	return p_;
}
int Bomb::GetPassedTime(std::chrono::time_point<std::chrono::high_resolution_clock>& end)
{
	auto diff = end - start_;
	
	return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
}