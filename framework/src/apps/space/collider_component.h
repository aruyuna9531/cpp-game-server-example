#pragma once

#include "libmath3d/aabb3.h"
#include "libserver/system.h"
#include "libserver/component.h"
#include <vector>

class OctreeNode;
// 碰撞体组件，可以是身体或别的
class ColliderComponent :public Component<ColliderComponent>, public IAwakeFromPoolSystem<>
{
public:
	void Awake() override;
	void BackToPool() override;

	void RefreshAABB(const Vector3& curPosition);
	void SetSize(const Vector3& _size) { size = _size; }
	const AABB3& GetAABB() const { return _aabb; }
	void SetAtNode(OctreeNode* to) { atNode = to; }
	OctreeNode* GetAtNode() { return atNode; }

private:
	Vector3 size;		// 自身AABB大小
	AABB3 _aabb;
	OctreeNode* atNode;	// 在哪个八叉树节点
};
