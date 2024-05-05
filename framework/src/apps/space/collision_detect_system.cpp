#include "collision_detect_system.h"
#include "move_component.h"
#include "libserver/entity_system.h"
#include "libplayer/player.h"
#include "libplayer/player_component_last_map.h"
#include "collider_component.h"

OctreeNode::OctreeNode(const AABB3& _aabb, int _level, OctreeNode* _father) :
	aabb(_aabb),
	level(_level),
	father(_father){}

OctreeNode::~OctreeNode()
{

}

bool OctreeNode::InsertObject(ColliderComponent* object)
{
	if (!object) {
		// 不要浪费表情
		return false;
	}
	if (father == nullptr && !intersectAABBs(aabb, object->GetAABB())) {
		// 和根节点无交集
		LOG_DEBUG("OctreeNode::InsertObject failed because of no intersect of root, root " << aabb << ", object " << object->GetAABB());
		return false;
	}
	bool ok = false;
	if (level == OCTREE_MAX_LEVEL)
	{
		// 只能直接放这儿
		std::tie(std::ignore, ok) = objectList.insert({object->GetSN(), object});
		if (!ok) {
			LOG_ERROR("OctreeNode::InsertObject failed, object " << object->GetSN() << " already exist in this node");
			return false;
		}
		object->SetAtNode(this);
		return true;
	}
	if (sons) {
		// 有子节点，要考虑下放到子节点
		for (auto i = 0; i < OCTREE_SONS_COUNT; i++) {
			if (sons->nodes[i]->Contains(object->GetAABB())) {
				// 这个块完全包围object，让此节点处理 TODO 能不能直接算出来
				return sons->nodes[i]->InsertObject(object);
			}
		}
		// 没有一个子节点完全包含object，放在自己身上（必定跨过多个区域）
		std::tie(std::ignore, ok) = objectList.insert({object->GetSN(), object});
		if (!ok) {
			LOG_ERROR("OctreeNode::InsertObject failed, object " << object->GetSN() << " already exist in this node");
			return false;
		}
		object->SetAtNode(this);
		return true;
	} else {
		// 这里还是叶子结点，先直接放这儿
		std::tie(std::ignore, ok) = objectList.insert({object->GetSN(), object});
		if (!ok) {
			LOG_ERROR("OctreeNode::InsertObject failed, object " << object->GetSN() << " already exist in this node");
			return false;
		}
		object->SetAtNode(this);
		if (objectList.size() >= OCTREE_SPLIT_LIMIT) {
			// 分裂
			SplitSons();
		}
		return true;
	}
}

bool OctreeNode::RemoveObject(ColliderComponent* object) {
	if (!object) {
		// 不要浪费表情
		return false;
	}
	int success = objectList.erase(object->GetSN());
	if (success == 0) {
		LOG_ERROR("OctreeNode::RemoveObject for sn " << object->GetSN() << " failed");
		return false;
	}
	object->SetAtNode(nullptr);
	if (!sons && father && father->TotalObjects() < OCTREE_SPLIT_LIMIT) {
		// 自身是叶子结点的场合，检查这个八叉树是否不需要了
		LOG_DEBUG("OctreeNode::RemoveObject after remove, father total objects less than OCTREE_SPLIT_LIMIT, combines");
		for (auto i = 0; i < OCTREE_SONS_COUNT; i++) {
			auto sl = father->sons->nodes[i]->objectList;
			for (auto iter = sl.begin(); iter != sl.end(); iter++) {
				iter->second->SetAtNode(this);
			}
            objectList.insert(sl.begin(), sl.end());
		}
		father->sons.release();
	}
	return true;
}

