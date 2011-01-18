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
#include "tile_manager.hpp"
#include "a10_common.hpp"

class TileMap
{
	int width, height, tile_w, tile_h;
	MapCoords start_pos;
	list<MapCoords> enemy_positions;

	//vector< Tile* > tiles;
	TileSet* tiles;
	Tile** data; ///< two dimensional array of pointers to tiles ;)

	void deleteData();

public:
	TileMap(int w, int h, int tile_w, int tile_h, TileSet* ts)
	 : width(w), height(h), tile_w(tile_w), tile_h(tile_h),
	   start_pos(0,0), tiles(ts)
	{
		assert(w>0 and h>0);
		assert(tile_w>0 and tile_h>0);
		assert(tiles);

		data = new Tile*  [width*height];
		memset(data, NULL, width*height*sizeof(Tile*));
	};

	TileMap() : tiles(NULL), data(NULL) {};

	void loadFromFile(string map_file_path, boost::function<TileSet*(string)> tileset_factory);

	~TileMap();

	double getDistToNearestBlock(CBoxEdge<double>& e, double maxdist, int dim, uint32_t flag){return 1;}

	inline int tileSize(int dim){assert(valid_dim(dim)); return (dim==1?this->tile_w:this->tile_h);};
	inline Tile* getData(int x, int y){assert(x>=0 and x<width); assert(y>=0 and y<height); assert(data); return data[y*width + x];}
	inline Tile* getData(MapCoords pos){return getData(pos.x,pos.y);}
	inline int   getWidth(){return this->width;}
	inline int   getHeight(){return this->height;}
	inline int   getTileW(){return this->tile_w;}
	inline int   getTileH(){return this->tile_h;}
	inline int   getPixelWidth() {return this->width *this->tile_w;}
	inline int   getPixelHeight(){return this->height*this->tile_h;}

	MapCoords    toMapCoords(ScreenCoords v);
	ScreenCoords toScreenCoords(MapCoords v);
};

#endif // TILE_MAP_HPP_INCLUDED
