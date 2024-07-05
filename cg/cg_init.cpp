#include "cg/cg_hooks.hpp"
#include "cg_init.hpp"

#include <chrono>
#include <thread>
#include <windows.h>

#include "cg/cg_local.hpp"
#include "cg/cg_offsets.hpp"
#include <cm/cm_brush.hpp>
#include <cm/cm_export.hpp>
#include <cm/cm_typedefs.hpp>
#include <iostream>

#include "cmd/cmd.hpp"

using namespace std::chrono_literals;


void CG_Init()
{
    std::this_thread::sleep_for(300ms);
    CG_CreatePermaHooks();

    Cmd_AddCommand("cm_mapexport", CM_MapExport);

    while (true) {
        std::this_thread::sleep_for(500ms);

    }

}