// 分裂子节点
void OctreeNode::SplitSons() {
	if (sons) {
		// 已分裂
		LOG_ERROR("OctreeNode::SplitSons failed, already splited");
		return;
	}

	sons.reset(new OctreeSons());
	
	if (level >= OCTREE_MAX_LEVEL) {
		// 已达到最高层数，不能继续分裂
		LOG_WARN("OctreeNode::SplitSons failed, is up to max level " << level);
		return;
	}

	LOG_TRACE("OctreeNode::SplitSons start")
	_vectorNumType X0 = aabb.min.x;
	_vectorNumType X1 = (aabb.min.x + aabb.max.x) * 0.5;
	_vectorNumType X2 = aabb.max.x;
	
	_vectorNumType Y0 = aabb.min.y;
	_vectorNumType Y1 = (aabb.min.y + aabb.max.y) * 0.5;
	_vectorNumType Y2 = aabb.max.y;
	
	_vectorNumType Z0 = aabb.min.z;
	_vectorNumType Z1 = (aabb.min.z + aabb.max.z) * 0.5;
	_vectorNumType Z2 = aabb.max.z;

	sons->nodes[BOTTOM_LEFT_FRONT] = std::make_shared<OctreeNode>(AABB3(Vector3(X0,Y0,Z0), Vector3(X1,Y1,Z1)), level + 1, this);
	sons->nodes[BOTTOM_RIGHT_FRONT] = std::make_shared<OctreeNode>(AABB3(Vector3(X1,Y0,Z0), Vector3(X2,Y1,Z1)), level + 1, this);
	sons->nodes[BOTTOM_LEFT_BACK] = std::make_shared<OctreeNode>(AABB3(Vector3(X0,Y0,Z1), Vector3(X1,Y1,Z2)), level + 1, this);
	sons->nodes[BOTTOM_RIGHT_BACK] = std::make_shared<OctreeNode>(AABB3(Vector3(X1,Y0,Z1), Vector3(X2,Y1,Z2)), level + 1, this);
	sons->nodes[TOP_LEFT_FRONT] = std::make_shared<OctreeNode>(AABB3(Vector3(X0,Y1,Z0), Vector3(X1,Y2,Z1)), level + 1, this);
	sons->nodes[TOP_RIGHT_FRONT] = std::make_shared<OctreeNode>(AABB3(Vector3(X1,Y1,Z0), Vector3(X2,Y2,Z1)), level + 1, this);
	sons->nodes[TOP_LEFT_BACK] = std::make_shared<OctreeNode>(AABB3(Vector3(X0,Y1,Z1), Vector3(X1,Y2,Z2)), level + 1, this);
	sons->nodes[TOP_RIGHT_BACK] = std::make_shared<OctreeNode>(AABB3(Vector3(X1,Y1,Z1), Vector3(X2,Y2,Z2)), level + 1, this);

	// 把自己的objects分下去
	for (auto iter = objectList.begin(); iter != objectList.end(); ) {
		if (sons->GetNode(BOTTOM_LEFT_FRONT)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to bottom left front node");
			sons->GetNode(BOTTOM_LEFT_FRONT)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(BOTTOM_RIGHT_FRONT)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to bottom right front node");
			sons->GetNode(BOTTOM_RIGHT_FRONT)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(BOTTOM_LEFT_BACK)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to bottom left back node");
			sons->GetNode(BOTTOM_LEFT_BACK)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(BOTTOM_RIGHT_BACK)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to bottom right back node");
			sons->GetNode(BOTTOM_RIGHT_BACK)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(TOP_LEFT_FRONT)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to top left front node");
			sons->GetNode(TOP_LEFT_FRONT)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(TOP_RIGHT_FRONT)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to top right front node");
			sons->GetNode(TOP_RIGHT_FRONT)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(TOP_LEFT_BACK)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split to top left back node");
			sons->GetNode(TOP_LEFT_BACK)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		if (sons->GetNode(TOP_RIGHT_BACK)->Contains(iter->second->GetAABB())) {
			LOG_TRACE("OctreeNode::SplitSons object " << iter->second->GetAABB() << " is split top right back node");
			sons->GetNode(TOP_RIGHT_BACK)->InsertObject(iter->second);
			iter = objectList.erase(iter);
			continue;
		}
		iter++;
	}
}

int OctreeNode::TotalObjects() const {
	int total = 0;
	if (sons) {
		for (auto i = 0; i < OCTREE_SONS_COUNT; i++) {
			total += sons->nodes[i]->TotalObjects();
		}
	}
	return total + objectList.size();
}

CollisionDetectSystem::CollisionDetectSystem()
{
    _lastTime = Global::GetInstance()->TimeTick;
}

void CollisionDetectSystem::Update(EntitySystem* pEntities)
{
    // 每 0.5 秒刷一次
    const auto curTime = Global::GetInstance()->TimeTick;
    const auto timeElapsed = curTime - _lastTime;
    if (timeElapsed < 500)
        return;

    if (_pCollections == nullptr)
    {
        _pCollections = pEntities->GetComponentCollections<ColliderComponent>();
        if (_pCollections == nullptr)
            return;
    }

    _lastTime = curTime;

    const auto plists = _pCollections->GetAll();
    for (auto iter = plists->begin(); iter != plists->end(); ++iter)
    {
        auto pColliderComponent = dynamic_cast<ColliderComponent*>(iter->second);
        UpdateObjectFromOctree(pColliderComponent);
    }
    for (auto iter = plists->begin(); iter != plists->end(); ++iter)
    {
        auto pColliderComponent = dynamic_cast<ColliderComponent*>(iter->second);
		DetectCollision(pColliderComponent);
    }
}


bool CollisionDetectSystem::UpdateObjectFromOctree(ColliderComponent* object) {
	if (!object) {
		// 不要浪费表情
		return false;
	}
	auto atNode = object->GetAtNode();
	if (!atNode) {
		// 不在，尝试插入
        LOG_TRACE("CollisionDetectSystem::UpdateObjectFromOctree try insert object sn " << object->GetSN() << " to octree");
		return ocRoot->InsertObject(object);
	}
    // 已在，更新位置（这里可能会出现八叉树的合并又分裂，有时间可以优化一下）
    if (!atNode->RemoveObject(object)) {
        LOG_ERROR("CollisionDetectSystem::UpdateObjectFromOctree remove object sn " << object->GetSN() << " failed");
        return false;
    }
    // 这里不用判断，可能已经润到其他space了
    ocRoot->InsertObject(object);
    return true;
}

std::vector<ColliderComponent*> CollisionDetectSystem::DetectCollision(ColliderComponent* c)
{
	std::vector<ColliderComponent*> result;
	auto atNode = c->GetAtNode();
	if (!atNode) {
		return result;
	}
	std::map<uint64, ColliderComponent*> m = atNode->GetObjectList();
	for (auto iter = m.begin(); iter != m.end(); iter++) {
		if (intersectAABBs(iter->second->GetAABB(), c->GetAABB())) {
			LOG_DEBUG("ColliderComponent::DetectCollision: sn " << GetSN() << " and " << iter->second->GetSN() << " collapse.");
			result.push_back(iter->second);
		}
	}

	auto fatherPtr = atNode->GetFather();
	while (fatherPtr) {
		std::map<uint64, ColliderComponent*> m2 = fatherPtr->GetObjectList();
		for (auto iter2 = m2.begin(); iter2 != m2.end(); iter2++) {
			if (intersectAABBs(iter2->second->GetAABB(), c->GetAABB())) {
				LOG_DEBUG("ColliderComponent::DetectCollision: sn " << GetSN() << " and " << iter2->second->GetSN() << " collapse. (at father level " << fatherPtr->GetLevel() << "/" << atNode->GetLevel() << ")");
				result.push_back(iter2->second);
			}
		}
		fatherPtr = fatherPtr->GetFather();
	}
}