#pragma once
#include "libmath3d/aabb3.h"
#include "libserver/common.h"
#include "libserver/system.h"
#include "libserver/util_time.h"
#include "libserver/component_collections.h"
#include "collider_component.h"
#include <map>
#include <memory>

#define OCTREE_SONS_COUNT 8
#define OCTREE_SPLIT_LIMIT 10	// 一个Node超过这个数就分裂
#define OCTREE_MAX_LEVEL 3 // 最高层数（暂定）

enum OctreeType
{
	ROOT = -1,
	BOTTOM_LEFT_FRONT = 0,		// 0  x,y,z=min
	BOTTOM_RIGHT_FRONT,		// 1  x=half, y,z=min
	BOTTOM_LEFT_BACK,		// 2  x,z=min, y=half
	BOTTOM_RIGHT_BACK,      // 3  z=min, x,y = half
	TOP_LEFT_FRONT,         // 4  z=half, x,y = min
	TOP_RIGHT_FRONT,        // 5  x,z=half, y = min
	TOP_LEFT_BACK,          // 6  y,z=half, x = min
	TOP_RIGHT_BACK          // 7  x,y,z = half
};

struct OctreeSons {
	std::shared_ptr<OctreeNode> nodes[OCTREE_SONS_COUNT];

	std::shared_ptr<OctreeNode> GetNode(OctreeType nodeType) {
		if (this == nullptr) return nullptr;
		return nodes[nodeType];
	}
};

class OctreeNode {
public:
	OctreeNode(const AABB3& _aabb, int _level, OctreeNode* _father);
	~OctreeNode();
public:
	bool InsertObject(ColliderComponent* object); //插入对象
	bool RemoveObject(ColliderComponent* object); //移除对象（从自己的set）
	bool isLeaf() const { return sons == nullptr; }
	std::shared_ptr<OctreeNode> GetNode(OctreeType nodeType) const {
		if (!sons) {
			return nullptr;
		}
		return sons->GetNode(nodeType); 
	}
	bool Contains(const AABB3& obj) const {
        // 判断自身AABB是否完全包含obj
        return aabb.contains(obj);
    }
	const std::map<uint64, ColliderComponent*>& GetObjectList() const {
		return objectList;
	}
	OctreeNode* GetFather() {return father;}
	int GetLevel() {return level;}

private:
	void SplitSons();
	int level;		// 自己在树的第几层
	std::map<uint64, ColliderComponent*> objectList; // 本节点存储的对象列表。key=sn
	OctreeNode* father; // 父节点
	OctreeType octreeNodeType;	// 本节点在父节点的位置
	AABB3 aabb;		// node管辖的范围
	std::unique_ptr<OctreeSons> sons;	// 要么0（自己是null）要么8，用一个指针表示

	int TotalObjects() const;
};

// 公开函数
extern bool UpdateObjectFromOctree(ColliderComponent* object);
// 碰撞检测系统
class CollisionDetectSystem : public ISystem<CollisionDetectSystem>
{
public:
    CollisionDetectSystem();
    void Update(EntitySystem* pEntities) override;
    bool UpdateObjectFromOctree(ColliderComponent* object);
	std::vector<ColliderComponent*> DetectCollision(ColliderComponent* c);
private:
    timeutil::Time _lastTime;
    ComponentCollections* _pCollections{ nullptr };
    std::shared_ptr<OctreeNode> ocRoot = std::make_shared<OctreeNode>(AABB3(Vector3(-100,-100,-100), Vector3(100,100,100)), 1, nullptr);;
};