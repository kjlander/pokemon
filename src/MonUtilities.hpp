#pragma once

#include <set>
#include <string>

namespace pokemon 
{
// forward declarations
class MonType;

// A typeMatchup is used to determine whether a move is super effective or not very effective
// against another type
using typeMatchup = std::set<std::string>;

const typeMatchup grassSE { "ground", "rock", "water" };
const typeMatchup grassNVE { "flying", "poison", "bug", "steel", "fire", "grass", "dragon" };
const typeMatchup fireSE { "bug", "grass", "ice", "steel" };
const typeMatchup fireNVE { "rock", "fire", "water", "dragon" };
const typeMatchup voidMatchup { };


// For status effects
enum class Status
{
    None,
    Burn,
    Freeze,
    Paralysis,
    Poison,
    Sleep
};

} // namespace pokemon