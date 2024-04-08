#ifndef   SSL_TYPES_HEADER
#define   SSL_TYPES_HEADER

#include  "offsets.hpp"

#include  <cstdint>

namespace types {

typedef struct _coordinates {
    float x;
    float y;
    float z;
} coordinates, *coordinates_ptr;

typedef struct _cursor_ratio {
    float x;
    float y;
} cursor_ratio, *cursor_ratio_ptr;

typedef struct _cursor_resolution {
    float x;
    float y;
} cursor_resolution, *cursor_resolution_ptr;

class [[nodiscard]] stats {
public:
    stats() = delete;
    stats(const stats& other) = delete;
    stats& operator = (const stats& other) = delete;
    stats(const stats&& other) = delete;
    stats& operator = (const stats&& other) = delete;

    stats(uintptr_t base_addr)
      : attack_damage_ ( (std::int32_t* )(base_addr + offsets::bases::attack_damage)  )
      , attack_range_  ( (float * )(base_addr + offsets::bases::attack_range)         )
      , ability_range_ ( (float * )(base_addr + offsets::bases::ability_range)        )
      , current_health_( (std::int32_t *)(base_addr + offsets::bases::current_health) )
      , max_health_( (std::int32_t *)(base_addr + offsets::bases::max_health) )
    {}

    constexpr std::int32_t  attack_damage()  const { return *attack_damage_;  }
    constexpr float         attack_range()   const { return *attack_range_;   }
    constexpr float         ability_range()  const { return *ability_range_;  }
    constexpr std::int32_t  current_health() const { return *current_health_; }
    constexpr std::int32_t  max_health()     const { return *max_health_;     }

    constexpr void set_attack_damage(std::int32_t value)  const { *attack_damage_ = value;  }
    constexpr void set_attack_range(float value)          const { *attack_range_ = value;   }
    constexpr void set_ability_range(float value)         const { *ability_range_ = value;  }
    constexpr void set_current_health(std::int32_t value) const { *current_health_ = value; }
    constexpr void set_max_health(std::int32_t value)     const { *max_health_ = value;     }
private:
    std::int32_t* const attack_damage_;
    float*        const attack_range_;
    float*        const ability_range_;
    std::int32_t* const current_health_;
    std::int32_t* const max_health_;
};

class [[nodiscard]] cursor {
public:
    cursor() = delete;
    cursor(const cursor& other) = delete;
    cursor& operator = (const cursor& other) = delete;
    cursor(const cursor&& other) = delete;
    cursor& operator = (const cursor&& other) = delete;

    cursor(uintptr_t base_addr)
      : ratio( (types::cursor_ratio_ptr)(base_addr + offsets::bases::cursor_ratio)           )
      , res  ( (types::cursor_resolution_ptr)(base_addr + offsets::bases::cursor_resolution) )
    {}

    constexpr float x_ratio() const { return ratio->x; };
    constexpr float y_ratio() const { return ratio->y; };
    constexpr float res_x()   const { return res->x;   };
    constexpr float res_y()   const { return res->y;   };
private:
    types::cursor_ratio_ptr      ratio;
    types::cursor_resolution_ptr res;
};


} // namespace types

#endif // SSL_TYPES_HEADER
