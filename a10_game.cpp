#include "a10_game.hpp"

//------------------------------------------------------------------------------
A10_Game::A10_Game( Kernel* k )
	 : Widget("A10", k),
	   bg1(k->graphicsMgr->loadImage("gfx/backgrounds/bg1.png")),
	   bg2(k->graphicsMgr->loadImage("gfx/backgrounds/bg2.png")),
	   bg3(k->graphicsMgr->loadImage("gfx/backgrounds/bg3.png")),
	   cursor_direction(k->graphicsMgr->loadImage("gfx/direction_beam.png")),
	   cursor_cross(k->graphicsMgr->loadImage("gfx/cross.png")),
	   cursor_arrow(k->graphicsMgr->loadImage("gfx/cursor_arrow.png")),
	   sprite_mgr(k->graphicsMgr),
	   player(k->graphicsMgr),
	   lives(MAX_LIVES), paused(true)
{
	k->inputMgr->hideCursor();
	//k->inputMgr->grabCursor();

	cursor_arrow.    color.setA(50);
	cursor_direction.color.setA(150);

	player.skeleton.loadFromFile("skeletons/player.skt");
	player.skeleton.setLineWidth(3);
	player.skeleton_delta = Vect(0,-8);
	player.skeleton.setScale(0.05);
	player.skeleton.setColor(WHITE);

	player.anim_running_left  = load_sk_animation(player.skeleton, "animations/player_running_left.ska");
	player.anim_running_right = load_sk_animation(player.skeleton, "animations/player_running_right.ska");
	player.anim_standing      = load_sk_animation(player.skeleton, "animations/player_standing.ska");
	player.set_current_animation(player.anim_standing);
};
//------------------------------------------------------------------------------
void
A10_Game::init()
{
	if(not kernel->isInitComplete()) throw Error("init", "Can't start A10 as basic systems are not properly initialized.");

	kernel->inputMgr->addKeyListener(boost::bind(&A10_Game::keyListener, this, _1, _2));
	kernel->setCalcFrameFunc        (boost::bind(&A10_Game::move_stuff,  this, _1));
	kernel->settings.appname = "A10";

	setSize(this->kernel->graphicsMgr->getScreenSize().cast<Vect::T>());

	start_screen = kernel->guiMgr->createWidget<WSprite>("start screen");

	/*ptree pt = mkPtree("type", "image");
	pt.put("path", "gfx/text/start.png");
	start_screen->set(pt);*/

	start_screen->setSprite(sprite_mgr.createSprite("image", mkPtree("path", "gfx/text/start.png")));
	start_screen->setAbsPos(this->getSize()/2-start_screen->getSize()/2);

	cout << "start_screen: " << start_screen->getSize().print() << endl;

	gameover_screen = kernel->guiMgr->createWidget<WSprite>("gameover screen");
	//pt.put("path","gfx/text/gameover.png");
	//gameover_screen->set(pt);
	gameover_screen->setSprite(sprite_mgr.createSprite("image", mkPtree("path", "gfx/text/gameover.png")));
	gameover_screen->setAbsPos(this->getSize()/2-gameover_screen->getSize()/2);
	gameover_screen->hide();

	died_screen = kernel->guiMgr->createWidget<WSprite>("died screen");
	//pt.put("path","gfx/text/died.png");
	//died_screen->set(pt);
	died_screen->setSprite(sprite_mgr.createSprite("image", mkPtree("path", "gfx/text/died.png")));
	died_screen->setAbsPos(this->getSize()/2-died_screen->getSize()/2);
	died_screen->hide();

	addChild(start_screen);
	addChild(died_screen);
	addChild(gameover_screen);

	//skedit_widget = kernel->guiMgr->createWidget<SkeletonEditorWidget>("skeleton editor");


	health_widget = boost::shared_ptr<HealthWidget>(new HealthWidget(this, "main health widget", this->kernel));
	addChild(health_widget);
	health_widget->setRelativeToParent(RIGHT,true,TOP,true);

	map1.          loadFromFile("maps/map1/main.map",       boost::bind(&A10_Game::getTileset, this, _1));
	map_foreground.loadFromFile("maps/map1/foreground.map", boost::bind(&A10_Game::getTileset, this, _1));

	map_widget = boost::shared_ptr<MapWidget>(new MapWidget(this, "main map widget", this->kernel));
	map_widget->setMap(&this->map1);
	map_widget->enableCreatures();
	map_widget->setSize(this->getSize());

	cout << "map size: " << map_widget->getSize().print() << endl;

	mapf_widget = boost::shared_ptr<MapWidget>(new MapWidget(this, "map foreground widget", this->kernel));
	mapf_widget->setMap(&this->map_foreground);
	mapf_widget->setSize(this->getSize());

	addChild(mapf_widget);
	addChild(map_widget);
	kernel->guiMgr->addWidget(shared_from_this());


	status_widget = this->kernel->guiMgr->createWidget<WText>("status text");
	status_widget->setAbsPos(Vect(10,10));
	status_widget->setText("Status goes here");

	addChild(status_widget);

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
		if(this->draw_bounding_box) //this->isBoundingBoxEnabled())
			this->kernel->graphicsMgr->drawBoxToScreen(this->getBB());

		this->bg1.draw(this->map_widget->getDelta()/Vect(4, 8));
		this->bg2.draw(this->map_widget->getDelta()/Vect(3, 6));
		this->bg3.draw(this->map_widget->getDelta()/Vect(2, 4));

		this->drawChilds();

		Vect player_pos = this->player.shape.center+this->map_widget->getDelta();

		double angle = atan(cursor_delta.y/cursor_delta.x); if(cursor_delta.x<0) angle+=PI;
		//this->cursor_direction.draw_rotated(player_pos, angle, Vect(0, 0.5));

		double dist = cursor_delta.length(); if(dist < 100) dist = 100;
		this->cursor_arrow.draw_rotated(player_pos + Vect(dist * cos(angle), dist*sin(angle)), angle, Vect(0.5, 0.5));
	}
};
//------------------------------------------------------------------------------
bool
A10_Game::keyListener(KEY k, bool state)
{
	if(state and k == KEY_ESCAPE)
		{this->kernel->stop(); return true;}

	if(state and paused)
	{
		this->start();
		this->start_screen->fadeOut(0.5);
		this->gameover_screen->hide();
		this->died_screen->hide();

		if(this->lives <= 0) this->restart();

		return true;
	}

	return false; // didn't handle input
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

	//cout << "FPS: " << (1/sec) << endl;

	if(not paused and lives>0)
	{

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
				//this->pause();
				this->died_screen->show();
				this->died_screen->alpha = A_OPAQUE;
				this->died_screen->fadeOut(2);
			}
			else
			{
				cout << "GAME OVER" << endl;
				this->restart();

				this->pause();
				this->gameover_screen->show();
			}
		}

		this->map_widget ->setDeltaCenter(this->getPlayer().shape.center*(-1));
		this->mapf_widget->setDeltaCenter(this->getPlayer().shape.center*(-1));
	}

	this->cursor_pos   = this->kernel->inputMgr->getPoint("mouse1").point.pos;
	this->cursor_delta = this->cursor_pos - this->kernel->graphicsMgr->getScreenSize().cast<Vect::T>()/2;
	//this->cursor_pos - this->player.shape.center - this->map_widget->getDelta();
};
//------------------------------------------------------------------------------
void
A10_Game::reset_player()
{
	this->player.speed        = Vect(0,0);
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
