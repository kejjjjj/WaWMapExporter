#include "cm_terrain.hpp"
#include "cm_typedefs.hpp"

#include "cg/cg_local.hpp"
#include "cg/cg_offsets.hpp"
#include <unordered_map>
#include <ranges>
#include <com/com_vector.hpp>
#include <iostream>

#define MASK_PLAYERSOLID 0x02810011

bool CM_AabbTreeHasCollision(const CollisionAabbTree* tree)
{
	const dmaterial_t* materialInfo = &cm->materials[tree->materialIndex];
	return (materialInfo->contentFlags & MASK_PLAYERSOLID) != 0;
}

std::unordered_map<CollisionPartition*, CollisionPartition*> discovered_partitions;


bool CM_DiscoverTerrain([[maybe_unused]]const std::unordered_set<std::string>& filters)
{

	try {
		discovered_partitions.clear();
		for (const auto i : std::views::iota(0u, cm->numLeafs)) {

			auto terrain = CM_LeafToGeometry(&cm->leafs[i], filters);

			if (terrain)
				CClipMap::insert(terrain);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

void CM_AdvanceAabbTree(const CollisionAabbTree* aabbTree, cm_terrain* terrain, const std::unordered_set<std::string>& filters, const float* color)
{
	if (aabbTree->childCount) {
		auto child = &cm->aabbTrees[aabbTree->u.firstChildIndex];
		for ([[maybe_unused]]const auto i : std::views::iota(0u, aabbTree->childCount)) {
			CM_AdvanceAabbTree(child, terrain, filters, color);
			++child;
		}
		return;
	}

	const auto mat = cm->materials[aabbTree->materialIndex].material;
	if (CM_IsMatchingFilter(filters, mat) == false) {
		return;
	}

	terrain->material = mat;
	terrain->color[0] = color[0];
	terrain->color[1] = color[1];
	terrain->color[2] = color[2];
	terrain->color[3] = color[3];

	CollisionAabbTreeIndex fChild = aabbTree->u;
	CollisionPartition* partition = &cm->partitions[fChild.partitionIndex];

	if (discovered_partitions.find(partition) != discovered_partitions.end())
		return;

	discovered_partitions[partition] = partition;

	auto firstTri = partition->firstTri;
	if (firstTri < firstTri + partition->triCount)
	{

		auto triIndice = 3 * firstTri;

		do {
			cm_triangle tri;
			tri.has_collision = CM_AabbTreeHasCollision(aabbTree);
			tri.a = cm->verts[cm->triIndices[triIndice]];
			tri.b = cm->verts[cm->triIndices[triIndice + 1]];
			tri.c = cm->verts[cm->triIndices[triIndice + 2]];

			PlaneFromPointsASM(tri.plane, tri.a, tri.b, tri.c);

			tri.color[0] = color[0];
			tri.color[1] = color[1];
			tri.color[2] = color[2];
			tri.color[3] = 0.3f;

			terrain->tris.emplace_back(tri);

			++firstTri;
			triIndice += 3;

		} while (firstTri < partition->firstTri + partition->triCount);

	}
}

std::unique_ptr<cm_geometry> CM_LeafToGeometry(const cLeaf_t* leaf, const std::unordered_set<std::string>& filters)
{
	if (!leaf || !leaf->collAabbCount)
		return 0;

	std::int32_t aabbIdx = 0;
	cm_terrain terrain;

	terrain.leaf = leaf;

	do {
		const CollisionAabbTree* aabb = &cm->aabbTrees[aabbIdx + leaf->firstCollAabbIndex];
		CM_AdvanceAabbTree(aabb, &terrain, filters, vec4_t{ 0,0.1f,1.f, 0.8f });
		++aabbIdx;
	} while (aabbIdx < leaf->collAabbCount);


	return std::make_unique<cm_terrain>(terrain);
}
