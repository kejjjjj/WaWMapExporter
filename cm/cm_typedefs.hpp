#pragma once

#include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utils/typedefs.hpp>
#include "global_macros.hpp"

struct sc_winding_t
{
	std::vector<fvec3> points;
	bool is_bounce = false;
	bool is_elevator = false;
	fvec3 normals;
	vec4_t color;

};
enum class showCollisionType
{
	DISABLED,
	BRUSHES,
	TERRAIN,
	BOTH
};
enum class polyType
{
	EDGES,
	POLYS,
};

enum class cm_geomtype
{
	brush,
	terrain,
	model
};

struct cplane_s;
struct cbrush_t;
struct cLeaf_t;

struct cm_triangle
{
	fvec3 a;
	fvec3 b;
	fvec3 c;



	vec4_t plane = {};
	vec4_t color = {};
	char* material = {};
	bool has_collision = false;
	//bool edge_walkable = true;

	fvec3 get_mins() const noexcept {
		fvec3 lowest = FLT_MAX;

		lowest.x = a.x;
		if (b.x < lowest.x) lowest.x = b.x;
		if (c.x < lowest.x) lowest.x = c.x;

		lowest.y = a.y;
		if (b.y < lowest.y) lowest.y = b.y;
		if (c.y < lowest.y) lowest.y = c.y;

		lowest.z = a.z;
		if (b.z < lowest.z) lowest.z = b.z;
		if (c.z < lowest.z) lowest.z = c.z;

		return lowest;

	}
	fvec3 get_maxs() const noexcept {
		fvec3 highest = -FLT_MAX;

		highest.x = a.x;
		if (b.x > highest.x) highest.x = b.x;
		if (c.x > highest.x) highest.x = c.x;

		highest.y = a.y;
		if (b.y > highest.y) highest.y = b.y;
		if (c.y > highest.y) highest.y = c.y;

		highest.z = a.z;
		if (b.z > highest.z) highest.z = b.z;
		if (c.z > highest.z) highest.z = c.z;

		return highest;

	}
};
struct cm_winding
{
	cm_winding() = default;
	cm_winding(const std::vector<fvec3>& p, const fvec3& normal, const fvec3& col);

	std::vector<fvec3> points;
	fvec3 mins;
	fvec3 maxs;
	fvec3 normals;
	vec4_t color;
	bool is_bounce = {};
	bool is_elevator = {};

private:

	inline fvec3 get_mins() const noexcept
	{
		std::vector<float> x, y, z;

		for (auto& p : points) {
			x.push_back(p.x);
			y.push_back(p.y);
			z.push_back(p.z);

		}

		const float _x = *std::min_element(x.begin(), x.end());
		const float _y = *std::min_element(y.begin(), y.end());
		const float _z = *std::min_element(z.begin(), z.end());

		return { _x, _y, _z };
	}
	inline fvec3 get_maxs() const noexcept
	{
		std::vector<float> x, y, z;

		for (auto& p : points) {
			x.push_back(p.x);
			y.push_back(p.y);
			z.push_back(p.z);

		}

		const float _x = *std::max_element(x.begin(), x.end());
		const float _y = *std::max_element(y.begin(), y.end());
		const float _z = *std::max_element(z.begin(), z.end());

		return { _x, _y, _z };
	}
};

struct cm_renderinfo
{
	cplane_s* frustum_planes = {};
	int num_planes = {};
	float draw_dist = {};
	bool depth_test = {};
	bool as_polygons = {};
	bool only_colliding = {};
	bool only_bounces = {};
	int only_elevators = {};
	float alpha = 0.7f;


};

class brushModelEntity;

struct cm_geometry
{
	virtual ~cm_geometry() = default;
	virtual void render(const cm_renderinfo& info) = 0;
	virtual cm_geomtype type() const noexcept = 0;
	virtual int map_export(std::stringstream& o, int index) = 0;
	virtual void render2d() = 0;
	fvec3 origin;
	bool has_collisions = {};
	int num_verts = {};
	brushModelEntity* brushmodel = 0;

};


struct cm_brush : public cm_geometry
{
	~cm_brush() = default;

	std::vector<cm_winding> windings; //used for rendering
	std::vector<cm_triangle> triangles; //used for exporting

	struct ele_corner {
		fvec3 mins;
		fvec3 maxs;
	};

	std::vector<const cm_winding*> corners;

	cbrush_t* brush = {};

	void create_corners();
	void render(const cm_renderinfo& info) override;
	cm_geomtype type() const noexcept override { return cm_geomtype::brush; }
	void render2d() override {}
protected:
	int map_export(std::stringstream& o, int index) override;
};

struct cm_terrain : public cm_geometry
{
	~cm_terrain() = default;

	void render(const cm_renderinfo& info) override;
	void render2d() override;

	//void sort_tree();

	const cLeaf_t* leaf = 0;
	std::vector<cm_triangle> tris;
	vec4_t color = {};
	char* material = {};

	cm_geomtype type() const noexcept override { return cm_geomtype::terrain; }

protected:
	int map_export(std::stringstream& o, int index) override;
	int map_export_triangle(std::stringstream& o, const cm_triangle& tri, int index) const;

private:
};

struct cm_model : public cm_geometry
{
	~cm_model() = default;

	void render([[maybe_unused]] const cm_renderinfo& info) override {};
	void render2d() override {};

	int map_export(std::stringstream& o, int index) override;


	const char* name = {};
	fvec3 origin;
	fvec3 angles;
	float modelscale = {};


	cm_geomtype type() const noexcept override { return cm_geomtype::model; }

};

void CM_LoadMap();
bool CM_IsMatchingFilter(const std::unordered_set<std::string>& filters, const char* material);


using geom_ptr = std::vector<std::unique_ptr<cm_geometry>>;
class CClipMap
{
public:

	static void insert(std::unique_ptr<cm_geometry>& geom) {

		if (geom)
			geometry.emplace_back(std::move(geom));

		wip_geom = nullptr;
	}
	static void insert(std::unique_ptr<cm_geometry>&& geom) {

		if (geom)
			geometry.emplace_back(std::move(geom));

		wip_geom = nullptr;
	}
	static void clear_type(const cm_geomtype t)
	{
		auto itr = std::remove_if(geometry.begin(), geometry.end(), [&t](std::unique_ptr<cm_geometry>& g)
			{
				return g->type() == t;
			});

		geometry.erase(itr, geometry.end());

	}
	static std::vector<geom_ptr::iterator> get_all_of_type(const cm_geomtype t)
	{
		std::vector<geom_ptr::iterator> r;

		for (auto b = geometry.begin(); b != geometry.end(); ++b)
		{
			if (b->get()->type() == t)
				r.push_back(b);
		}

		return r;
	}
	static auto begin() { return geometry.begin(); }
	static auto end() { return geometry.end(); }
	static size_t size() { return geometry.size(); }
	static void clear() { geometry.clear(); wip_geom.reset(); }
	static auto& get() { return geometry; }

	static std::unique_ptr<cm_geometry> wip_geom;
	static fvec3 wip_color;

private:
	static geom_ptr geometry;
};

void CM_LoadMap();

