#ifndef   SSL_OFFSETS_HEADER
#define   SSL_OFFSETS_HEADER

#include  <cstdint>

namespace offsets {
namespace bases {

constexpr std::uint32_t cursor_ratio        = 0x376578;
constexpr std::uint32_t cursor_resolution   = 0x382944;
constexpr std::uint32_t player_coordinates  = 0x5D3750;

constexpr std::uint32_t gold                = 0x3B5A18;
constexpr std::uint32_t attack_damage       = 0x3B5BD0;
constexpr std::uint32_t max_health          = 0x3C1300;
constexpr std::uint32_t attack_range        = 0x3C1B3C;
constexpr std::uint32_t ability_range       = 0x3C1B58;
constexpr std::uint32_t current_health      = 0x5B2DEC;

} // namespace bases
} // namespace offsets

#endif // SSL_OFFSETS_HEADER
