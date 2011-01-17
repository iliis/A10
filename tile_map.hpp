#ifndef TILE_MAP_HPP_INCLUDED
#define TILE_MAP_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>

#include "tools/geometry.hpp"
#include "tile.hpp"
#include "a10_common.hpp"

class TileMap
{
	int width, height, tile_w, tile_h;
	MapCoords start_pos;
	list<MapCoords> enemy_positions;

	//vector< Tile* > tiles;
	Tile* tiles[256];
	Tile** data; ///< two dimensional array of pointers to tiles ;)

public:
	TileMap(int w, int h, int tile_w, int tile_h)
	 : width(w), height(h), tile_w(tile_w), tile_h(tile_h),
	   start_pos(0,0)
	{
		assert(w>0 and h>0);
		assert(tile_w>0 and tile_h>0);

		memset(tiles, NULL, sizeof tiles);

		data = new Tile*  [width*height];
		memset(data, NULL, width*height*sizeof(Tile*));
	};

	TileMap(string map_file_path);

	~TileMap();

	inline int tileSize(int dim){assert(valid_dim(dim)); return (dim==1?this->tile_w:this->tile_h);};
	inline Tile* getData(int x, int y){assert(x>0 and x<width); assert(y>0 and y<height); return data[x*width+y];}
	inline Tile* getData(MapCoords pos){return getData(pos.x,pos.y);}

	MapCoords    toMapCoords(ScreenCoords v);
	ScreenCoords toScreenCoords(MapCoords v);

	void readTilesFile(string path);
};

#endif // TILE_MAP_HPP_INCLUDED
