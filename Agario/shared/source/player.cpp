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

float Player::get_x()
{
	return pos.x;
}

float Player::get_y()
{
	return pos.y;
}

float Player::get_x_vel()
{
	return this->v.x;
}

float Player::get_y_vel()
{
	return this->v.y;
}

int Player::get_rad()
{
	return this->rad;
}

void Player::new_pos()
{
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); // Random seed
	std::default_random_engine generator(seed); // Generator
	std::uniform_real_distribution<float> distribution_x(float(World::Size.x) * 0.05, float(World::Size.x) * 0.95); // Creating x distribution
	std::uniform_real_distribution<float> distribution_y(float(World::Size.y) * 0.05, float(World::Size.y) * 0.95); // Creating y distribution
	sf::Vector2f pos(distribution_x(generator), distribution_y(generator)); // Generating position
	this->pos = pos;
}

void Player::upd_vel(sf::Vector2f vel)
{
	this->v = vel;
}

void Player::increase_rad()
{
	this->rad += 10;
}
