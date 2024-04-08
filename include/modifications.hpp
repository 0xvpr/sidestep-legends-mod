#ifndef   SSL_MODIFICATIONS_HEADER
#define   SSL_MODIFICATIONS_HEADER

#include "definitions.hpp"
#include "types.hpp"

#include "vpr/deviate.h"

namespace modifications {

class [[nodiscard]] mod {
public:
    mod() = delete;
    mod(const mod& other) = delete;
    mod& operator = (const mod& other) = delete;
    mod(const mod&& other) = delete;
    mod& operator = (const mod&& other) = delete;

    mod(uintptr_t base_addr)
      : base_addr_{ base_addr }
      , c{ base_addr_ }
      , s{ base_addr_ }
      , coords{ (types::coordinates_ptr)(base_addr_ + offsets::bases::player_coordinates) }
    {}

    // Approximate teleport based on cursor
    constexpr void blink() const {
        coords->x += ((cursor_centered_f - c.x_ratio()) * max_width_f);
        coords->z += ((cursor_centered_f - c.y_ratio()) * max_width_f);
    }

    constexpr void max_stats() const {
        s.set_attack_damage(500);
        s.set_attack_range(500);
        s.set_ability_range(5);
        s.set_current_health(s.max_health());
    }

    void toggle_godmode(bool& godmode) {
        constexpr unsigned char original_bytes[6] {
            0x29, 0x1D, 0x00, 0x3C, 0x3F, 0x00 // sub dword [health_ptr], ebx
        };

        constexpr unsigned char hack_bytes[6] {
            0x90,
            0x90,
            0x90,
            0x90,
            0x90,
            0x90
        };

        if ((godmode = !godmode)) {
            vpr::deviate::patch(
                base_addr_+0x1BF1E6,
                hack_bytes,
                sizeof(hack_bytes)
            );
        } else {
            vpr::deviate::patch(
                base_addr_+0x1BF1E6,
                original_bytes,
                sizeof(original_bytes)
            );
        }
    }
private:
    const uintptr_t              base_addr_;
    const types::cursor          c;
    const types::stats           s;
    const types::coordinates_ptr coords;
};

} // namespace modifications

#endif // SSL_MODIFICATIONS_HEADER
