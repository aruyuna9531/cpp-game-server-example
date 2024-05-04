#pragma once
#include "libserver/component.h"

#include "player_component.h"
#include "libmath3d/vector3.h"
#include "libserver/system.h"

struct LastWorld
{
	int WorldId{ 0 };
	uint64 WorldSn{ 0 };
	Vector3 Position{ 0, 0,0 };

	LastWorld(const int worldId, const uint64 worldSn, const Vector3 pos);

	LastWorld(Proto::LastWorld proto);

	void SerializeToProto(Proto::LastWorld* pProto) const;
};

// 玩家上次下线时保存的位置（组件）
class PlayerComponentLastMap :public Component<PlayerComponentLastMap>, public IAwakeFromPoolSystem<>, public PlayerComponent
{
public:
	void Awake() override;
    void BackToPool() override;

	void ParserFromProto(const Proto::Player& proto) override;
	void SerializeToProto(Proto::Player* pProto) override;

	LastWorld* GetLastPublicMap() const;
	LastWorld* GetLastDungeon() const;
	LastWorld* GetCur() const;

	void EnterWorld(int worldId, uint64 worldSn);
	void SetCurWorld(int worldId);

protected:
	void EnterDungeon(int worldId, uint64 worldSn, Vector3 position);	

private:
	LastWorld* _pPublic{ nullptr };		// 在公共地图的位置
	LastWorld* _pDungeon{ nullptr };	// 在副本的位置

	int _curWorldId{ 0 };
};

