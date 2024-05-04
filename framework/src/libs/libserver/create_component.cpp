#include "create_component.h"
#include "common.h"
#include "component_factory.h"
#include "packet.h"
#include "entity.h"
#include "system_manager.h"
#include "thread.h"

// ↓关于std::tuple（C++11以上）
// 可以存储多个不同数据类型的结构，如std::tuple<double, char> 存储 {1, 'A'}
// 使用std::get<typename>(tupleName)获得typename类型的数据，或者std::get<index>(tupleName)获得index下标的数据
// 也可以用于函数有多个返回值（但不想专门新建struct）的情况。
// 关于std::tie（跟tuple搭配用）：
// 可用于解包tuple到各个变量（左值）。语法形式和效果上都类似于golang的 k, v := expr（如果expr返回2个值）。
// 如std::tie(a,b,c) = std::tuple<double,char,std::string>{1,'A', "BBB"}，可得到a=1,b='A',c="BBB"（a,b,c的预设类型要一一对应）
// ps: std::tie可以直接解耦std::pair，如std::tie(iter,inserted)=(std::set<?>).insert(?)。insert返回pair，分别是插入项的迭代器和是否成功插入（bool），将直接分别赋值给iter和inserted。
// 如果std::tie解耦的东西有不想要的，可以
// C++17开始可以使用结构化绑定：auto [a,b,c] = std::tuple<double,char,std::string>{1,'A', "BBB"}可以直接使用a=1，b='A'，c="BBB"。（这就真的和go差不多了）

template <typename... TArgs, size_t... Index>
IComponent* ComponentFactoryEx(EntitySystem* pEntitySystem, std::string className, uint64 sn, const std::tuple<TArgs...>& args, std::index_sequence<Index...>)
{
    return pEntitySystem->AddComponentByName(className, sn, std::get<Index>(args)...);
}

template<size_t ICount>
struct DynamicCall
{
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, uint64 sn, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        if (params.empty())
        {
            return ComponentFactoryEx(pEntitySystem, classname, sn, t1, std::make_index_sequence<sizeof...(TArgs)>());
        }

        Proto::CreateComponentParam param = (*(params.begin()));
        params.erase(params.begin());

        if (param.type() == Proto::CreateComponentParam::Int)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.int_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, sn, t2, params);
        }

        if (param.type() == Proto::CreateComponentParam::String)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.string_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, sn, t2, params);
        }

        if (param.type() == Proto::CreateComponentParam::UInt64)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.uint64_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, sn, t2, params);
        }

        return nullptr;
    }
};

// 以下代码是因为DynamicCall存在递归调用，没有<0>定义的话编译期就会死循环
template<>
struct DynamicCall<0>
{
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, uint64 sn, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        return nullptr;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateComponentC::Awake()
{
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();

    pMsgSystem->RegisterFunction(this, Proto::MsgId::MI_CreateComponent, BindFunP1(this, &CreateComponentC::HandleCreateComponent));
    pMsgSystem->RegisterFunction(this, Proto::MsgId::MI_RemoveComponent, BindFunP1(this, &CreateComponentC::HandleRemoveComponent));

    pMsgSystem->RegisterFunction(this, Proto::MsgId::MI_CreateSystem, BindFunP1(this, &CreateComponentC::HandleCreateSystem));

}

void CreateComponentC::BackToPool()
{

}

#define MaxDynamicCall 4
void CreateComponentC::HandleCreateComponent(Packet* pPacket) const
{
    Proto::CreateComponent proto = pPacket->ParseToProto<Proto::CreateComponent>();
    const std::string className = proto.class_name();
    uint64 sn = proto.sn();

    if (proto.params_size() > MaxDynamicCall)
    {
        LOG_ERROR(" !!!! CreateComponent failed. className:" << className.c_str() << " params size > " << MaxDynamicCall);
        return;
    }

    auto params = proto.params();
    const auto pObj = DynamicCall<MaxDynamicCall>::Invoke(GetSystemManager()->GetEntitySystem(), className, sn, std::make_tuple(), params);
    if (pObj == nullptr)
    {
        LOG_ERROR(" !!!! CreateComponent failed. className:" << className.c_str());
    }

    //std::cout << "CreateComponent. name:" << className << std::endl;
}

void CreateComponentC::HandleRemoveComponent(Packet* pPacket)
{
    Proto::RemoveComponent proto = pPacket->ParseToProto<Proto::RemoveComponent>();
    //GetEntitySystem( )->RemoveFromSystem( proto.sn( ) );
}

void CreateComponentC::HandleCreateSystem(Packet* pPacket)
{
    Proto::CreateSystem proto = pPacket->ParseToProto<Proto::CreateSystem>();
    const std::string systemName = proto.system_name();

    const auto pThread = static_cast<Thread*>(GetSystemManager());
    if (int(pThread->GetThreadType()) != proto.thread_type())
        return;

    GetSystemManager()->AddSystem(systemName);
}
