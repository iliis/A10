#ifndef MAP_WIDGET_HPP_INCLUDED
#define MAP_WIDGET_HPP_INCLUDED

#include "managers/widget.hpp"

#include "tile_map.hpp"

class A10_Game;

class MapWidget : public Widget
{
	A10_Game* game;
	TileMap*  map;
	vector2<double> delta;
	bool draw_creatures;
protected:
	virtual	void	_set(ptree n){};
	virtual	void 	_draw();
public:
	MapWidget(A10_Game* _game, string name, Kernel* k);

	inline vector2<double> getDelta(){return this->delta;}

	void setDelta(vector2<double> d);
	void setDeltaCenter(vector2<double> d);
	inline void setMap(TileMap* m){assert(m); this->map = m;}
	inline void enableCreatures(bool enable=true){this->draw_creatures = enable;}
};

#endif // MAP_WIDGET_HPP_INCLUDED
