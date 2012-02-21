#ifndef TILE_MAP_HPP_INCLUDED
#define TILE_MAP_HPP_INCLUDED


#include "tile.hpp"
#include "tile_manager.hpp"
#include "a10_common.hpp"

class TileMap
{
	vector2<int> size, tile_size;
	MapCoords start_pos;
	list<MapCoords> enemy_positions;

	//vector< Tile* > tiles;
	TileSet* tiles;
	Tile** data; ///< two dimensional array of pointers to tiles ;)

	void deleteData();

public:
	TileMap(int w, int h, int tile_w, int tile_h, TileSet* ts)
	 : size(w,h), tile_size(tile_w, tile_h),
	   start_pos(0,0), tiles(ts)
	{
		assert(w>0 and h>0);
		assert(tile_w>0 and tile_h>0);
		assert(tiles);

		data = new Tile*  [size.x * size.y];

		setNULL(data, size.x * size.y);
	};

	TileMap() : tiles(NULL), data(NULL) {};

	void loadFromFile(string map_file_path, boost::function<TileSet*(string)> tileset_factory);

	~TileMap();

	FNumber getDistToNearestBlock(CBoxEdge<FNumber>& e, FNumber maxdist, int dim, uint32_t flag);

	bool collides(CBox<FNumber>    box,   uint32_t flag);
	bool collides(CBoxEdge<FNumber> edge, uint32_t flag);
	bool collides(ScreenCoords point,     uint32_t flag);
	bool collides(MapCoords    point,     uint32_t flag);

	inline int  tileSize(const int dim) {assert(valid_dim(dim)); return this->tile_size[dim];};
	inline Tile* getData(int x, int y){assert(x>=0 and x<size.x); assert(y>=0 and y<size.y); assert(data); return data[y*size.x + x];}
	inline Tile* getData(MapCoords pos){return getData(pos.x,pos.y);}
	inline int   getWidth(){return this->size.x;}
	inline int   getHeight(){return this->size.y;}
	inline int   getTileW(){return this->tile_size.x;}
	inline int   getTileH(){return this->tile_size.y;}
	inline int   getPixelSize(int dim){assert(valid_dim(dim)); return (dim==1?this->getPixelWidth():this->getPixelHeight());}
	inline int   getPixelWidth() {return this->size.x*this->tile_size.x;}
	inline int   getPixelHeight(){return this->size.y*this->tile_size.y;}

	inline ScreenCoords getStartPosScr(){return this->toScreenCoords(this->start_pos)+(this->tile_size.cast<Vect::T>()/2);}

	MapCoords    toMapCoords(ScreenCoords v);
	ScreenCoords toScreenCoords(MapCoords v);
};

#endif // TILE_MAP_HPP_INCLUDED
