#ifndef HEALTH_WIDGET_HPP_INCLUDED
#define HEALTH_WIDGET_HPP_INCLUDED

#include "managers/widget.hpp"
#include "tile_map.hpp"

class A10_Game;

class HealthWidget : public Widget
{
	A10_Game* game;
	Image heart_img;
protected:
	virtual	void	_set(ptree n){};
	virtual	void 	_draw();
public:
	HealthWidget(A10_Game* _game, string name, Kernel* k);

	virtual const char* getType() const {return "A10_HealthWidget";}
};

#endif // HEALTH_WIDGET_HPP_INCLUDED
