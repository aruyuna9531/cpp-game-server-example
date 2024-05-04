#pragma once
#include "component.h"
#include "disposable.h"

#include <list>
#include <map>

// ComponentCollections 是按组件类型来存的，将一系列相似的组件放在一起
class ComponentCollections :public IDisposable
{
public:
    ComponentCollections(std::string componentName);
    ~ComponentCollections();

    void Add(IComponent* pObj);
    void Remove(uint64 sn);

    IComponent* Get(uint64 sn = 0);
    std::map<uint64, IComponent*>* GetAll();

    void Swap();
    void Dispose() override;

    std::string GetClassType() const;

private:
    // 以下key类型是父类Entity的sn
    std::map<uint64, IComponent*> _objs;
    // 增加或删除数据时在下面缓存，在下一帧处理，避免死锁
    std::map<uint64, IComponent*> _addObjs;
    std::list<uint64> _removeObjs;

    std::string _componentName{ "" };
};
