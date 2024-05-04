#pragma once
#include "disposable.h"
#include "common.h"
#include "sn_object.h"

class EntitySystem;

class System :public IDisposable, public SnObject
{
public:
    virtual ~System() = default;
    void Dispose() override {}

    virtual const char* GetTypeName() { return ""; }
    virtual uint64 GetTypeHashCode() { return 0; }
    virtual void Update(EntitySystem* pEntities) {}
};

template<class T>
class ISystem : public System
{
public:
    virtual ~ISystem() = default;
    virtual const char* GetTypeName() override { return typeid(T).name(); }
    virtual uint64 GetTypeHashCode() override { return typeid(T).hash_code(); }
};

class IAwakeSystemBase :public IDisposable
{
public:
    void Dispose() override {}
    static bool IsSingle() { return true; }
};

template <typename... TArgs>
class IAwakeSystem :virtual public IAwakeSystemBase
{
public:
    IAwakeSystem() = default;
    virtual ~IAwakeSystem() = default;
    virtual void Awake(TArgs... args) = 0;
};

class IAwakeFromPoolSystemBase :public IDisposable
{
public:
    void Dispose() override {}
    static bool IsSingle() { return false; }
};

// IAwakeFromPoolSystem 提供对象池中对象初始化的功能，所有要从对象池中生成的组件类都要继承这个接口。
// 继承格式：public IAwakeFromPoolSystem<typename1, typename2, ..., typenameN> （也可以没有形参）
// 到时候那个组件类的纯虚函数定义格式就是：void Awake(typename1, typename2, ..., typenameN) override
// 对象从对象池中被唤醒都会执行Awake函数
template <typename... TArgs>
class IAwakeFromPoolSystem :virtual public IAwakeFromPoolSystemBase
{
public:
    IAwakeFromPoolSystem() = default;
    virtual ~IAwakeFromPoolSystem() = default;
    virtual void Awake(TArgs... args) = 0;
};
