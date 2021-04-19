#pragma once

#include "player.h"

void Player::update(float dt)
{
	pos += v * dt;
}

float Player::get_maxspeed()
{
	return this->MaxSpeed;
}

sf::Vector2f Player::get_pos()
{
	return this->pos;
}

void Player::set_pos(sf::Vector2f new_pos)
{
	this->pos.x = new_pos.x;
	this->pos.y = new_pos.y;
}

sf::Vector2f Player::get_vel()
{
	return this->v;
}

int Player::get_rad()
{
	return this->rad;
}

void Player::set_vel(sf::Vector2f vel)
{
	this->v = vel;
}

void Player::increase_score()
{
	this->score++;
}

int Player::get_score()
{
	return this->score;
}