#pragma once

enum Message
{
	ClientCreated,
	ClientDisconnect,
	ClientReady,
	UpdateWorld,
	Movement,
	SceneGameplay,
	SceneGameover,
	RemovePlayer
};