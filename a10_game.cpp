#include "a10_game.hpp"

//------------------------------------------------------------------------------
A10_Game::A10_Game( Kernel* k )
	 : Widget("A10", k),
	   bg1(k->graphicsMgr->loadImage("gfx/backgrounds/bg1.png")),
	   bg2(k->graphicsMgr->loadImage("gfx/backgrounds/bg2.png")),
	   bg3(k->graphicsMgr->loadImage("gfx/backgrounds/bg3.png")),
	   lives(MAX_LIVES)
{

};
//------------------------------------------------------------------------------
void
A10_Game::init()
{
	if(not kernel->isInitComplete()) throw Error("init", "Can't start A10 as basic systems are not properly initialized.");

	kernel->inputMgr->addKeyListener(boost::bind(&A10_Game::keyListener, this, _1, _2));
	kernel->setCalcFrameFunc        (boost::bind(&A10_Game::move_stuff,  this, _1));

	setSize(this->kernel->graphicsMgr->getScreenSize().get<float>());

	health_widget = boost::shared_ptr<HealthWidget>(new HealthWidget(this, "main health widget", this->kernel));
	addChild(health_widget);
	health_widget->setRelativeToParent(RIGHT,true,TOP,true);

	map1.          loadFromFile("maps/map1/main.map",       boost::bind(&A10_Game::getTileset, this, _1));
	map_foreground.loadFromFile("maps/map1/foreground.map", boost::bind(&A10_Game::getTileset, this, _1));

	map_widget = boost::shared_ptr<MapWidget>(new MapWidget(this, "main map widget", this->kernel));
	map_widget->setMap(&this->map1);
	map_widget->enableCreatures();
	map_widget->setSize(this->kernel->graphicsMgr->getScreenSize().get<float>());

	mapf_widget = boost::shared_ptr<MapWidget>(new MapWidget(this, "map foreground widget", this->kernel));
	mapf_widget->setMap(&this->map_foreground);
	mapf_widget->setSize(this->kernel->graphicsMgr->getScreenSize().get<float>());

	addChild(mapf_widget);
	addChild(map_widget);
	kernel->guiMgr->addWidget(shared_from_this());

	restart();
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
A10_Game::draw()
{
	if(this->isVisible())
	{
		if(this->isBoundingBoxEnabled())
			this->kernel->graphicsMgr->drawBoxToScreen(this->getShape());

		this->bg1.draw(this->map_widget->getDelta().get<float>()/Vect(4, 8));
		this->bg2.draw(this->map_widget->getDelta().get<float>()/Vect(3, 6));
		this->bg3.draw(this->map_widget->getDelta().get<float>()/Vect(2, 4));

		this->drawChilds();
	}
};
//------------------------------------------------------------------------------
void
A10_Game::keyListener(KEY k, bool state)
{
	if(state and k == KEY_ESCAPE)
		this->kernel->stop();
};
//------------------------------------------------------------------------------
TileSet*
A10_Game::getTileset(string path)
{
	if(this->tilesets.find(path) == this->tilesets.end())
	{
		TileSet* ts = new TileSet(kernel->graphicsMgr);
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

	if(    this->kernel->inputMgr->getKeyState(KEY_RIGHT)
		or this->kernel->inputMgr->getKeyState(KEY_d))
		player.setHorizMovement(1);
	else if(this->kernel->inputMgr->getKeyState(KEY_LEFT)
		 or this->kernel->inputMgr->getKeyState(KEY_a))
		player.setHorizMovement(-1);
	else
		player.setHorizMovement(0);

	if (   this->kernel->inputMgr->getKeyState(KEY_SPACE)
	    or this->kernel->inputMgr->getKeyState(KEY_UP)
		or this->kernel->inputMgr->getKeyState(KEY_w))
		player.jump();

	this->player.move(sec, this->getMainMap());

	if(this->getMainMap().collides(this->player.shape, 2))
	{
		-- this->lives;

		if(lives > 0)
		{
			this->reset_player();
			cout << "died. lives: " << this->lives << endl;
		}
		else
		{
			cout << "GAME OVER" << endl;
			this->restart();
		}
	}

	this->map_widget ->setDeltaCenter(this->getPlayer().shape.center*(-1));
	this->mapf_widget->setDeltaCenter(this->getPlayer().shape.center*(-1));
};
//------------------------------------------------------------------------------
void
A10_Game::reset_player()
{
	this->player.speed        = vector2<double>(0,0);
	this->player.shape.center = this->getMainMap().getStartPosScr();
};
//------------------------------------------------------------------------------
void
A10_Game::restart()
{
	this->reset_player();
	this->lives = MAX_LIVES;
}
//------------------------------------------------------------------------------
