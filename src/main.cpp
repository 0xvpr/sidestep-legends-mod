#include "modifications.hpp"

#include <processthreadsapi.h>
#include <libloaderapi.h>

#include <windef.h>
#include <winuser.h>

unsigned button_thread(modifications::handler_ptr handler) {
    bool godmode_enabled = false;

    while (handler->thread_running) {
        if ((GetAsyncKeyState('T') & 1)) {
            handler->mod.blink();
        }
        if ((GetAsyncKeyState('M') & 1)) {
            handler->mod.max_stats();
        }

        if ((GetAsyncKeyState('G') & 1)) {
            handler->mod.toggle_godmode(godmode_enabled);
        }
    }

    return 0;
}

unsigned main_thread(HINSTANCE instance) {
    uintptr_t base_addr = (uintptr_t)GetModuleHandleA(nullptr);

    modifications::mod mod{base_addr};
    modifications::handler button_handler{
        .thread_running = true,
        .mod = mod
    };

    HANDLE button_thread_handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)button_thread, &button_handler, 0, nullptr);
    while (!(GetAsyncKeyState(VK_SPACE) & 1)) {

    }

    button_handler.thread_running = false;

    WaitForSingleObject(button_thread_handle, INFINITE);
    FreeLibraryAndExitThread(instance, 0);
}

bool DllMain(HINSTANCE instance, DWORD reason, LPVOID _reserved) {
    (void)_reserved;

    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main_thread, instance, 0, nullptr);
    }

    return 1;
}
