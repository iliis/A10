#ifndef MAP_WIDGET_HPP_INCLUDED
#define MAP_WIDGET_HPP_INCLUDED

#include "managers/widget.hpp"

#include "tile_map.hpp"

class MapWidget : public Widget
{
	TileMap& map;
protected:
	virtual	void	_set(ptree n){};
	virtual	void 	_draw();
public:
	MapWidget(TileMap& _map, string name, Kernel* k) : Widget(name,k), map(_map) {};
};

#endif // MAP_WIDGET_HPP_INCLUDED
