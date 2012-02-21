#ifndef MAP_WIDGET_HPP_INCLUDED
#define MAP_WIDGET_HPP_INCLUDED

#include "managers/widget.hpp"

#include "tile_map.hpp"

class A10_Game;

class MapWidget : public Widget
{
	A10_Game* game;
	TileMap*  map;
	Vect delta;
	bool draw_creatures;
protected:
	virtual	void	_set(ptree n){};
	virtual	void 	_draw();
public:
	MapWidget(A10_Game* _game, string name, Kernel* k);

	inline Vect getDelta(){return this->delta;}

	void setDelta(Vect d);
	void setDeltaCenter(Vect d);
	inline void setMap(TileMap* m){assert(m); this->map = m;}
	inline void enableCreatures(bool enable=true){this->draw_creatures = enable;}

	virtual const char* getType() const {return "A10_MapWidget";}
};

#endif // MAP_WIDGET_HPP_INCLUDED
