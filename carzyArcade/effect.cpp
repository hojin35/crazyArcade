#include "effect.h"

Wave::Wave(int x, int y, ch::time_point<ch::high_resolution_clock> start)
{
	p_.x = x;
	p_.y = y;
	start_ = start;
}
Wave::Wave(Point p, ch::time_point<ch::high_resolution_clock> start)
{
	p_.x = p.x;
	p_.y = p.y;
	start_ = start;
}
Point Wave::GetPosition()
{
	return p_;
}
int Wave::GetPassedTime(ch::time_point<ch::high_resolution_clock>& end)
{
	auto diff = end - start_;

	return ch::duration_cast<ch::milliseconds>(diff).count();
}

Boom::Boom(int x, int y, ch::time_point<ch::high_resolution_clock> start)
{
	p_.x = x;
	p_.y = y;
	start_ = start;
}
Boom::Boom(Point p, ch::time_point<ch::high_resolution_clock> start)
{
	p_.x = p.x;
	p_.y = p.y;
	start_ = start;
}
Point Boom::GetPosition()
{
	return p_;
}
int Boom::GetPassedTime(ch::time_point<ch::high_resolution_clock>& end)
{
	auto diff = end - start_;

	return ch::duration_cast<ch::milliseconds>(diff).count();
}