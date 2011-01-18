#ifndef MAP_WIDGET_HPP_INCLUDED
#define MAP_WIDGET_HPP_INCLUDED

#include "managers/widget.hpp"

#include "tile_map.hpp"

class A10_Game;

class MapWidget : public Widget
{
	A10_Game* game;
	vector2<double> delta;
protected:
	virtual	void	_set(ptree n){};
	virtual	void 	_draw();
public:
	MapWidget(A10_Game* _game, string name, Kernel* k) : Widget(name,k), game(_game), delta(0,0) {assert(_game);};
	void setDelta(vector2<double> d);
	void setDeltaCenter(vector2<double> d);
};

#endif // MAP_WIDGET_HPP_INCLUDED
