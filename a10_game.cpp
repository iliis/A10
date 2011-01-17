#include "a10_game.hpp"

//------------------------------------------------------------------------------
A10_Game::A10_Game( Kernel& k )
	 : kernel(k)
{
	if(not kernel.isInitComplete()) throw Error("init", "Can't start A10 as basic systems are not properly initialized.");

	this->kernel.inputMgr->addKeyListener(boost::bind(&A10_Game::keyListener, this, _1, _2));


	this->map1.loadFromFile("maps/map1/main.map", boost::bind(&A10_Game::getTileset, this, _1));

	this->map_widget = boost::shared_ptr<MapWidget>(new MapWidget(this->map1, "main map widget", &this->kernel));
	this->kernel.guiMgr->addWidget(map_widget);
};
//------------------------------------------------------------------------------
A10_Game::~A10_Game()
{
	for(map<string,TileSet*>::iterator it  = this->tilesets.begin();
									   it != this->tilesets.end();
									   ++it)
		if(it->second) delete it->second;
};
//------------------------------------------------------------------------------
void
A10_Game::keyListener(KEY k, bool state)
{
	if(state and k == KEY_ESCAPE)
		this->kernel.stop();
};
//------------------------------------------------------------------------------
TileSet*
A10_Game::getTileset(string path)
{
	if(this->tilesets.find(path) == this->tilesets.end())
	{
		TileSet* ts = new TileSet(kernel.graphicsMgr);
		ts->loadFromFile(path);

		this->tilesets[path] = ts;
		return ts;
	}

	return this->tilesets[path];
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
