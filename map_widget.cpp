#include "map_widget.hpp"
#include "managers/kernel.hpp"
#include "a10_game.hpp"

//------------------------------------------------------------------------------
void
MapWidget::_draw()
{
	this->setDeltaCenter(this->game->getPlayer().shape.center*(-1));

	for(int y=max(-delta.y/game->getMainMap().getTileH()-2, 0.0); y<game->getMainMap().getHeight() and y<(-delta.y+this->size.y)/game->getMainMap().getTileH(); ++y)
	for(int x=max(-delta.x/game->getMainMap().getTileW()-2, 0.0); x<game->getMainMap().getWidth()  and x<(-delta.x+this->size.x)/game->getMainMap().getTileW(); ++x)
	{
		Tile* t = game->getMainMap().getData(x,y);

		if(t) t->img.draw(Vect(x*game->getMainMap().getTileW()+delta.x, y*game->getMainMap().getTileH()+delta.y));
	}

	CBox<double> pshape = game->getPlayer().shape;
	pshape.center = pshape.center + delta;

	this->kernel->graphicsMgr->drawBoxToScreen(box<double>(pshape.center - pshape.extend, pshape.extend*2).get<float>(), RED);
};
//------------------------------------------------------------------------------
void
MapWidget::setDelta(vector2<double> d)
{
	for(int dim=1;dim<3;++dim)
	{
		if (d[dim] > 0)
			d[dim] = 0;
		else if(d[dim] < this->size[dim]-this->game->getMainMap().getPixelSize(dim))
			    d[dim] = this->size[dim]-this->game->getMainMap().getPixelSize(dim);
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
