#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>

struct ImVec2;

template <class T>
struct vec3;

template <class T>
struct vec2
{
	T x, y;

	constexpr vec2() { x = 0; y = 0; }
	constexpr vec2(const T& v) { x = v; y = v; }
	constexpr vec2(const T& a, const T& b) { x = a; y = b; }
	constexpr vec2(const T* a) { x = a[0]; y = a[1]; }
	constexpr vec2(const vec3<T>& v);

	//constexpr vec2(const float(*a)[2]) { x = a[0]; y = a[1];  }

	constexpr vec2(const ImVec2& b);
	constexpr operator ImVec2();
	constexpr operator ImVec2() const;

	constexpr vec2 operator+(const vec2& v) const { return { x + v.x, y + v.y }; }
	constexpr vec2 operator-(const vec2& v) const { return { x - v.x, y - v.y }; }
	constexpr vec2 operator*(const vec2& v) const { return { x * v.x, y * v.y }; }
	constexpr vec2 operator/(const vec2& v) const { return { x / v.x, y / v.y }; }
	constexpr void operator=(const vec2& v) { x = v.x; y = v.y; }
	constexpr void operator=(const vec3<T>& v);

	constexpr vec2 operator+(const T& v) const { return { x + v, y + v }; }
	constexpr vec2 operator-(const T& v) const { return { x - v, y - v }; }
	constexpr vec2 operator*(const T& v) const { return { x * v, y * v }; }
	constexpr vec2 operator/(const T& v) const { return { x / v, y / v }; }

	constexpr vec2 operator+=(const vec2& v) { return { x += v.x, y += v.y }; }
	constexpr vec2 operator-=(const vec2& v) { return { x -= v.x, y -= v.y }; }
	constexpr vec2 operator*=(const vec2& v) { return { x *= v.x, y *= v.y }; }
	constexpr vec2 operator/=(const vec2& v) { return { x /= v.x, y /= v.y }; }
	constexpr bool operator==(const vec2& v) const { return  x == v.x && y == v.y; }
	constexpr bool operator!=(const vec2& v) const { return  x != v.x || y != v.y; }
	constexpr bool operator>(const vec2& v) const { return  x > v.x && y > v.y; }
	constexpr bool operator<(const vec2& v) const { return  x < v.x && y < v.y; }
	constexpr bool operator>=(const vec2& v) const { return  x >= v.x && y >= v.y; }
	constexpr bool operator<=(const vec2& v) const { return  x <= v.x && y <= v.y; }

	constexpr vec2 operator+=(const T& v) { return { x += v, y += v }; }
	constexpr vec2 operator-=(const T& v) { return { x -= v, y -= v }; }
	constexpr vec2 operator*=(const T& v) { return { x *= v, y *= v }; }
	constexpr vec2 operator/=(const T& v) { return { x /= v, y /= v }; }


	constexpr T& operator[](const int index) const {
		return ((T*)&x)[index];
	}
	constexpr operator vec2<int>() { return { (int)x, (int)y }; }
	constexpr operator vec2<float>() { return { (float)x, (float)y }; }


	T mag() const {
		return std::sqrtf(x * x + y * y);
	}
	constexpr T dot(const vec2& vec) const
	{
		return x * vec.x + y * vec.y;
	}
	vec2<T> normalize() const  noexcept {
		vec2<T> r = *this;
		float len = this->mag();
		float ilength;

		if (len) {
			ilength = 1 / len;
			r.x *= ilength;
			r.y *= ilength;
		}

		return r;
	}
	vec2<T> normalize(float& len) const noexcept {
		vec2<T> r = *this;
		len = this->mag();
		float ilength;

		if (len) {
			ilength = 1 / len;
			r.x *= ilength;
			r.y *= ilength;
		}

		return r;
	}
	T dist(const vec2& vec) const
	{
		const vec2 sub = *this - vec;
		return sub.mag();
	}
	constexpr vec2 inverse() const
	{
		return { -x, -y };
	}
	constexpr float MagSq() const { //magnitude squared
		return (x * x + y * y);
	}
	constexpr vec2 clamp(const T min, const T max) {
		vec2<T> r;
		if (x < min)		r.x = min;
		else if (x > max)	r.x = max;

		if (y < min)		r.y = min;
		else if (y > max)	r.y = max;

		return r;
	}
	constexpr vec2 normalize360() const noexcept
	{
		return vec2
		{
			(360.0f / 65536) * ((int)(x * (65536 / 360.0f)) & 65535),
			(360.0f / 65536) * ((int)(y * (65536 / 360.0f)) & 65535),
		};
	}
	constexpr vec2 normalize180() const noexcept
	{
		vec3 angle = normalize360();
		for (int i = 0; i < 2; i++) {
			if (angle[i] > 180.0f) {
				angle[i] -= 360.0f;
			}
		}
		return angle;
	}
	//expects radians
	vec2 fromAngle(T angle) {
		return { std::cosf(static_cast<float>(angle)), std::cosf(static_cast<float>(angle)) };
	}
	constexpr vec2 smooth(const vec2& dst, float smoothingFactor) const noexcept {
		smoothingFactor = std::max(0.f, std::min(1.f, smoothingFactor));
		return *this + smoothingFactor * (dst - *this);
	}
	constexpr vec2 angular_distance(const vec2& other)
	{
		constexpr const auto ce_fmodf = [](float x, float y) {
			return (y != 0.0f) ? x - static_cast<int>(x / y) * y : 0.0f;
			};

		constexpr auto x_diff = ce_fmodf(other.x - x + 180, 360) - 180;
		constexpr auto y_diff = ce_fmodf(other.y - y + 180, 360) - 180;

		return vec2(x_diff, y_diff).normalize180();
	}
	template<typename Func, typename ... Args>
	constexpr vec2 for_each(Func func, Args... args) const {

		return vec2
		{
			func(x, std::forward<Args>(args)...),
			func(y, std::forward<Args>(args)...),
		};
	}
};

