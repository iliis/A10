#ifndef A10_GAME_HPP_INCLUDED
#define A10_GAME_HPP_INCLUDED

#include <map>

#include "managers/kernel.hpp"

#include "tile_manager.hpp"
#include "tile_map.hpp"
#include "map_widget.hpp"
#include "creature.hpp"

class A10_Game
{
// ---------------- IMPLEMENTATION --------------------------------------
	void keyListener(KEY k, bool state);

	Kernel& kernel;

	boost::shared_ptr<MapWidget> map_widget;
	std::map<string, TileSet*> tilesets;

// ----------------- GAME LOGIC ------------------------------------------
	TileMap map1;
	Creature player;
	int lives;

public:
	A10_Game( Kernel& k );
	~A10_Game();

	inline void run(){this->kernel.run();};


	void move_stuff(TimeVal delta);
	void reset_player();

	inline boost::shared_ptr<MapWidget> getMapWidget(){return this->map_widget;}
	TileSet* getTileset(string path);

	inline TileMap& getMainMap(){return this->map1;}
	inline Creature& getPlayer(){return this->player;}
};

#endif // A10_GAME_HPP_INCLUDED
