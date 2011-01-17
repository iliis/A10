#ifndef TILE_MANAGER_HPP_INCLUDED
#define TILE_MANAGER_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstring>

#include <boost/lexical_cast.hpp>

#include "tools/errorclass.hpp"

#include "tile.hpp"

typedef char TileCode;
#define TILE_CODE_MAX 256
#define VALID_TILE_CODE(x) ((x)>=0 and (x)<TILE_CODE_MAX)

class TileSet
{
	Tile* tiles[256];
	boost::shared_ptr<GraphicsManager> gm;
public:

	TileSet(boost::shared_ptr<GraphicsManager> gm);
	~TileSet();

	Tile* addTile(TileCode c, std::string path, uint32_t flags);
	void loadFromFile(std::string path);

	inline Tile* getTile(TileCode nr){assert(VALID_TILE_CODE(nr)); return this->tiles[(size_t)nr];}
	inline bool  hasTile(TileCode nr){return VALID_TILE_CODE(nr) and this->tiles[(size_t)nr] != NULL;}

	inline Tile* operator[](TileCode nr){return this->getTile(nr);}
};

#endif // TILE_MANAGER_HPP_INCLUDED
