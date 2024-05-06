#ifndef   SSL_MODIFICATIONS_HEADER
#define   SSL_MODIFICATIONS_HEADER

#include  "definitions.hpp"
#include  "patches.hpp"
#include  "offsets.hpp"
#include  "types.hpp"

#include  "vpr/deviate.h"

#include  <atomic>

namespace modifications {

class [[nodiscard]] mod {
public:
    mod() = delete;
    mod(const mod& other) = delete;
    mod& operator = (const mod& other) = delete;
    mod(const mod&& other) = delete;
    mod& operator = (const mod&& other) = delete;

    explicit mod(uintptr_t base_addr) noexcept
      : base_addr_{ base_addr }
      , c{ base_addr_ }
      , s{ base_addr_ }
      , coords{ (types::coordinates_ptr)(base_addr_ + offsets::bases::player_coordinates) } 
    {}

    // Approximate teleport based on cursor
    constexpr void blink() const noexcept {
        coords->x += ((cursor_centered_f - c.x_ratio()) * max_width_f);
        coords->z += ((cursor_centered_f - c.y_ratio()) * max_width_f);
    }

    constexpr void max_stats() const noexcept {
        s.set_attack_damage(500);
        s.set_attack_range(500);
        s.set_ability_range(5);
        s.set_current_health(s.max_health());
    }

    void toggle_godmode(bool& godmode) const noexcept {
        godmode = !godmode;
        vpr::deviate::patch(
            base_addr_ + offsets::bases::godmode_func,
            godmode ? patches::hack_bytes : patches::original_bytes,
            sizeof(patches::original_bytes)
        );
    }
private:
    const uintptr_t              base_addr_;
    const types::cursor          c;
    const types::stats           s;
    const types::coordinates_ptr coords;
};

typedef struct _handler {
    std::atomic<bool> thread_running;
    modifications::mod& mod;
    HINSTANCE instance;
} handler, *handler_ptr;

} // namespace modifications

#endif // SSL_MODIFICATIONS_HEADER
