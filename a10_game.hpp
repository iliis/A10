#ifndef A10_GAME_HPP_INCLUDED
#define A10_GAME_HPP_INCLUDED

#include "managers/kernel.hpp"

#include "tile_map.hpp"
#include "map_widget.hpp"

class A10_Game
{
	void keyListener(KEY k, bool state);

	TileMap map;
	Kernel& kernel;

	boost::shared_ptr<MapWidget> map_widget;

public:
	A10_Game( Kernel& k );


	inline void run(){this->kernel.run();};

	inline boost::shared_ptr<MapWidget> getMapWidget(){return this->map_widget;}
};

#endif // A10_GAME_HPP_INCLUDED