using ivec2 = vec2<int>;
using fvec2 = vec2<float>;

template <class T>
struct vec3
{

	T x, y, z;

	constexpr vec3() { x = 0; y = 0, z = 0; }
	constexpr vec3(const T& v) { x = v; y = v, z = v; }
	constexpr vec3(const T& a, const T& b, const T& c) { x = a; y = b, z = c; }
	constexpr vec3(const T(*a)[3]) { x = a[0]; y = a[1], z = a[2]; }
	constexpr vec3(const T* a) { x = a[0]; y = a[1], z = a[2]; }
	constexpr vec3(const vec2<T>& vec) { x = vec.x; y = vec.y; z = 0.f; }
	//constexpr explicit vec3(const vec3<int>& v) { x = (int)v.x, y = (int)v.y; }

	constexpr vec3 operator+(const vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	constexpr vec3 operator-(const vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }
	constexpr vec3 operator*(const vec3& v) const { return { x * v.x, y * v.y, z * v.z }; }
	constexpr vec3 operator/(const vec3& v) const { return { x / v.x, y / v.y, z / v.z }; }
	constexpr void operator=(const vec3& v) { x = v.x; y = v.y, z = v.z; }
	constexpr void operator=(const T* v) { x = v[0]; y = v[1], z = v[2]; }
	std::basic_ostream<char, std::char_traits<char>> operator<< (const vec3& v) {};

	constexpr vec3 operator+(T v) const { return { x + v, y + v, z + v }; }
	constexpr vec3 operator-(T v) const { return { x - v, y - v, z - v }; }
	constexpr vec3 operator*(T v) const { return { x * v, y * v, z * v }; }
	constexpr vec3 operator/(T v) const { return { x / v, y / v, z / v }; }

	constexpr vec3 operator+=(const vec3& v) { return { x += v.x, y += v.y, z += v.z }; }
	constexpr vec3 operator-=(const vec3& v) { return { x -= v.x, y -= v.y, z -= v.z }; }
	constexpr vec3 operator*=(const vec3& v) { return { x *= v.x, y *= v.y, z *= v.z }; }
	constexpr vec3 operator/=(const vec3& v) { return { x /= v.x, y /= v.y, z /= v.z }; }
	constexpr bool operator==(const vec3& v) const { return  x == v.x && y == v.y && z == v.z; }
	constexpr bool operator!=(const vec3& v) const { return  x != v.x || y != v.y || z != v.z; }
	constexpr bool operator>(const vec3& v) const { return  x > v.x && y > v.y && z > v.z; }
	constexpr bool operator<(const vec3& v) const { return  x < v.x && y < v.y && z < v.z; }
	constexpr bool operator>=(const vec3& v) const { return  x >= v.x && y >= v.y && z >= v.z; }
	constexpr bool operator<=(const vec3& v) const { return  x <= v.x && y <= v.y && z <= v.z; }

	constexpr vec3 operator+=(T v) { return { x += v, y += v, z += v }; }
	constexpr vec3 operator-=(T v) { return { x -= v, y -= v, z -= v }; }
	constexpr vec3 operator*=(T v) { return { x *= v, y *= v, z *= v }; }
	constexpr vec3 operator/=(T v) { return { x /= v, y /= v, z /= v }; }


#ifdef IMGUI_API

#endif
	constexpr operator vec3<int>() { return { (int)x, (int)y, (int)z }; }
	constexpr operator vec3<float>() { return { (float)x, (float)y, (float)z }; }
	constexpr operator vec2<int>() { return { (int)x, (int)y }; }
	constexpr operator vec2<float>() { return { (float)x, (float)y }; }
	constexpr operator T* () { return &x; }
	//constexpr operator const T* () const { return &x; }

	constexpr T& operator[](const int index) const {
		return ((T*)&x)[index];
	}

	T mag() const noexcept {
		return static_cast<T>(std::sqrtf(x * x + y * y + z * z));
	}
	constexpr T mag_sq() const noexcept {
		return (x * x + y * y + z * z);
	}
	constexpr T dot(const vec3& vec) const noexcept
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
	vec3 normalize() const  noexcept {
		vec3 r = *this;
		const float length = this->mag();
		float ilength;

		if (length) {
			ilength = 1 / length;
			r.x *= ilength;
			r.y *= ilength;
			r.z *= ilength;
		}

		return r;
	}
	vec3 normalize(float& len) const  noexcept {
		vec3 r = *this;
		len = this->mag();
		float ilength;

		if (len) {
			ilength = 1 / len;
			r.x *= ilength;
			r.y *= ilength;
			r.z *= ilength;
		}

		return r;
	}
	T dist(const vec3& vec) const noexcept
	{
		const vec3 sub = *this - vec;
		return sub.mag();
	}
	constexpr T dist_sq(const vec3& vec) const noexcept
	{
		const vec3 sub = *this - vec;
		return sub.mag_sq();
	}
	constexpr vec3 inverse() const noexcept
	{
		return { -x, -y, -z };
	}
	vec3 abs() const noexcept {

		constexpr auto cexpr_abs = [](const T v) {
			return v < static_cast<T>(0) ? -v : v;
			};

		return { cexpr_abs(x), cexpr_abs(y), cexpr_abs(z) };
	}
	constexpr vec3 clamp(const T min, const T max) noexcept {
		vec3<T> r;

		if (x < min)		r.x = min;
		else if (x > max)	r.x = max;

		if (y < min)		r.y = min;
		else if (y > max)	r.y = max;

		if (z < min)		r.z = min;
		else if (z > max)	r.z = max;

		return r;
	}
	vec3 toangles() const noexcept
	{
		float forward;
		float yaw, pitch;

		if (y == 0 && x == 0) {
			yaw = 0;
			if (z > 0) {
				pitch = 90;
			}
			else {
				pitch = 270;
			}
		}
		else {
			if (x) {
				yaw = (std::atan2f(y, x) * 180 / 3.14159265358979323846f);
			}
			else if (y > 0) {
				yaw = 90;
			}
			else {
				yaw = 270;
			}
			//if (yaw < 0) {
			//	yaw += 360;
			//}

			forward = sqrt(x * x + y * y);
			pitch = (atan2(z, forward) * 180 / 3.14159265358979323846f);
			if (pitch < 0) {
				pitch += 360;
			}
		}

		return vec3(-pitch, yaw, 0);
	}
	vec3 toforward() const noexcept {
		float angle;
		static float sp, sy, cp, cy;
		// static to help MS compiler fp bugs

		angle = y * (3.14159265358979323846f * 2 / 360);
		sy = sin(angle);
		cy = cos(angle);

		angle = x * (3.14159265358979323846f * 2 / 360);
		sp = sin(angle);
		cp = cos(angle);

		return vec3(cp * cy, cp * sy, -sp);

	}
	vec3 toright() const noexcept {
		float angle;
		static float sr, cr, sp, sy, cp, cy;
		// static to help MS compiler fp bugs

		angle = y * (3.14159265358979323846f * 2 / 360);
		sy = sin(angle);
		cy = cos(angle);

		angle = x * (3.14159265358979323846f * 2 / 360);
		sp = sin(angle);
		cp = cos(angle);

		angle = z * (3.14159265358979323846f * 2 / 360);
		sr = sin(angle);
		cr = cos(angle);

		return vec3((-1 * sr * sp * cy + -1 * cr * -sy), (-1 * sr * sp * sy + -1 * cr * cy), -1 * sr * cp);

	}
	vec3 toup() const noexcept {
		float angle;
		static float sr, cr, sp, sy, cp, cy;
		// static to help MS compiler fp bugs

		angle = y * (3.14159265358979323846f * 2 / 360);
		sy = sin(angle);
		cy = cos(angle);

		angle = x * (3.14159265358979323846f * 2 / 360);
		sp = sin(angle);
		cp = cos(angle);

		angle = z * (3.14159265358979323846f * 2 / 360);
		sr = sin(angle);
		cr = cos(angle);

		return vec3((cr * sp * cy + -sr * -sy), (cr * sp * sy + -sr * cy), cr * cp);

	}
	constexpr vec3 to_degrees()
	{
		constexpr float pi = 3.14159265358979323846f;
		return { x * (180.f / pi), y * (180.f / pi), z * (180.f / pi) };
	}
	constexpr vec3 normalize360() const noexcept
	{
		return vec3
		{
			(360.0f / 65536) * ((int)(x * (65536 / 360.0f)) & 65535),
			(360.0f / 65536) * ((int)(y * (65536 / 360.0f)) & 65535),
			(360.0f / 65536) * ((int)(z * (65536 / 360.0f)) & 65535),

		};
	}
	constexpr vec3 normalize180() const noexcept
	{
		vec3 angle = normalize360();
		for (int i = 0; i < 3; i++) {
			if (angle[i] > 180.0f) {
				angle[i] -= 360.0f;
			}
		}
		return angle;
	}
	constexpr vec3 angle_delta(const vec3& other) const noexcept
	{
		return (*this - other).normalize180();
	}
	constexpr vec3 smooth(const vec3& dst, float smoothingFactor) const noexcept {
		smoothingFactor = std::max(0.f, std::min(1.f, smoothingFactor));
		return *this + smoothingFactor * (dst - *this);
	}
	constexpr vec3 angular_distance(const vec3& other)
	{
		constexpr const auto ce_fmodf = [](float x, float y) {
			return (y != 0.0f) ? x - static_cast<int>(x / y) * y : 0.0f;
			};

		constexpr auto x_diff = ce_fmodf(other.x - x + 180, 360) - 180;
		constexpr auto y_diff = ce_fmodf(other.y - y + 180, 360) - 180;
		constexpr auto z_diff = ce_fmodf(other.z - z + 180, 360) - 180;

		return vec3(x_diff, y_diff, z_diff).normalize180();
	}

	template<typename Func, typename ... Args>
	constexpr vec3 for_each(Func func, Args... args) const {

		return vec3
		{
			func(x, std::forward<Args>(args)...),
			func(y, std::forward<Args>(args)...),
			func(z, std::forward<Args>(args)...)
		};
	}
	constexpr vec3<int> to_short() const noexcept
	{
		static_assert(std::is_floating_point_v<T>);
		constexpr const auto a2s = [](float x) {
			return (static_cast<int>((x) * 65536 / 360) & 65535);
			};

		return vec3<int>{a2s(x), a2s(y), a2s(z) };
	}
	constexpr vec3<float> from_short() const noexcept
	{
		static_assert(std::is_integral_v<T>);
		constexpr const auto s2a = [](int x) {
			return static_cast<float>((x) * (360.0f / 65536));
			};

		return vec3<float>{s2a(x), s2a(y), s2a(z) };
	}
};

using ivec3 = vec3<int>;
using fvec3 = vec3<float>;
inline std::ostream& operator<<(std::ostream& os, fvec3 const& v) {
	return os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
}
inline std::ostream& operator<<(std::ostream& os, ivec3 const& v) {
	return os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
}
inline std::ostream& operator<<(std::ostream& os, fvec2 const& v) {
	return os << "{ " << v.x << ", " << v.y << " }";
}
inline std::ostream& operator<<(std::ostream& os, ivec2 const& v) {
	return os << "{ " << v.x << ", " << v.y << " }";
}
struct Quaternion {
	float w;
	float x;
	float y;
	float z;
};

struct Pixel
{
	uint8_t r, g, b, a;

	constexpr Pixel() { r = 0, g = 0, b = 0, a = 255; }
	constexpr Pixel(const uint8_t val) { r = val, g = val, b = val, a = val; };
	constexpr Pixel(const uint8_t _r, const uint8_t _g, const uint8_t _b, const uint8_t _a) { r = _r, g = _g, b = _b, a = _a; }
	constexpr Pixel(const Pixel& p) { r = p.r, g = p.g, b = p.b, a = p.a; }

	Pixel operator=(const Pixel& px) {
		return { px.r, px.g, px.b, px.a };
	}

#ifdef IMGUI_API
	operator ImColor() { return ImColor(r, g, b, a); };
	operator unsigned int() { return IM_COL32(r, g, b, a); };

#endif
	unsigned int packed() const { return *(unsigned int*)&r; }

};

template<typename T>
constexpr vec2<T>::vec2(const vec3<T>& v)
{
	x = v.x;
	y = v.y;
}

template<typename T>
constexpr void vec2<T>::operator=(const vec3<T>& v)
{
	x = v.x;
	y = v.y;
}

