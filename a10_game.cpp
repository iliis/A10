#include "a10_game.hpp"

//------------------------------------------------------------------------------
A10_Game::A10_Game( Kernel& k )
	 : map("maps/map1/main.map"), kernel(k)
{
	if(not kernel.isInitComplete()) throw Error("init", "Can't start A10 as basic systems are not properly initialized.");

	this->kernel.inputMgr->addKeyListener(boost::bind(&A10_Game::keyListener, this, _1, _2));

	this->map_widget = boost::shared_ptr<MapWidget>(new MapWidget("main map widget", &this->kernel));
};
//------------------------------------------------------------------------------
void
A10_Game::keyListener(KEY k, bool state)
{
	if(state and k == KEY_ESCAPE)
		this->kernel.stop();
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
