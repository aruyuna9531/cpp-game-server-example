#include "move_system.h"
#include "move_component.h"
#include "collider_component.h"
#include "libserver/entity_system.h"
#include "libplayer/player.h"
#include "libplayer/player_component_last_map.h"

MoveSystem::MoveSystem()
{
    _lastTime = Global::GetInstance()->TimeTick;
}

void MoveSystem::Update(EntitySystem* pEntities)
{
    // 每 0.5 秒刷一次
    const auto curTime = Global::GetInstance()->TimeTick;
    const auto timeElapsed = curTime - _lastTime;
    if (timeElapsed < 500)
        return;

    if (_pCollections == nullptr)
    {
        _pCollections = pEntities->GetComponentCollections<MoveComponent>();
        if (_pCollections == nullptr)
            return;
    }

    _lastTime = curTime;

    const auto plists = _pCollections->GetAll();
    for (auto iter = plists->begin(); iter != plists->end(); ++iter)
    {
        auto pMoveComponent = dynamic_cast<MoveComponent*>(iter->second);
        auto pPlayer = pMoveComponent->GetParent<Player>();

        auto pLastMap = pPlayer->GetComponent<PlayerComponentLastMap>();
        if (pMoveComponent->Update(timeElapsed, pLastMap, 2))
        {
            // 停下来了，移除MoveComponent
            pPlayer->RemoveComponent<MoveComponent>();
        }
        // 更新collider
        auto pCol = pPlayer->GetComponent<ColliderComponent>();
        pCol->RefreshAABB(pLastMap->GetCur()->Position);
    }
}
