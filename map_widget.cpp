#include "map_widget.hpp"
#include "managers/kernel.hpp"

//------------------------------------------------------------------------------
void
MapWidget::_draw()
{

	for(int x=0; x<map.getWidth(); ++x)
	for(int y=0; y<map.getHeight(); ++y)
	{
		Tile* t = map.getData(x,y);

		if(t) t->img.draw(Vect(x*map.getTileW(), y*map.getTileH()));
	}

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
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
