#include "modifications.hpp"

#include <processthreadsapi.h>
#include <libloaderapi.h>
#include <synchapi.h>
#include <windef.h>

#include <winuser.h>

#include <atomic>

std::atomic<bool> button_thread_running = true;

unsigned button_thread(modifications::mod* m) {
    bool godmode_enabled = false;
    while (button_thread_running) {
        if ((GetAsyncKeyState('T') & 1)) {
            m->blink();
        }
        if ((GetAsyncKeyState('M') & 1)) {
            m->max_stats();
        }

        if ((GetAsyncKeyState('G') & 1)) {
            m->toggle_godmode(godmode_enabled);
        }
    }

    return 0;
}

unsigned main_thread(HINSTANCE instance) {
    uintptr_t base_addr = (uintptr_t)GetModuleHandleA(nullptr);

    modifications::mod m{base_addr};
    HANDLE button_thread_handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)button_thread, &m, 0, nullptr);

    while (!(GetAsyncKeyState(VK_SPACE) & 1)) {

    }

    button_thread_running = false;

    WaitForSingleObject(button_thread_handle, INFINITE);
    FreeLibraryAndExitThread(instance, 0);
}

bool DllMain(HINSTANCE instance, DWORD reason, LPVOID _reserved) {
    (void)_reserved;

    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main_thread, instance, 0, nullptr);
    }

    return true;
}
