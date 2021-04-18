#pragma once

#include "player.h"

void Player::update(float dt)
{
	pos += v * dt;
}

float Player::get_maxspeed()
{
	// Computing velocity
	float result = this->StartingVelocity * (1 - 0.05 * log((float(1)/float(10) * this->score + 1)));

	if (result < this->StartingVelocity * 0.7)
	{
		// Limiting possible velocity
		return this->StartingVelocity * 0.7;
	}
	else
	{
		return result;
	}
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

int Player::get_score()
{
	return score;
}

void Player::set_score(int score)
{
	this->score = score;
}

void Player::set_vel(sf::Vector2f vel)
{
	this->v = vel;
}

void Player::increase_score()
{
	this->score++;
}

void Player::set_initial_score()
{
	this->score = 0;
}

float Player::get_radius()
{
	// Computing radius
	float result = this->StartingRadius + log((float(1) / float(15)) * float(this->score) + float(1)) / log(1.2) * 8;

	if (result > this->StartingRadius * 6)
	{
		// Limiting possible radius
		return this->StartingRadius * 6;
	}
	else
	{
		return result;
	}
}
