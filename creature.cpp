#include "creature.hpp"

const FNumber ANIM_SPEED_FACTOR = 5; /// higher = faster

//------------------------------------------------------------------------------
Creature::Creature(boost::shared_ptr<GraphicsManager> gmgr, CBox<Vect::T> _shape)
 : graphicsMgr(gmgr), skeleton(gmgr),
   active_anim(&anim_standing), active_kf(0.1), next_kf(anim_standing.begin()), frame_age(0), time_since_last_touch(0),
   shape(_shape), speed(0,0), horiz_speed(500), jump_speed(800), touching(false), health(100)
{

};
//------------------------------------------------------------------------------
void
Creature::setHorizMovement(int sgn)
{
	if(sgn > 0)
		this->setXSpeed(this->horiz_speed);
	else if(sgn < 0)
		this->setXSpeed(-this->horiz_speed);
	else
		this->setXSpeed(0);
};
//------------------------------------------------------------------------------
void
Creature::jump()
{
	if (this->time_since_last_touch < fromSeconds(0.15)) ///< allow for jump even a few frames after falling down
	{
		this->time_since_last_touch = fromSeconds(100); ///< prevent double jump
		this->speed.y  = -jump_speed;
		this->touching = false;
	}
};
//------------------------------------------------------------------------------
void
Creature::move(FNumber sec, TileMap& map)
{
	if(this->health > 0) /// still alive?
	{
		touching = false;

		/// Obey gravity. It's the law!
		speed.y += GRAVITY*sec;

		vector2<Vect::T> newpos = shape.center + speed*sec;

		/// horizontal bounding
		if (newpos.x + shape.extend.x >= map.getPixelWidth())
		{
			speed.x  = 0;
			newpos.x = map.getPixelWidth() - shape.extend.x;
		}
		else if (newpos.x < shape.extend.x)
		{
			speed.x  = 0;
			newpos.x = shape.extend.x;
		}

		/// vertical bounding
		if (newpos.y + shape.extend.y >= map.getPixelHeight())
		{
			speed.y  = 0;
			newpos.y = map.getPixelHeight() - shape.extend.y;
			touching = true;
		}
		else if (newpos.y < shape.extend.y)
		{
			speed.y  = 0;
			newpos.y = shape.extend.y;
		}

		/// collision with map

		/// horizontal
		if (speed.x != 0 and newpos.x - shape.center.x != 0) ///< horiz movement?
		{
			CBoxEdge<Vect::T> tmpedge = shape.edgeX(speed.x);

			/// otherwise we get collisions due to small errors:
			tmpedge.p1.y += 0.2;
			tmpedge.p2.y -= 0.2;

			FNumber col_dist = map.getDistToNearestBlock(tmpedge, newpos.x-shape.center.x, 1, 1);

			if (Abs(col_dist) < Abs(newpos.x-shape.center.x))
			{
				newpos.x = shape.center.x + col_dist;
				speed.x  = 0;
			}

			shape.center.x = newpos.x;
		}

		/// vertical
		if (speed.y != 0 and newpos.y-shape.center.y != 0) ///< vertical movement?
		{
			CBoxEdge<Vect::T> tmpedge = shape.edgeY(speed.y);

			/// again, to counter small errors:
			tmpedge.p1.x += 0.1;
			tmpedge.p2.x -= 0.1;

			FNumber col_dist = map.getDistToNearestBlock(tmpedge, newpos.y-shape.center.y, 2, 1);
			col_dist = col_dist;

			if (Abs(col_dist) < Abs(newpos.y-shape.center.y))
			{
				if (col_dist >= 0) touching = true;

				newpos.y = shape.center.y + col_dist;
				speed.y  = 0;
			}
		}

		shape.center = newpos;


		if(this->speed.x > 0) /// going right
			this->set_current_animation(this->anim_running_right);
		else if(this->speed.x < 0) /// going left
			this->set_current_animation(this->anim_running_left);
		else /// not going anywhere
			this->set_current_animation(this->anim_standing);

		if(this->touching)
			this->time_since_last_touch = 0;
		else
			this->time_since_last_touch += fromSeconds(sec);

		/// ANIMATION
		//----------------------------------------------------------------------
		if(this->active_anim)
		{
			assert(this->active_anim->size() > 1);
			if(this->next_kf == this->active_anim->end()) this->next_kf = this->active_anim->begin();

			this->frame_age += fromSeconds(sec) * ANIM_SPEED_FACTOR / (touching?1:2); ///< slow in midair
			while(this->frame_age >= fromSeconds(this->active_kf.duration))
			{
				this->frame_age -= fromSeconds(this->active_kf.duration);
				this->active_kf  = *this->next_kf;
				++this->next_kf;
				if(this->next_kf == this->active_anim->end()) this->next_kf = this->active_anim->begin();
			}

			this->active_kf.apply_interpolated(toSeconds(this->frame_age) / this->active_kf.duration, *this->next_kf);
		}
	}
};
//------------------------------------------------------------------------------
void
Creature::set_current_animation(list<SkeletonKeyframe>& anim)
{
	assert(anim.size() > 1);

	if(&anim != this->active_anim)
	{
		this->active_kf   = SkeletonKeyframe(this->skeleton, 0.5);
		this->active_anim = &anim;
		this->next_kf     = anim.begin();
		this->frame_age   = 0;
	}
};
//------------------------------------------------------------------------------
void
Creature::swap_momentum(Creature& c)
{
	swap(this->speed, c.speed);
};
//------------------------------------------------------------------------------
void
Creature::swap_position(Creature& c)
{
	swap(this->shape.center, c.shape.center);
};
//------------------------------------------------------------------------------
void
Creature::draw(vector2<Vect::T> delta)
{
	this->skeleton.draw(this->shape.center+delta+this->skeleton_delta);

	/// bounding box:
	//this->graphicsMgr->drawBoxToScreen(Box(this->shape.center - this->shape.extend+delta, this->shape.extend*2), RED);
};
//------------------------------------------------------------------------------
