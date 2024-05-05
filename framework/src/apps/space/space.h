#pragma once
#include "libserver/thread_mgr.h"

#include "console_cmd_world.h"
#include "world_operator_component.h"
#include "world_gather.h"
#include "move_system.h"
#include "collision_detect_system.h"

inline void InitializeComponentSpace(ThreadMgr* pThreadMgr)
{
    pThreadMgr->CreateComponent<WorldGather>();
    pThreadMgr->CreateComponent<WorldOperatorComponent>();

    auto pConsole = pThreadMgr->GetEntitySystem()->GetComponent<Console>();
    pConsole->Register<ConsoleCmdWorld>("world");

    // 新系统
    pThreadMgr->CreateSystem<MoveSystem>();
    pThreadMgr->CreateSystem<CollisionDetectSystem>();
}
