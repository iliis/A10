#include "creature.hpp"

//------------------------------------------------------------------------------
Creature::Creature()
 : shape(100,50,15,20), speed(0,0), horiz_speed(500), jump_speed(800)
{

};
//------------------------------------------------------------------------------
Creature::Creature(CBox<double> _shape)
 : shape(_shape), speed(0,0), horiz_speed(500), jump_speed(800)
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
	if (this->touching and this->speed.y <= 0)
	{
		this->speed.y  = -jump_speed;
		this->touching = false;
	}
};
//------------------------------------------------------------------------------
void
Creature::move(double sec, TileMap& map)
{
	if(this->health > 0) /// still alive?
	{
		/// Obey gravity. It's the law!
		speed.y += GRAVITY*sec;

		vector2<double> newpos = shape.center + speed*sec;

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
			CBoxEdge<double> tmpedge = shape.edgeX(speed.x);

			/// otherwise we get collisions due to small errors:
			tmpedge.p1.y += 0.2;
			tmpedge.p2.y -= 0.2;

			double col_dist = map.getDistToNearestBlock(tmpedge, newpos.x-shape.center.x, 1, 1);

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
			CBoxEdge<double> tmpedge = shape.edgeY(speed.y);

			/// again, to counter small errors:
			tmpedge.p1.x += 0.1;
			tmpedge.p2.x -= 0.1;

			double col_dist = map.getDistToNearestBlock(tmpedge, newpos.y-shape.center.y, 2, 1);
			col_dist = col_dist;

			if (Abs(col_dist) < Abs(newpos.y-shape.center.y))
			{
				if (col_dist >= 0) touching = true;

				newpos.y = shape.center.y + col_dist;
				speed.y  = 0;
			}
		}

		shape.center = newpos;
	}
};
//------------------------------------------------------------------------------
void
Creature::jump(double s)
{
	if (this->touching and s > 0 and speed.y <= 0)
	{
		speed.y  = -s;
		touching = false;
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

//------------------------------------------------------------------------------
