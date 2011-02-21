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
	TimeVal frame_age;

	CBox<double>    shape;
	vector2<double> speed, skeleton_delta;
	double horiz_speed, jump_speed;

	bool touching;
	double health;

	Creature(boost::shared_ptr<GraphicsManager> gmgr, CBox<double> _shape = CBox<double>(100,50,10,20));

	void move(double sec, TileMap& map);

	void set_current_animation(list<SkeletonKeyframe>& anim);

	inline void setXSpeed(double s){this->speed.x = s;}
	void setHorizMovement(int sgn);
	void jump();

	void jump(double s);
	void swap_momentum(Creature& c);
	void swap_position(Creature& c);

	void draw(vector2<double> delta);
};

#endif // CREATURE_HPP_INCLUDED
