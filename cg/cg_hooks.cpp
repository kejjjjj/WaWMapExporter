#include "utils/hook.hpp"
#include <cm/cm_brush.hpp>

#include <chrono>
#include <thread>
using namespace std::chrono_literals;

static void CG_CreateHooks();

void CG_CreatePermaHooks()
{
	hooktable::initialize();

	CG_CreateHooks();

}
void CG_CreateHooks()
{
	hooktable::preserver<void>("__asm_adjacency_winding", 0x5D87FC, __brush::__asm_adjacency_winding);


}