#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <string>

struct Tile
{
	std::string path;
	uint32_t flags;

	Tile(std::string _path, uint32_t _flags) : path(_path), flags(_flags) {};
};

#endif // TILE_HPP_INCLUDED
