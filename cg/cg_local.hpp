#pragma once

struct cmd_function_s
{
	cmd_function_s* next;
	const char* name;
	const char* autoCompleteDir;
	const char* autoCompleteExt;
	void(__cdecl* function)();
};


struct XModelLodInfo
{
	float dist;
	unsigned __int16 numsurfs;
	unsigned __int16 surfIndex;
	int partBits[4];
	char lod;
	char smcIndexPlusOne;
	char smcAllocBits;
	char unused;
};
struct XModelHighMipBounds
{
	float mins[3];
	float maxs[3];
};

struct XModelStreamInfo
{
	XModelHighMipBounds* highMipBounds;
};
struct XModel
{
	const char* name;
	char numBones;
	char numRootBones;
	unsigned char numsurfs;
	char lodRampType;
	unsigned __int16* boneNames;
	char* parentList;
	__int16* quats;
	float* trans;
	char* partClassification;
	struct DObjAnimMat* baseMat;
	struct XSurface* surfs;
	struct Material** materialHandles;
	XModelLodInfo lodInfo[4];
	struct XModelCollSurf_s* collSurfs;
	int numCollSurfs;
	int contents;
	struct XBoneInfo* boneInfo;
	float radius;
	float mins[3];
	float maxs[3];
	__int16 numLods;
	__int16 collLod;
	XModelStreamInfo streamInfo;
	int memUsage;
	char flags;
	bool bad;
	struct PhysPreset* physPreset;
	struct PhysGeomList* physGeoms;
};

struct GfxPackedPlacement
{
	float origin[3];
	float axis[3][3];
	float scale;
};

struct __declspec(align(4)) GfxStaticModelDrawInst
{
	float cullDist;
	GfxPackedPlacement placement;
	XModel* model;
	unsigned __int16 smodelCacheIndex[4];
	char reflectionProbeIndex;
	char primaryLightIndex;
	unsigned __int16 lightingHandle;
	char flags;
	char pad[18];
};

struct SimplePlaneIntersection
{
	float xyz[3];
	int planeIndex[3];
};
struct adjacencyWinding_t
{
	int numsides;
	int sides[12];
};

struct cplane_s
{
	float normal[3];
	float dist;
	char type;  // for fast side tests: 0,1,2 = axial, 3 = nonaxial
	char signbits;  // signx + (signy<<1) + (signz<<2), used as lookup during collision
	char pad[2];
};

#pragma pack(push, 2)
struct cbrushside_t
{
	cplane_s* plane;
	unsigned int materialNum;
	__int16 firstAdjacentSideOffset;
	char edgeCount;
};
#pragma pack(pop)

#pragma pack(push, 16)
struct cbrush_t
{
	float mins[3];
	int contents;
	float maxs[3];
	unsigned int numsides;
	cbrushside_t* sides;
	__int16 axialMaterialNum[2][3];
	char* baseAdjacentSide;
	__int16 firstAdjacentSideOffsets[2][3];
	char edgeCount[2][3];
	__int16 colorCounter;
	__int16 cmBrushIndex;
	__int16 cmSubmodelIndex;
	bool isSubmodel;
	bool pad;
};
#pragma pack(pop)

struct dmaterial_t
{
	char material[64];
	int surfaceFlags;
	int contentFlags;
};
struct CollisionBorder
{
	float distEq[3];
	float zBase;
	float zSlope;
	float start;
	float length;
};

struct CollisionPartition
{
	char triCount;
	char borderCount;
	int firstTri;
	int hmm;
	int hmm2;
	CollisionBorder* borders;
};


union CollisionAabbTreeIndex
{
	int firstChildIndex;
	int partitionIndex;
};

struct CollisionAabbTree
{
	float origin[3];
	unsigned __int16 materialIndex;
	unsigned __int16 childCount;
	float halfSize[3];
	CollisionAabbTreeIndex u;
};

#pragma pack(push, 4)
struct cLeaf_t
{
	unsigned __int16 firstCollAabbIndex;
	unsigned __int16 collAabbCount;
	int brushContents;
	int terrainContents;
	float mins[3];
	float maxs[3];
	int leafBrushNode;
	__int16 cluster;
};
#pragma pack(pop)
struct clipMap_t
{
	char* name;
	int isInUse;
	int planeCount;
	void* cplane_s___planes;
	unsigned int numStaticModels;
	struct cStaticModel_s* staticModelList;
	unsigned int numMaterials;
	dmaterial_t* materials;
	unsigned int numBrushSides;
	void* cbrushside_t___brushsides;
	unsigned int numBrushEdges;
	unsigned __int8* brushEdges;
	unsigned int numNodes;
	void* cNode_t___nodes;
	unsigned int numLeafs;
	cLeaf_t* leafs;
	unsigned int leafbrushNodesCount;
	void* cLeafBrushNode_s___leafbrushNodes;
	unsigned int numLeafBrushes;
	unsigned __int16* leafbrushes;
	unsigned int numLeafSurfaces;
	unsigned int* leafsurfaces;
	unsigned int vertCount;
	float(*verts)[3];
	unsigned int numBrushVerts;
	float(*brushVerts)[3];
	unsigned int nuinds;
	unsigned __int16* uinds;
	int triCount;
	unsigned __int16* triIndices;
	unsigned __int8* triEdgeIsWalkable;
	int borderCount;
	void* CollisionBorder___borders;
	int partitionCount;
	CollisionPartition* partitions;
	int aabbTreeCount;
	CollisionAabbTree* aabbTrees;
	unsigned int numSubModels;
	void* cmodel_t___cmodels;
	unsigned __int16 numBrushes;
	cbrush_t* brushes;
	int numClusters;
	int clusterBytes;
	unsigned __int8* visibility;
	int vised;
	struct MapEnts* mapEnts;
	void* cbrush_t___box_brush;
};

