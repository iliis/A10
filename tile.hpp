#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <string>
#include <stdint.h>

#include "managers/image_mgr.hpp"

struct Tile
{
	Image img;
	uint32_t flags;

	Tile(Image i, uint32_t _flags) : img(i), flags(_flags) {};
};

#endif // TILE_HPP_INCLUDED
