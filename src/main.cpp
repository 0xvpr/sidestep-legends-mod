#include "modifications.hpp"

#include <processthreadsapi.h>
#include <libloaderapi.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_gdi.h>
#include <imgui/imgui_impl_win32.h>

#include <string>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool is_open = true;
bool godmode_enabled = false;

void render_mod_menu(modifications::handler_ptr handler) {
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplGDI_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize( {600.f, 400.f}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos( {0.f, 0.f}, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Menu", &is_open, ImGuiWindowFlags_NoScrollbar)) {
        if ( ImGui::Button((std::string("<F1> Godmode ")+(godmode_enabled ? "(enabled)" : "(disabled)")).c_str()) ) {
            handler->mod.toggle_godmode(godmode_enabled);
        }

        if ( ImGui::Button(std::string("<F2> Max Stats").c_str()) ) {
            handler->mod.max_stats();
        }

        ImGui::End();
    }

    if (!handler->thread_running) {
        PostQuitMessage(0);
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplGDI_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default: {
            break;
        }
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

[[nodiscard]] __forceinline
BOOL CALLBACK enum_windows_callback(HWND window, LPARAM lParam) {
    DWORD wndProcId = 0;
    GetWindowThreadProcessId(window, &wndProcId);

    if (GetCurrentProcessId() != wndProcId) {
        return true;
    }

    *(HWND *)lParam = window;
    return false;
}

[[nodiscard]] __forceinline
HWND get_current_window() {
    HWND window = nullptr;
    EnumWindows(enum_windows_callback, (LPARAM)&window);

    return window;
}

unsigned button_thread(const modifications::handler_ptr handler) {
    godmode_enabled = false;

    while (handler->thread_running) {
        if ((GetAsyncKeyState('T') & 1)) {
            handler->mod.blink();
        }

        if ((GetAsyncKeyState(VK_NUMPAD1) & 1)) {
            handler->mod.toggle_godmode(godmode_enabled);
        }

        if ((GetAsyncKeyState(VK_NUMPAD2) & 1)) {
            handler->mod.max_stats();
        }
    }

    return 0;
}

unsigned imgui_thread(const modifications::handler_ptr handler) {
    WNDCLASSA wc{
        .style         = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc   = wnd_proc,
        .cbClsExtra    = 0,
        .cbWndExtra    = 0,
        .hInstance     = handler->instance,
        .hIcon{},
        .hCursor       = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
        .lpszMenuName{},
        .lpszClassName = "ImGDI32",
    };
    RegisterClassA(&wc);

    HWND parent_window = get_current_window();
    RECT rect{};
    GetClientRect(parent_window, &rect);

    HWND window_handle = CreateWindowA(
        wc.lpszClassName,
        "Sidestep Legends Mod",
        WS_CHILD | WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        rect.left,
        rect.top,
        600,
        400,
        parent_window,
        nullptr,
        handler->instance,
        nullptr
    );
    SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplWin32_Init(window_handle);
    ImGui_ImplGDI_Init();

    MSG msg{};
    while (handler->thread_running) {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                handler->thread_running = false;
                is_open = false;
            }
        }

        if (!handler->thread_running) {
            break;
        }

        render_mod_menu(handler);
    }

    ImGui_ImplGDI_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

unsigned main_thread(HINSTANCE instance) {
    uintptr_t base_addr = (uintptr_t)GetModuleHandleA(nullptr);

    modifications::mod mod{base_addr};
    modifications::handler button_handler{
        .thread_running = true,
        .mod = mod,
        .instance = instance
    };
    modifications::handler imgui_handler{
        .thread_running = true,
        .mod = mod,
        .instance = instance
    };

    HANDLE button_thread_handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)button_thread, &button_handler, 0, nullptr);
    HANDLE imgui_thread_handle  = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)imgui_thread, &imgui_handler, 0, nullptr);
    while (!(GetAsyncKeyState(VK_HOME) & 1)) {
        // Panic button
    }

    button_handler.thread_running = false;
    imgui_handler.thread_running = false;

    WaitForSingleObject(button_thread_handle, INFINITE);
    WaitForSingleObject(imgui_thread_handle, INFINITE);

    if (godmode_enabled) {
        mod.toggle_godmode(godmode_enabled);
    }
    
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
