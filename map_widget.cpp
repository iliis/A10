#include "map_widget.hpp"
#include "managers/kernel.hpp"
#include "a10_game.hpp"

//------------------------------------------------------------------------------
MapWidget::MapWidget(A10_Game* _game, string name, Kernel* k)
	 : Widget(name,k), game(_game), map(NULL), delta(0,0), draw_creatures(false)
{
	assert(_game);
};
//------------------------------------------------------------------------------
void
MapWidget::_draw()
{
	assert(this->map);

	for(int y=max(-delta.y/this->map->getTileH()-2, Vect::T(0)); y<this->map->getHeight() and y<(-delta.y+this->height)/this->map->getTileH(); ++y)
	for(int x=max(-delta.x/this->map->getTileW()-2, Vect::T(0)); x<this->map->getWidth()  and x<(-delta.x+this->width )/this->map->getTileW(); ++x)
	{
		Tile* t = this->map->getData(x,y);

		if(t) t->img.draw(Vect(x*this->map->getTileW()+delta.x, y*this->map->getTileH()+delta.y));
	}

	if(this->draw_creatures)
	{
		this->game->getPlayer().draw(delta);
	}
};
//------------------------------------------------------------------------------
void
MapWidget::setDelta(Vect d)
{
	for(int dim=1;dim<3;++dim)
	{
		if (d[dim] > 0)
			d[dim] = 0;
		else if(d[dim] < this->getSize(dim)-this->map->getPixelSize(dim))
			    d[dim] = this->getSize(dim)-this->map->getPixelSize(dim);
	}

	this->delta = d;
};
//------------------------------------------------------------------------------
void
MapWidget::setDeltaCenter(Vect d)
{
	this->setDelta(d+this->getSize()/2);
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
