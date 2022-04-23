#include "unit.h"


//user
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
int User::GetExplosionRange()
{
	return explosion_range_;
}

// Bomb
Bomb::Bomb(int x,int y, ch::time_point<ch::high_resolution_clock>& now,int explosion_range)
{
	p_.x = x;
	p_.y = y;
	start_ = now;
	explosion_range_ = explosion_range;
}
Point Bomb::GetPosition()
{
	return p_;
}
int Bomb::GetPassedTime(ch::time_point<ch::high_resolution_clock>& end)
{
	auto diff = end - start_;
	
	return ch::duration_cast<ch::milliseconds>(diff).count();
}
int Bomb::GetExplosionRange()
{
	return explosion_range_;
}