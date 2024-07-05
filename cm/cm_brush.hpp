
#include <utils/typedefs.hpp>

struct cm_geometry;
struct cbrush_t;
struct SimplePlaneIntersection;
struct adjacencyWinding_t;
struct cbrushside_t;

void CM_GetBrushWindings(cbrush_t* brush);
std::unique_ptr<cm_geometry> CM_GetBrushPoints(cbrush_t* brush, const fvec3& poly_col);

void CM_BuildAxialPlanes(float(*planes)[6][4], const cbrush_t* brush);
void CM_GetPlaneVec4Form(const cbrushside_t* sides, const float(*axialPlanes)[4], int index, float* expandedPlane);
int GetPlaneIntersections(const float** planes, int planeCount, SimplePlaneIntersection* OutPts);
int BrushToPlanes(const cbrush_t* brush, float(*outPlanes)[4]);
adjacencyWinding_t* BuildBrushAdjacencyWindingForSide(int ptCount, char* collMap, float* normals, int planeIndex, SimplePlaneIntersection* pts, adjacencyWinding_t* optionalOutWinding);
char* CM_MaterialForNormal(const cbrush_t* target, const fvec3& normals);

namespace __brush
{
	inline bool rb_requesting_to_stop_rendering = false; //a silly way to handle multithreaded rendering contexts
	void __asm_adjacency_winding();
}