#ifndef A10_GAME_HPP_INCLUDED
#define A10_GAME_HPP_INCLUDED

#include <map>

#include "managers/kernel.hpp"
#include "managers/widget.hpp"
#include "managers/widgets/all_widgets.hpp"
#include "managers/widgets/skeleton_editor.hpp"

#include "tile_manager.hpp"
#include "tile_map.hpp"
#include "map_widget.hpp"
#include "health_widget.hpp"
#include "creature.hpp"

const int MAX_LIVES = 3;

class A10_Game : public Widget
{
// ---------------- IMPLEMENTATION --------------------------------------
	bool keyListener(KEY k, bool state);

	boost::shared_ptr<MapWidget>    map_widget, mapf_widget;
	boost::shared_ptr<HealthWidget> health_widget;
	boost::shared_ptr<WImage> start_screen, gameover_screen, died_screen;
	boost::shared_ptr<WText> status_widget;
	std::map<string, TileSet*> tilesets;

	TileMap map_foreground;
	Image bg1,bg2,bg3;
	Image cursor_direction, cursor_cross, cursor_arrow;

	SpriteManager sprite_mgr;

// ----------------- GAME LOGIC ------------------------------------------
	TileMap map1;
	Creature player;
	int lives;

	bool paused;

	Vect cursor_pos, cursor_delta;



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

	inline void start(){this->paused = false;}
	inline void pause(){this->paused = true;}

	inline boost::shared_ptr<MapWidget>    getMapWidget()   {return this->map_widget;}
	inline boost::shared_ptr<HealthWidget> getHealthWidget(){return this->health_widget;}
	TileSet* getTileset(string path);

	inline TileMap&  getMainMap(){return this->map1;}
	inline Creature& getPlayer(){return this->player;}
	inline int       getPlayerLives(){return this->lives;}


	virtual const char* getType() const {return "A10_Game";}
};

#endif // A10_GAME_HPP_INCLUDED
