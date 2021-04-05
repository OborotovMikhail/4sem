#pragma once
#include "player.h"

void Player::update(float dt)
{
	pos += v * dt;
}

sf::Vector2f Player::get_pos()
{
	return this->pos;
}

int Player::get_rad()
{
	return this->rad;
}
