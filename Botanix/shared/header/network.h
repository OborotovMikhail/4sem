#pragma once

enum Message
{
	ClientCreated,
	ClientDisconnect,
	ClientReady,
	ClientHeroSelected,
	ClientNickname,
	UpdateWorld,
	Movement,
	SceneGameplay,
	SceneGameover,
	RemovePlayer,
	PlayerNicknames,
	PlayerHeroSelected,
	ErrorServerFull,
	ErrorOngoingGame
};