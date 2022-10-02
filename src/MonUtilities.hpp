#pragma once

#include <set>
#include <string>

namespace pokemon 
{
    class MonType;

    using typeMatchup = std::set<std::string>;

    const typeMatchup grassSE { "ground", "rock", "water" };
    const typeMatchup grassNVE { "flying", "poison", "bug", "steel", "fire", "grass", "dragon" };
    const typeMatchup fireSE { "bug", "grass", "ice", "steel" };
    const typeMatchup fireNVE { "rock", "fire", "water", "dragon" };
    const typeMatchup voidMatchup { };

} // namespace pokemon