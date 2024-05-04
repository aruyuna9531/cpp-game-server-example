#pragma once

#include "disposable.h"
#include "system.h"
#include "common.h"
#include "thread_type.h"

#include <list>
#include <random>
#include "check_time_component.h"
#include "update_system.h"

class EntitySystem;
class MessageSystem;
class DynamicObjectPoolCollector;

// 管理所有的系统
class SystemManager : virtual public IDisposable, public CheckTimeComponent
{
public:
    SystemManager();
    void InitComponent(ThreadType threadType);

    virtual void Update();
    void Dispose() override;

    MessageSystem* GetMessageSystem() const;
    EntitySystem* GetEntitySystem() const;
    UpdateSystem* GetUpdateSystem() const;

    DynamicObjectPoolCollector* GetPoolCollector() const;

    std::default_random_engine* GetRandomEngine() const;

    void AddSystem(const std::string& name);

protected:
    MessageSystem* _pMessageSystem;     // 消息系统，负责处理从网络层或从别的线程发来的Packet
    EntitySystem* _pEntitySystem;       // 实体系统，负责所有组件和实体的管理，所有组件实例在这个类中都可以找到。如果是多线程，该系统只负责本线程的组件
    UpdateSystem* _pUpdateSystem;       // 更新系统，处理需要不断更新的数据的组件

    std::list<System*> _systems;

    std::default_random_engine* _pRandomEngine;

    DynamicObjectPoolCollector* _pPoolCollector;
};
