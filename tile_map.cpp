#include "tile_map.hpp"

using namespace std;

//------------------------------------------------------------------------------
void
TileMap::loadFromFile(string map_file_path, boost::function<TileSet*(string)> tileset_factory)
{
	this->deleteData();

	ifstream map_file(map_file_path.c_str());
	if(map_file.is_open())
	{
		string tiles_file;

		getline(map_file, tiles_file); if(not map_file.good()) throw ERROR("parse", "Can't read from map file: '"+map_file_path+"'");
		//this->readTilesFile(tiles_file);
		this->tiles = tileset_factory(tiles_file);
		assert(tiles);

		string line;
		getline(map_file, line); if(not map_file.good()) throw ERROR("parse", "Can't read from map file: '"+map_file_path+"'");
		this->size.x = boost::lexical_cast<int>(line);

		getline(map_file, line); if(not map_file.good()) throw ERROR("parse", "Can't read from map file: '"+map_file_path+"'");
		this->size.y = boost::lexical_cast<int>(line);

		if(this->size.x <= 0 or this->size.y <= 0)
			throw ERROR("parse", "Invalid width/height in '"+map_file_path+"'.");

		getline(map_file, line); if(not map_file.good()) throw ERROR("parse", "Can't read from map file: '"+map_file_path+"'");
		this->tile_size.x = boost::lexical_cast<int>(line);

		getline(map_file, line); if(not map_file.good()) throw ERROR("parse", "Can't read from map file: '"+map_file_path+"'");
		this->tile_size.y = boost::lexical_cast<int>(line);

		if(this->tile_size.x <= 0 or this->tile_size.y <= 0)
			throw ERROR("parse", "Invalid tile-width/height in '"+map_file_path+"'.");

		data = new Tile*  [size.x*size.y];
		setNULL(data, size.x*size.y);

		char c=' '; int i=0;
		while(map_file.good() and i < size.x*size.y)
		{
			map_file.get(c);
			size_t code = c;

			if(c != '\n')
			{
				if(c == 'S') /// startpoint
				{
					this->start_pos.x = i % this->size.x;
					this->start_pos.y = i / this->size.x;
				}
				else if(c == '1') /// enemy
				{
					/// TODO: implement that ;)
				}
				else if(this->tiles->hasTile(code))
					this->data[i] = this->tiles->getTile(code);
				else
					this->data[i] = NULL;

				++i;
			}
		}

		map_file.close();
		cout << "successfully loaded " << map_file_path << endl;
	}
	else
		throw ERROR("load","Can't read map file at all: '"+map_file_path+"'");
};
//------------------------------------------------------------------------------
TileMap::~TileMap()
{
	this->deleteData();
};
//------------------------------------------------------------------------------
void
TileMap::deleteData()
{
	if(this->data)
		delete[] this->data;

	this->data = NULL;
}
//------------------------------------------------------------------------------
MapCoords
TileMap::toMapCoords(ScreenCoords v)
{
	MapCoords result(0,0);

	for(int dim=1;dim<3;++dim)
	{
		if (v[dim] >= size[dim]*tile_size[dim])
			result[dim] = size[dim]-1;
		else if (v[dim] >= 0)
			result[dim] = floor(v[dim] / tile_size[dim]);
	}

	assert(result.x >= 0 and result.x < size.x);
	assert(result.y >= 0 and result.y < size.y);

	return result;
};
//------------------------------------------------------------------------------
ScreenCoords
TileMap::toScreenCoords(MapCoords v)
{
	ScreenCoords result(0,0);

	for(int dim=1;dim<3;++dim)
	{
		if (v[dim]>size[dim])
			result[dim] = size[dim]*tile_size[dim];
		else if (v[dim] > 0)
			result[dim] = v[dim]   *tile_size[dim];
	}

	return result;
};
//------------------------------------------------------------------------------
bool
TileMap::collides(CBox<Vect::T> box,  uint32_t flag)
{
	MapCoords topleft     = this->toMapCoords(box.center - box.extend);
	MapCoords bottomright = this->toMapCoords(box.center + box.extend);

	MapCoords xy(0,0);
	for(xy.y = topleft.y; xy.y <= bottomright.y; ++xy.y)
	for(xy.x = topleft.x; xy.x <= bottomright.x; ++xy.x)
		if(this->collides(xy, flag)) return true;

	return false;
};
//------------------------------------------------------------------------------
bool
TileMap::collides(CBoxEdge<Vect::T> edge, uint32_t flag)
{
	return this->collides(CBox<Vect::T>(edge), flag);
};
//------------------------------------------------------------------------------
bool
TileMap::collides(ScreenCoords point,    uint32_t flag)
{
	return this->collides(this->toMapCoords(point), flag);
};
//------------------------------------------------------------------------------
bool
TileMap::collides(MapCoords    point,    uint32_t flag)
{
	Tile* t = this->getData(point);

	return (t and t->flags & flag)
	     or point.x < 0
	     or point.y < 0
	     or point.x >= this->size.x
	     or point.y >= this->size.y;
};
//------------------------------------------------------------------------------
Vect::T
TileMap::getDistToNearestBlock(CBoxEdge<Vect::T>& e, Vect::T maxdist, int dim, uint32_t flag)
{
	assert(valid_dim(dim));
	assert(    (dim==1 and e.p1.y <= e.p2.y and e.p1.x == e.p2.x)
			or (dim==2 and e.p1.x <= e.p2.x and e.p1.y == e.p2.y));
	assert(maxdist != 0);

	if(this->collides(e, flag)) return 0;

	ScreenCoords tmp = e.p2;
	tmp[dim] += maxdist;

	MapCoords start = this->toMapCoords(e.p1),
	          max   = this->toMapCoords(tmp), /// max is automatically bounded by map size
	          pos   = start;

	/// don't check first one, as there is certainly no collision (see a few lines up ;)
	pos[dim] += sgn(maxdist);
	for(; sgn(pos[dim] - max[dim]) != sgn(maxdist); pos[dim] += sgn(maxdist))
	for(pos[3-dim] = start[3-dim]; pos[3-dim] <= max[3-dim]; ++pos[3-dim])
	{
		if(this->collides(pos, flag))
		{
			if(maxdist >= 0)
				return (pos[dim]-start[dim])*this->tile_size[dim] - e.p1[dim] + (start[dim]  )*tile_size[dim];
			else
				return (pos[dim]-start[dim])*this->tile_size[dim] - e.p1[dim] + (start[dim]+1)*tile_size[dim];
		}
	}

	return maxdist*10; ///< "no" collision
};
//------------------------------------------------------------------------------
