#pragma once

#include <string>
#include <utility>

struct AnimationSpec {
    AnimationSpec(const std::string& idle_n, const float idle_spd,
            const std::string& phys_n, const float phys_spd,
            const std::string& special_n, const float special_spd,
            const std::string& item_n, const float item_spd,
            const std::string& guard_n, const float guard_spd,
            const std::string& death_n, const float death_spd)
            : idle(idle_n, idle_spd),
              physical(phys_n, phys_spd),
              special(special_n, special_spd),
              item(item_n, item_spd),
              guard(guard_n, guard_spd),
              death(death_n, death_spd) {}

    const std::pair<const std::string, const float> idle;
    const std::pair<const std::string, const float> physical;
    const std::pair<const std::string, const float> special;
    const std::pair<const std::string, const float> item;
    const std::pair<const std::string, const float> guard;
    const std::pair<const std::string, const float> death;
};
