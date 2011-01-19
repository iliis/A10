#ifndef A10_GAME_HPP_INCLUDED
#define A10_GAME_HPP_INCLUDED

#include <map>

#include "managers/kernel.hpp"
#include "managers/widget.hpp"

#include "tile_manager.hpp"
#include "tile_map.hpp"
#include "map_widget.hpp"
#include "health_widget.hpp"
#include "creature.hpp"

const int MAX_LIVES = 3;

class A10_Game : public Widget
{
// ---------------- IMPLEMENTATION --------------------------------------
	void keyListener(KEY k, bool state);

	boost::shared_ptr<MapWidget>    map_widget, mapf_widget;
	boost::shared_ptr<HealthWidget> health_widget;
	std::map<string, TileSet*> tilesets;

	TileMap map_foreground;
	Image bg1,bg2,bg3;

// ----------------- GAME LOGIC ------------------------------------------
	TileMap map1;
	Creature player;
	int lives;



protected:
	virtual	void	_set(ptree n){};
	virtual	void 	draw();

public:
	A10_Game( Kernel* k );
	~A10_Game();

	void init();

	inline void run(){this->kernel->run();};


	void move_stuff(TimeVal delta);
	void reset_player();
	void restart();

	inline boost::shared_ptr<MapWidget>    getMapWidget()   {return this->map_widget;}
	inline boost::shared_ptr<HealthWidget> getHealthWidget(){return this->health_widget;}
	TileSet* getTileset(string path);

	inline TileMap&  getMainMap(){return this->map1;}
	inline Creature& getPlayer(){return this->player;}
	inline int       getPlayerLives(){return this->lives;}
};

#endif // A10_GAME_HPP_INCLUDED
