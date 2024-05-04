#include "move_component.h"
#include "libplayer/player.h"
#include "libplayer/player_component_last_map.h"

#include <cmath>

void MoveVector3::Update(Vector3 target, Vector3 position)
{
	Target = target;
	const auto xdis = target.x - position.x;
	const auto zdis = target.z - position.z;
	const auto distance = sqrt(xdis * xdis + zdis * zdis);

	ScaleX = xdis / distance;
	ScaleZ = zdis / distance;
}

void MoveComponent::Awake()
{
	while (!_targets.empty())
		_targets.pop();
}

void MoveComponent::BackToPool()
{
	while (!_targets.empty())
		_targets.pop();
}

// 实体更新移动轨迹
void MoveComponent::Update(std::queue<Vector3> targets, Vector3 curPosition)
{
	if (targets.empty())
		return;

	// 删掉旧点
	while (!_targets.empty())
	{
		_targets.pop();
	}

	std::swap(targets, _targets);

	auto v3 = _targets.front();
	_vector3.Update(v3, curPosition);
	_targets.pop();
	LOG_TRACE("MoveComponent::Update(new targets): first target = " << v3 << ", curPos = " << curPosition);
}

// 实体正在移动
bool MoveComponent::Update(const float timeElapsed, PlayerComponentLastMap* pLastMap, const float speed)
{
	auto curPosition = pLastMap->GetCur()->Position;

	// 本次移动距离, timeElapsed 为毫秒，速度是秒
	const auto moveDis = timeElapsed * 0.001 * speed;

	bool isStop = false;
	if (curPosition.GetDistance(_vector3.Target) < moveDis )
	{
		// 已达到目标点
		LOG_TRACE("MoveComponent::Update: Reach target point " << _vector3.Target);
		curPosition = _vector3.Target;

		// 计算下一个点
		if (!_targets.empty())
		{
			const auto v3 = _targets.front();
			_vector3.Update(v3, curPosition);
			_targets.pop();
			LOG_TRACE("MoveComponent::Update: Next target point " << v3);
		}
		else
		{
			LOG_TRACE("MoveComponent::Update: No next target point, stop");
			isStop = true;
		}
	} 
	else
	{
		const auto xDis = moveDis * _vector3.ScaleX;
		const auto zDis = moveDis * _vector3.ScaleZ;

		curPosition.x += xDis;
		curPosition.z += zDis;
		LOG_TRACE("MoveComponent::Update: Moving, now at " << curPosition);
	}

	pLastMap->GetCur()->Position = curPosition;
	LOG_TRACE("MoveComponent::Update(2): cur position = " << curPosition);

	return isStop;
}
