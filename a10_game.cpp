#include "a10_game.hpp"

//------------------------------------------------------------------------------
A10_Game::A10_Game( Kernel& k )
	 : kernel(k), lives(3)
{
	if(not kernel.isInitComplete()) throw Error("init", "Can't start A10 as basic systems are not properly initialized.");

	this->kernel.inputMgr->addKeyListener(boost::bind(&A10_Game::keyListener, this, _1, _2));
	this->kernel.setCalcFrameFunc        (boost::bind(&A10_Game::move_stuff,  this, _1));

	this->map1.loadFromFile("maps/map1/main.map", boost::bind(&A10_Game::getTileset, this, _1));

	this->map_widget = boost::shared_ptr<MapWidget>(new MapWidget(this, "main map widget", &this->kernel));
	this->kernel.guiMgr->addWidget(map_widget);
	map_widget->setSize(this->kernel.graphicsMgr->getScreenSize().get<float>());

	this->reset_player();
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
void
A10_Game::move_stuff(TimeVal delta)
{
	double sec = toSeconds(delta);

	if(    this->kernel.inputMgr->getKeyState(KEY_RIGHT)
		or this->kernel.inputMgr->getKeyState(KEY_d))
		player.setHorizMovement(1);
	else if(this->kernel.inputMgr->getKeyState(KEY_LEFT)
		 or this->kernel.inputMgr->getKeyState(KEY_a))
		player.setHorizMovement(-1);
	else
		player.setHorizMovement(0);

	if (   this->kernel.inputMgr->getKeyState(KEY_SPACE)
	    or this->kernel.inputMgr->getKeyState(KEY_UP)
		or this->kernel.inputMgr->getKeyState(KEY_w))
		player.jump();

	this->player.move(sec, this->getMainMap());

	if(this->getMainMap().collides(this->player.shape, 2))
	{
		-- this->lives;

		this->reset_player();

		cout << "died. lives: " << this->lives << endl;
	}
};
//------------------------------------------------------------------------------
void
A10_Game::reset_player()
{
	this->player.speed        = vector2<double>(0,0);
	this->player.shape.center = this->getMainMap().getStartPosScr();
};
//------------------------------------------------------------------------------
