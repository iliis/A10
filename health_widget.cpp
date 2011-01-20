#include "health_widget.hpp"
#include "managers/kernel.hpp"
#include "a10_game.hpp"

//------------------------------------------------------------------------------
HealthWidget::HealthWidget(A10_Game* _game, string name, Kernel* k)
 : Widget(name,k), game(_game), heart_img(k->graphicsMgr->loadImage("gfx/heart.png"))
{
	assert(game);

	this->setSize(Vect(heart_img.getSize().x * MAX_LIVES,
					   heart_img.getSize().y));
};
//------------------------------------------------------------------------------
void
HealthWidget::_draw()
{
	Vect pos    = this->getAbsPos();
	     pos.x += this->size.x;

	for(int i=0; i<this->game->getPlayerLives(); ++i)
	{
		pos.x -= this->heart_img.getSize().x;
		this->heart_img.draw(pos);
	}
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
