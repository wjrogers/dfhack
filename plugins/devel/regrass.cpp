// All above-ground soil not covered by buildings will be covered with grass.
// Necessary for worlds generated prior to version 0.31.19 - otherwise, outdoor shrubs and trees no longer grow.

#include "Core.h"
#include "Console.h"
#include "Export.h"
#include "PluginManager.h"

#include "DataDefs.h"
#include "df/world.h"
#include "df/map_block.h"
#include "TileTypes.h"

using std::string;
using std::vector;
using namespace DFHack;

using df::global::world;

command_result df_regrass (color_ostream &out, vector <string> & parameters)
{
    if (!parameters.empty())
        return CR_WRONG_USAGE;

    CoreSuspender suspend;

    int count = 0;
    for (size_t i = 0; i < world->map.map_blocks.size(); i++)
    {
        df::map_block *cur = world->map.map_blocks[i];
        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                if (tileShape(cur->tiletype[x][y]) != tiletype_shape::FLOOR)
                    continue;
                if (tileMaterial(cur->tiletype[x][y]) != tiletype_material::SOIL)
                    continue;
                if (cur->designation[x][y].bits.subterranean)
                    continue;
                if (cur->occupancy[x][y].bits.building)
                    continue;

                cur->tiletype[x][y] = findRandomVariant((rand() & 1) ? tiletype::GrassLightFloor1 : tiletype::GrassDarkFloor1);
                count++;
            }
        }
    }

    if (count)
        out.print("Regrew %d tiles of grass.\n", count);
    return CR_OK;
}

DFHACK_PLUGIN("regrass");

DFhackCExport command_result plugin_init (color_ostream &out, std::vector<PluginCommand> &commands)
{
    commands.push_back(PluginCommand("regrass", "Regrows all surface grass, restoring outdoor plant growth for pre-0.31.19 worlds.", df_regrass));
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
    return CR_OK;
}
