#include <cg/cg_init.hpp>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <global_macros.hpp>

using namespace std::chrono_literals;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
    std::thread thread;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);

        thread = std::thread([&hModule]()
            {
                const auto game = GetModuleHandle(MODULE_NAME);

                if (!game) {
                    return 0;
                }

                //FILE* _con = 0;

                //AllocConsole();
                //freopen_s(&_con, "CONOUT$", "w", stdout);

                //puts("hello, world!");


                CG_Init();

                while (!!true) {
                    std::this_thread::sleep_for(1s);
                }

                //if(_con)
                //    fclose(_con);

                return 1;

            });
        thread.detach();


        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

