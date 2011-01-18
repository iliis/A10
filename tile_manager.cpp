#include "tile_manager.hpp"

using namespace std;

//------------------------------------------------------------------------------
TileSet::TileSet(boost::shared_ptr<GraphicsManager> gm) : gm(gm)
{
	assert(gm);

	setNULL(tiles);
	cout << "sizeof tiles: " << (sizeof tiles) << endl;
};
//------------------------------------------------------------------------------
TileSet::~TileSet()
{
	for(int i=0;i<TILE_CODE_MAX;++i)
		if(tiles[i]) delete tiles[i];
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Tile*
TileSet::addTile(TileCode c, string path, uint32_t flags)
{
	assert(VALID_TILE_CODE(c));

	if(this->hasTile(c))
	{
		cout << "WARNING: not adding tile (code " << (size_t(c)) << ") as it already exists;" << endl;
		return this->tiles[(size_t)c];
	}
	else
		return this->tiles[(size_t)c] = new Tile(this->gm->loadImage(path), flags);
};
//------------------------------------------------------------------------------
void
TileSet::loadFromFile(string path)
{
	ifstream tfile(path.c_str());
	if(tfile.is_open())
	{
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
			}
		}

		tfile.close();
	}
	else
		throw Error("load", "Can't reat tiles-file at all: '"+path+"'");
};
//------------------------------------------------------------------------------
