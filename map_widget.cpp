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

	for(int y=max(-delta.y/this->map->getTileH()-2, 0.0); y<this->map->getHeight() and y<(-delta.y+this->size.y)/this->map->getTileH(); ++y)
	for(int x=max(-delta.x/this->map->getTileW()-2, 0.0); x<this->map->getWidth()  and x<(-delta.x+this->size.x)/this->map->getTileW(); ++x)
	{
		Tile* t = this->map->getData(x,y);

		if(t) t->img.draw(Vect(x*this->map->getTileW()+delta.x, y*this->map->getTileH()+delta.y));
	}

	if(this->draw_creatures)
	{
		CBox<double> pshape = game->getPlayer().shape;
		pshape.center = pshape.center + delta;

		this->kernel->graphicsMgr->drawBoxToScreen(box<double>(pshape.center - pshape.extend, pshape.extend*2).get<float>(), RED);
	}
};
//------------------------------------------------------------------------------
void
MapWidget::setDelta(vector2<double> d)
{
	for(int dim=1;dim<3;++dim)
	{
		if (d[dim] > 0)
			d[dim] = 0;
		else if(d[dim] < this->size[dim]-this->map->getPixelSize(dim))
			    d[dim] = this->size[dim]-this->map->getPixelSize(dim);
	}

	this->delta = d;
};
//------------------------------------------------------------------------------
void
MapWidget::setDeltaCenter(vector2<double> d)
{
	this->setDelta(d+this->size.get<double>()/2);
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
