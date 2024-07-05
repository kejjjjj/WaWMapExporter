#include "cm_brush.hpp"
#include "cm_typedefs.hpp"
#include "cg/cg_local.hpp"
#include <iostream>
#include <com/com_vector.hpp>
#include <cg/cg_offsets.hpp>

SimplePlaneIntersection pts[1024];
SimplePlaneIntersection* pts_results[1024];

void CM_GetBrushWindings(cbrush_t* brush)
{
	if (!brush)
		return;

	CClipMap::wip_geom = CM_GetBrushPoints(brush, { 0.f, 1.f, 0.f });
	CClipMap::insert(CClipMap::wip_geom);

}
std::unique_ptr<cm_geometry> CM_GetBrushPoints(cbrush_t* brush, const fvec3& poly_col)
{
	if (!brush)
		return nullptr;

	float outPlanes[40][4]{};
	int planeCount = BrushToPlanes(brush, outPlanes);
	int intersections = GetPlaneIntersections((const float**)outPlanes, planeCount, pts);
	adjacencyWinding_t windings[40]{};

	int intersection = 0;
	int num_verts = 0;

	CClipMap::wip_geom = std::make_unique<cm_brush>();
	CClipMap::wip_color = poly_col;

	auto c_brush = dynamic_cast<cm_brush*>(CClipMap::wip_geom.get());

	c_brush->brush = brush;
	c_brush->origin = fvec3(brush->mins) + ((fvec3(brush->maxs) - fvec3(brush->mins)) / 2);

	do {
		auto w = BuildBrushAdjacencyWindingForSide(intersections, (char*)"lol", outPlanes[intersection], intersection, pts, &windings[intersection]);
		if (w) {
			//std::cout << w->numsides << '\n';
			num_verts += w->numsides;
		}
		++intersection;
	} while (intersection < planeCount);

	c_brush->num_verts = num_verts;
	c_brush->create_corners();

	return std::move(CClipMap::wip_geom);

}
void CM_BuildAxialPlanes(float(*planes)[6][4], const cbrush_t* brush)
{

	(*planes)[0][0] = -1.0;
	(*planes)[0][1] = 0.0;
	(*planes)[0][2] = 0.0;
	(*planes)[0][3] = -brush->mins[0];
	(*planes)[1][0] = 1.0;
	(*planes)[1][1] = 0.0;
	(*planes)[1][2] = 0.0;
	(*planes)[1][3] = brush->maxs[0];
	(*planes)[2][0] = 0.0;
	(*planes)[2][2] = 0.0;
	(*planes)[2][1] = -1.0;
	(*planes)[2][3] = -brush->mins[1];
	(*planes)[3][0] = 0.0;
	(*planes)[3][2] = 0.0;
	(*planes)[3][1] = 1.0;
	(*planes)[3][3] = brush->maxs[1];
	(*planes)[4][0] = 0.0;
	(*planes)[4][1] = 0.0;
	(*planes)[4][2] = -1.0;
	(*planes)[4][3] = -brush->mins[2];
	(*planes)[5][0] = 0.0;
	(*planes)[5][1] = 0.0;
	(*planes)[5][2] = 1.0;
	(*planes)[5][3] = brush->maxs[2];
}
void CM_GetPlaneVec4Form(const cbrushside_t* sides, const float(*axialPlanes)[4], int index, float* expandedPlane)
{
	if (index >= 6) {
		cplane_s* plane = sides[index - 6].plane;

		expandedPlane[0] = plane->normal[0];
		expandedPlane[1] = plane->normal[1];
		expandedPlane[2] = plane->normal[2];
		expandedPlane[3] = plane->dist;
		return;
	}

	const float* plane = axialPlanes[index];

	*expandedPlane = plane[0];
	expandedPlane[1] = plane[1];
	expandedPlane[2] = plane[2];
	expandedPlane[3] = plane[3];

}
int GetPlaneIntersections(const float** planes, int planeCount, SimplePlaneIntersection* OutPts)
{
	int r = 0;
	__asm
	{
		push OutPts;
		push planeCount;
		push planes;
		mov esi, 0x5EBDB0;
		call esi;
		add esp, 12;
		mov r, eax;
	}

	return r;
}
int BrushToPlanes(const cbrush_t* brush, float(*outPlanes)[4])
{
	float planes[6][4]{};
	CM_BuildAxialPlanes((float(*)[6][4])planes, brush);
	uint32_t i = 0;
	do {
		CM_GetPlaneVec4Form(brush->sides, planes, i, outPlanes[i]);

	} while (++i < brush->numsides + 6);

	return i;
}
adjacencyWinding_t* BuildBrushAdjacencyWindingForSide(int ptCount, char* collMap, float* normals, int planeIndex, SimplePlaneIntersection* spi, adjacencyWinding_t* optionalOutWinding)
{
	adjacencyWinding_t* r = 0;

	__asm
	{
		mov ecx, ptCount;
		push optionalOutWinding;
		push spi;
		push planeIndex;
		push normals;
		push collMap;
		mov esi, 0x5D8430;
		call esi;
		add esp, 20;
		mov r, eax;
	}

	return r;
}
static void __cdecl adjacency_winding(adjacencyWinding_t* w, float* points, vec3_t normal, unsigned int i0, unsigned int i1, unsigned int i2)
{
	auto brush = dynamic_cast<cm_brush*>(CClipMap::wip_geom.get());
	cm_triangle tri;
	std::vector<fvec3> winding_points;

	tri.a = &points[i2];
	tri.b = &points[i1];
	tri.c = &points[i0];

	PlaneFromPointsASM(tri.plane, tri.a, tri.b, tri.c);

	if (DotProduct(tri.plane, normal) < 0.f) {
		std::swap(tri.a, tri.c);
	}

	tri.material = CM_MaterialForNormal(brush->brush, normal);
	//tri.material = (char*)"caulk";
	brush->triangles.push_back(tri);

	for (int winding = 0; winding < w->numsides; winding++) {
		winding_points.push_back({ &points[winding * 3] });
	}

	brush->windings.push_back(cm_winding{ winding_points, normal, CClipMap::wip_color });

}
__declspec(naked) void __brush::__asm_adjacency_winding()
{
	static constexpr unsigned int dst = 0x5D8812;

	__asm
	{
		mov eax, [esp + 6038h + -6014h]; //i2
		lea edx, [eax + eax * 2];
		mov eax, [esp + 6038h + -6020h]; //i1
		lea ecx, [eax + eax * 2];
		mov eax, [esp + 6038h + -6018h]; //i0
		lea esi, [eax + eax * 2];

		push edx; //i2
		push ecx; //i1
		push esi; //i0
		push ebp; //normal

		lea eax, [esp + 6048h - 3000h];
		push eax; //points
		push ebx; //winding

		call adjacency_winding;
		add esp, 24;

		movss xmm0, dword ptr[ebp + 04h];
		mulss xmm0, dword ptr[esp + 6038h + -600Ch];
		movss xmm1, dword ptr[ebp + 8h];
		mulss xmm1, dword ptr[esp + 6038h + -6008h];
		
		jmp dst;
	}
		
}
char* CM_MaterialForNormal(const cbrush_t* target, const fvec3& normals)
{
	//non-axial!
	for (unsigned int i = 0; i < target->numsides; i++) {

		cbrushside_t* side = &target->sides[i];

		if (normals == side->plane->normal)
			return cm->materials[side->materialNum].material;
	}


	short mtl = -1;

	if (normals.z == 1.f)
		mtl = target->axialMaterialNum[1][2];
	else if (normals.z == -1.f)
		mtl = target->axialMaterialNum[0][2];

	if (normals.x == 1)
		mtl = target->axialMaterialNum[1][0];
	else if (normals.x == -1)
		mtl = target->axialMaterialNum[0][0];

	if (normals.y == 1.f)
		mtl = target->axialMaterialNum[1][1];
	else if (normals.y == -1.f)
		mtl = target->axialMaterialNum[0][1];

	if (mtl >= 0)
		return cm->materials[mtl].material;

	return nullptr;

}