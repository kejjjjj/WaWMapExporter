#include <fstream>
#include "cm_brush.hpp"
#include "cm_typedefs.hpp"
#include "utils/errors.hpp"
#include <fs/fs_io.hpp>
#include <fs/fs_globals.hpp>

#include "cg/cg_local.hpp"
#include "cg/cg_offsets.hpp"
#include <iostream>

static void CM_WriteHeader(std::stringstream& f)
{
	f << "iwmap 4\n";
	f << "\"000_Global\" flags  active\n";
	f << "\"The Map\" flags\n";
	f << "// entity 0\n{\n";
	f << "\"contrastGain\" " "\"0.125\"" << '\n';
	f << "\"diffusefraction\" " "\"0.5\"" << '\n';
	f << "\"_color\" " "\"0.2 0.27 0.3 1\"" << '\n';
	f << "\"sunlight\" " "\"1\"" << '\n';
	f << "\"sundirection\" " "\"-30 -95 0\"" << '\n';
	f << "\"sundiffusecolor\" " "\"0.2 0.27 0.3 1\"" << '\n';
	f << "\"suncolor\" " "\"0.2 0.27 0.3 1\"" << '\n';
	f << "\"ambient\" " "\".1\"" << '\n';
	f << "\"bouncefraction\" " "\".7\"" << '\n';
	f << "\"classname\" \"worldspawn\"\n";
}
static void CM_WriteAllBrushes(std::stringstream& o)
{
	if (CClipMap::size() == 0) {
		return FatalError("CClipMap::size() == 0");
	}

	int brushIndex = 0;

	auto& geo = CClipMap::get();
	bool entity_start = false;

	for (auto& geom : geo) {

		

		if (geom->type() == cm_geomtype::model && !entity_start) {
			entity_start = true;
			brushIndex = 1;
			//end brushes
			o << "}\n";
		}

		brushIndex = geom->map_export(o, brushIndex);
	}

	if (!entity_start) {
		o << "}\n";
	}

}

void CM_WriteInOrder(std::stringstream& o)
{
	CM_WriteHeader(o);
	CM_WriteAllBrushes(o);



}

void CM_MapExport()
{
	if (!cm->name)
		return;

	CM_LoadMap();

	std::string name = cm->name;
	name = name.substr(0u, name.length() - sizeof(".d3dbsp") + 1u);
	name = name.substr(sizeof("maps/mp"));

	const std::string path = "map_source\\kej\\";
	const std::string full_path = path + name + ".map";

	const auto writer = WaWIOWriter(full_path, false);
	std::stringstream map;

	CM_WriteInOrder(map);

	if (writer.IO_Write(map.str())) {
		const std::string str = std::format("^1{} export courtesy of the great\n^2xkejj^1.\n", name);
		((void(*)(int, char*))0x562490)(5, (char*)str.c_str());
	}
	else {
		((void(*)(int, char*))0x562490)(5, (char*)"^1Failed!\n");
	}


}