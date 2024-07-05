#include "resolution.hpp"
#include "cg/cg_local.hpp"
#include "cg/cg_offsets.hpp"

ivec2 adjust_from_640x480(const fvec2 r)
{
	return { adjust_from_640(r.x), adjust_from_480(r.y) };
}
fvec2 adjust_to_640x480(const ivec2 r)
{
	return { adjust_to_640(r.x), adjust_to_480(r.y) };
}

int adjust_from_640(const float x)
{
	auto real_w = 1280;
	auto scaleX = real_w / 640.f;
	return static_cast<int>(x * scaleX);

}
int adjust_from_480(const float y)
{
	auto real_h = 720;
	auto scaleY = real_h / 480.f;
	return static_cast<int>(y * scaleY);
}
float adjust_to_640(const int x)
{
	auto real_w = 1280;
	auto scaleX = 640.f / real_w;
	return (static_cast<float>(x) * scaleX);

}
float adjust_to_480(const int y)
{
	auto real_h = 720;
	auto scaleY = 480.f / real_h;
	return (static_cast<float>(y) * scaleY);
}