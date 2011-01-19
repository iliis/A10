#ifndef CREATURE_HPP_INCLUDED
#define CREATURE_HPP_INCLUDED

#include "a10_common.hpp"
#include "tile_map.hpp"

struct Creature
{
	CBox<double>    shape;
	vector2<double> speed;
	double horiz_speed, jump_speed;

	bool touching;
	double health;

	Creature();
	Creature(CBox<double> _shape);

	void move(double sec, TileMap& map);

	inline void setXSpeed(double s){this->speed.x = s;}
	void setHorizMovement(int sgn);
	void jump();

	void jump(double s);
	void swap_momentum(Creature& c);
	void swap_position(Creature& c);
};

#endif // CREATURE_HPP_INCLUDED