#pragma once
#include "player.h"

void Player::update(float dt)
{
	pos += v * dt;
}
