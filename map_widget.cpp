#include "map_widget.hpp"
#include "managers/kernel.hpp"

//------------------------------------------------------------------------------
void
MapWidget::_draw()
{

	for(int y=max(-delta.y/map.getTileH(), 0.0f); y<map.getHeight() and y<(-delta.y+this->size.y)/map.getTileH(); ++y)
	for(int x=max(-delta.x/map.getTileW(), 0.0f); x<map.getWidth()  and x<(-delta.x+this->size.x)/map.getTileW(); ++x)
	{
		Tile* t = map.getData(x,y);

		if(t) t->img.draw(Vect(x*map.getTileW(), y*map.getTileH())+delta);
	}
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
