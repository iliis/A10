#ifndef CREATURE_HPP_INCLUDED
#define CREATURE_HPP_INCLUDED

#include "managers/sprites/SpriteSkeleton.hpp"
#include "managers/time_mgr.hpp"

#include "a10_common.hpp"
#include "tile_map.hpp"

struct Creature
{
	boost::shared_ptr<GraphicsManager> graphicsMgr;
	SpriteSkeleton  skeleton;
	list<SkeletonKeyframe> anim_standing, anim_running_left, anim_running_right, *active_anim;
	SkeletonKeyframe active_kf; list<SkeletonKeyframe>::iterator next_kf;
	TimeVal frame_age, time_since_last_touch;

	CBox<Vect::T>    shape;
	Vect speed, skeleton_delta;
	FNumber horiz_speed, jump_speed;

	bool touching;
	FNumber health;

	Creature(boost::shared_ptr<GraphicsManager> gmgr, CBox<Vect::T> _shape = CBox<Vect::T>(100,50,10,20));

	void move(Vect::T sec, TileMap& map);

	void set_current_animation(list<SkeletonKeyframe>& anim);

	inline void setXSpeed(Vect::T s){this->speed.x = s;}
	void setHorizMovement(int sgn);
	void jump();
	void swap_momentum(Creature& c);
	void swap_position(Creature& c);

	void draw(Vect delta);
};

#endif // CREATURE_HPP_INCLUDED
