#pragma once
#include "libserver/component.h"
#include "libserver/system.h"
#include "libmath3d/vector3.h"

#include <queue>

class PlayerComponentLastMap;

struct MoveVector3
{
	void Update(Vector3 target, Vector3 position);

	Vector3 Target = kZeroVector;
	_vectorNumType ScaleX{ 0 };
	_vectorNumType ScaleZ{ 0 };
};

// 移动组件
class MoveComponent :public Component<MoveComponent>, public IAwakeFromPoolSystem<>
{
public:
	void Awake() override;
	void BackToPool() override;

	void Update(std::queue<Vector3> targets, Vector3 curPosition);
	bool Update(float timeElapsed, PlayerComponentLastMap* pLastMap, const float speed);

private:
	std::queue<Vector3> _targets;
	MoveVector3 _vector3;
};
