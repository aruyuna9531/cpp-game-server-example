#include "collider_component.h"
#include "libplayer/player.h"
#include "libplayer/player_component_last_map.h"

#include <cmath>

void ColliderComponent::Awake()
{
}

void ColliderComponent::BackToPool()
{
}

void ColliderComponent::RefreshAABB(const Vector3& curPosition) {
	_aabb = AABB3(curPosition, curPosition + size);
	LOG_TRACE("ColliderComponent::RefreshAABB collider sn " << GetSN() << " aabb refresh to: " << _aabb);
}
