#include "player_manager_component.h"
#include "libserver/message_system_help.h"

void PlayerManagerComponent::Awake()
{

}

void PlayerManagerComponent::BackToPool()
{
    if (!_players.empty())
    {
        LOG_ERROR("called PlayerManagerComponent BackToPool. but it has player.");
    }

    _players.clear();
}

Player* PlayerManagerComponent::AddPlayer(const uint64 playerSn, uint64 worldSn, NetIdentify* pNetIdentify)
{
    const auto iter = _players.find(playerSn);
    if (iter != _players.end())
    {
        std::cout << "AddPlayer error." << std::endl;
        return nullptr;
    }

    const auto pPlayer = GetSystemManager()->GetEntitySystem()->AddComponent<Player>(pNetIdentify, playerSn, worldSn);
    _players[playerSn] = pPlayer;
    return pPlayer;
}

Player* PlayerManagerComponent::GetPlayerBySn(const uint64 playerSn)
{
    const auto iter = _players.find(playerSn);
    if (iter == _players.end())
        return nullptr;

    return iter->second;
}

void PlayerManagerComponent::RemovePlayerBySn(const uint64 playerSn)
{
    const auto iter = _players.find(playerSn);
    if (iter == _players.end())
        return;

    Player* pPlayer = iter->second;
    LOG_INFO("RemovePlayer: sn " << pPlayer->GetPlayerSN() << ", name " << pPlayer->GetName() << ", account " << pPlayer->GetAccount() << ", reason: actively remove");
    _players.erase(playerSn);

    GetSystemManager()->GetEntitySystem()->RemoveComponent(pPlayer);
}

void PlayerManagerComponent::RemoveAllPlayers(NetIdentify* pNetIdentify)
{
    auto iter = _players.begin();
    while (iter != _players.end())
    {
        // 查找Socket值与输入的断线协议的Socket相等的玩家，将这些玩家踢下线
        auto pPlayer = iter->second;
        if (pPlayer->GetSocketKey()->Socket != pNetIdentify->GetSocketKey()->Socket)
        {
            ++iter;
            continue;
        }

        LOG_INFO("RemovePlayer: sn " << pPlayer->GetPlayerSN() << ", name " << pPlayer->GetName() << ", account " << pPlayer->GetAccount() << ", reason: Socket " << pNetIdentify->GetSocketKey()->Socket << " disconnected");
        
        iter = _players.erase(iter);

        // 保存玩家数据
        Proto::SavePlayer protoSave;
        protoSave.set_player_sn(pPlayer->GetPlayerSN());
        pPlayer->SerializeToProto(protoSave.mutable_player());
        MessageSystemHelp::SendPacket(Proto::MsgId::G2DB_SavePlayer, protoSave, APP_DB_MGR);

        // 
        GetSystemManager()->GetEntitySystem()->RemoveComponent(pPlayer);
    }
}

int PlayerManagerComponent::OnlineSize() const
{
    return _players.size();
}

std::map<uint64, Player*>* PlayerManagerComponent::GetAll()
{
    return &_players;
}
