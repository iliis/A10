#include "tile_map.hpp"

using namespace std;

//------------------------------------------------------------------------------
TileMap::TileMap(string map_file_path)
{
	memset(tiles,NULL,sizeof tiles);

	ifstream map_file(map_file_path.c_str());
	if(map_file.is_open())
	{
		string tiles_file;

		getline(map_file, tiles_file); if(not map_file.good()) throw Error("parse", "Can't read from map file: '"+map_file_path+"'");
		this->readTilesFile(tiles_file);

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
				else if(this->tiles[code])
				{
					this->data[i] = this->tiles[code];
					cout << "found tile " << int(c) << " (" << tiles[code] << ")" << endl;
				}

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
	delete[] data;

	for(int i=0;i<256;++i)
		if(tiles[i]) delete tiles[i];
};
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
void
TileMap::readTilesFile(string path)
{
	ifstream tfile(path.c_str());
	if(tfile.is_open())
	{
		int i=140;
		string line;
		while(tfile.good())
		{
			getline(tfile, line);

			if(line.size() > 4) /// ignore empty lines
			{
				/// TODO: make separation for flag parameter via spaces (to allow for more than one digit)
				size_t code = line[0];
				char   flag = line[2];
				string path = line.substr(4);

				if(tiles[code])
					cerr << "WARNING: There is already a tile with char '" << code << "'. Ignoring..." << endl;
				else
					tiles[code] = new Tile(path,boost::lexical_cast<uint32_t>(flag));

				i -= 5;
			}
		}
		cout << "loaded " << i << " tiles from " << path << endl,

		tfile.close();
	}
	else
		throw Error("load", "Can't reat tiles-file at all: '"+path+"'");
};
//------------------------------------------------------------------------------

//-----------------
/*

	draw(dest:SDL_SURFACE;dx,dy: INTEGER)
		require
			dest /= void and then dest.valid
		local
			x,y:INTEGER
		do
			from
				x := -dx//tile_w  -1 -- for bigger sprites
				if x < 0 then
					x := 0
				end
			until
				x >= -dx//tile_w + dest.width//tile_w+3 or x >= width
			loop
				from
					y := -dy//tile_h -1 -- for bigger sprites
					if y < 0 then
						y := 0
					end
				until
					y >= -dy//tile_h + dest.height//tile_h+3 or y >= height
				loop
					if data.item (x+1,y+1) /= void then
						data.item (x+1,y+1).draw_to (dest, x*tile_w + dx, y*tile_h + dy)
					end

					y := y + 1
				end

				x := x + 1
			end
		end*/
