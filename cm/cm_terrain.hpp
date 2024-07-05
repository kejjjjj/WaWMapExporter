#include <unordered_set>
#include <string>
#include <memory>

struct CollisionAabbTree;
struct cLeaf_t;

struct cm_terrain;
struct cm_geometry;

class CClipMap;


bool CM_AabbTreeHasCollision(const CollisionAabbTree* tree);
bool CM_DiscoverTerrain(const std::unordered_set<std::string>& filters);
void CM_AdvanceAabbTree(const CollisionAabbTree* aabbTree, cm_terrain* terrain, const std::unordered_set<std::string>& filters, const float* color);
std::unique_ptr<cm_geometry> CM_LeafToGeometry(const cLeaf_t* leaf, const std::unordered_set<std::string>& filters);
