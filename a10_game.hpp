#ifndef A10_GAME_HPP_INCLUDED
#define A10_GAME_HPP_INCLUDED

#include <map>

#include "managers/kernel.hpp"

#include "tile_manager.hpp"
#include "tile_map.hpp"
#include "map_widget.hpp"

class A10_Game
{
	void keyListener(KEY k, bool state);

	TileMap map1;
	Kernel& kernel;

	boost::shared_ptr<MapWidget> map_widget;
	std::map<string, TileSet*> tilesets;

public:
	A10_Game( Kernel& k );
	~A10_Game();

	inline void run(){this->kernel.run();};

	inline boost::shared_ptr<MapWidget> getMapWidget(){return this->map_widget;}
	TileSet* getTileset(string path);
};

#endif // A10_GAME_HPP_INCLUDED
