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

		getline(map_file, tiles_file); if(not map_file.good()) throw Error("parse", "Can't read from map file: '"+map_file_path+"'");
		//this->readTilesFile(tiles_file);
		this->tiles = tileset_factory(tiles_file);
		assert(tiles);

		string line;
		getline(map_file, line); if(not map_file.good()) throw Error("parse", "Can't read from map file: '"+map_file_path+"'");
		this->width = boost::lexical_cast<int>(line);

		getline(map_file, line); if(not map_file.good()) throw Error("parse", "Can't read from map file: '"+map_file_path+"'");
		this->height = boost::lexical_cast<int>(line);

		if(this->width <= 0 or this->height <= 0)
			throw Error("parse", "Invalid width/height in '"+map_file_path+"'.");

		getline(map_file, line); if(not map_file.good()) throw Error("parse", "Can't read from map file: '"+map_file_path+"'");
		this->tile_w = boost::lexical_cast<int>(line);

		getline(map_file, line); if(not map_file.good()) throw Error("parse", "Can't read from map file: '"+map_file_path+"'");
		this->tile_h = boost::lexical_cast<int>(line);

		if(this->tile_w <= 0 or this->tile_h <= 0)
			throw Error("parse", "Invalid tile-width/height in '"+map_file_path+"'.");

		//this->data = vector< vector<Tile*> >(this->width, vector<Tile*>(this->height));
		data = new Tile*  [width*height];
		memset(data, NULL, width*height*sizeof(Tile*));
		cout << "sizeof tile*: " << sizeof(Tile*) << "  w: " << width << "  h: " << height << endl;

		char c=' '; int i=0;
		while(map_file.good() and i < width*height)
		{
			map_file.get(c);
			size_t code = c;

			if(c != '\n')
			{
				if(c == 'S') /// startpoint
				{
					this->start_pos.x = i % this->width;
					this->start_pos.y = i / this->height;
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
		throw Error("load","Can't read map file at all: '"+map_file_path+"'");
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

	if (v.x >= width*tile_w)
		result.x = width-1;
	else if (v.x >= 0)
		result.x = floor(v.x/tile_w);

	if (v.y >= height*tile_h)
		result.y = height-1;
	else if (v.y >= 0)
		result.y = floor(v.y/tile_h);

	assert(result.x >= 0 and result.x < width);
	assert(result.y >= 0 and result.y < height);

	return result;
};
//------------------------------------------------------------------------------
ScreenCoords
TileMap::toScreenCoords(MapCoords v)
{
	ScreenCoords result(0,0);

	if (v.x>width)
		result.x = width*tile_w;
	else if (v.x > 0)
		result.x = v.x * tile_w;

	if (v.y>height)
		result.y = height*tile_h;
	else if (v.y > 0)
		result.y = v.y * tile_h;

	return result;
};
//------------------------------------------------------------------------------
