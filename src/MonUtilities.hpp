#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace pokemon 
{
// forward declarations
class Mon;
class MonType;

/* A typeMatchup is used to determine whether a move is super effective or not very effective
   against another type. */
using typeMatchup = std::set<std::string>;

const typeMatchup grassSE { "ground", "rock", "water" };
const typeMatchup grassNVE { "flying", "poison", "bug", "steel", "fire", "grass", "dragon" };
const typeMatchup fireSE { "bug", "grass", "ice", "steel" };
const typeMatchup fireNVE { "rock", "fire", "water", "dragon" };
const typeMatchup normalNVE { "rock", "steel" };
const typeMatchup voidMatchup { };


// namespace MonTypes
// {

enum class Ability
{
    LiquidOoze,
};

enum class Type
{
    None = -1,
    Normal,
    Fighting,
    Flying,
    Poison,
    Ground,
    Rock,
    Bug,
    Ghost,
    Steel,
    Fire,
    Water,
    Grass,
    Electric,
    Psychic,
    Ice,
    Dragon,
    Dark,
    Fairy
};

// Effectiveness values will be divided by 10 to get the correct modifier.
// The implementation of the effectiveness chart is inspired from the
// implementation of the pkmn engine: https://github.com/pkmn/engine
enum class Eff
{
    I = 0,  // Immune
    R = 5,  // Resist
    N = 10, // Neutral
    S = 20  // Super
};

// Alias the enum to make the chart easier to write and read.
const Eff I = Eff::I;
const Eff R = Eff::R;
const Eff N = Eff::N;
const Eff S = Eff::S;

using EffectivenessChart = std::array<std::array<Eff, 18>, 18>;
const EffectivenessChart CHART
{
    {
     { N, N, N, N, N, R, N, I, R, N, N, N, N, N, N, N, N, N }, // Normal
     { S, N, R, R, N, S, R, I, S, N, N, N, N, R, S, N, S, R }, // Fighting
     { N, S, N, N, N, R, S, N, R, N, N, S, R, N, N, N, N, N }, // Flying
     { N, N, N, R, R, R, N, R, I, N, N, S, N, N, N, N, N, S }, // Poison
     { N, N, I, S, N, S, R, N, S, S, N, R, S, N, N, N, N, N }, // Ground
     { N, R, S, N, R, N, S, N, R, S, N, N, N, N, S, N, N, N }, // Rock
     { N, R, R, R, N, N, N, R, R, R, N, S, N, S, N, N, S, R }, // Bug
     { I, N, N, N, N, N, N, S, N, N, N, N, N, S, N, N, R, N }, // Ghost
     { N, N, N, N, N, S, N, N, R, R, R, N, R, N, S, N, N, S }, // Steel
     { N, N, N, N, N, R, S, N, S, R, R, S, N, N, S, R, N, N }, // Fire
     { N, N, N, N, S, S, N, N, N, S, R, R, N, N, N, R, N, N }, // Water
     { N, N, R, R, S, S, R, N, R, R, S, R, N, N, N, R, N, N }, // Grass
     { N, N, S, N, I, N, N, N, N, N, S, R, R, N, N, R, N, N }, // Electric
     { N, S, N, S, N, N, N, N, R, N, N, N, N, R, N, N, I, N }, // Psychic
     { N, N, S, N, S, N, N, N, R, R, R, S, N, N, R, S, N, N }, // Ice
     { N, N, N, N, N, N, N, N, R, N, N, N, N, N, N, S, N, I }, // Dragon
     { N, R, N, N, N, N, N, S, N, N, N, N, N, S, N, N, R, R }, // Dark
     { N, S, N, R, N, N, N, N, R, R, N, N, N, N, N, S, S, N }  // Fariy
    }
};

// } // MonTypes


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

enum class Volatile
{
    Bound,
    CantEscape,
    Confusion,
    Curse,
    Drowsy,
    Embargo,
    Encore,
    Flinch,
    HealBlock,
    Identified,
    Infatuation,
    LeechSeed,
    Nightmare,
    PerishSong,
    Taunt,
    Telekinesis,
    Torment,
    TypeChange,
};

struct Statuses
{
    // Non-volatiles
    uint8_t NVtimer = 0;
    Status NVStatus = Status::None;

    // Volatiles
    uint8_t Bound = 0;
    uint8_t Confusion = 0;
    uint8_t Drowsy = 0;
    uint8_t Embargo = 0;
    uint8_t Encore = 0;
    uint8_t Flinch = 0;
    uint8_t HealBlock = 0;
    uint8_t PerishSong = 0;
    uint8_t Taunt = 0;
    uint8_t Telekinesis = 0;
    bool CantEscape = false;
    bool Curse = false;
    bool Identified = false;
    bool LeechSeed = false;
    bool MagicCoat = false;
    bool Nightmare = false;
    bool Torment = false;
    Type TypeChange = Type::None;
    std::shared_ptr<Mon> Binder = nullptr;
    std::shared_ptr<Mon> Infatuator = nullptr;
    std::shared_ptr<Mon> LeechSeeder = nullptr;
};

enum class Item
{
    GripClaw,
    KingsRock,
    RazorFang,
};

// If you want to use an ostream other than std::cout, provide a reference to it as 
// the first argument. Accepts any number of printable args afterwards.
template<typename... Args>
void monLog(Args&&... args)
{
    // Get the type of the first element in the paramater pack, if it's a std::ostream,
    // then we'll write the message to that. Otherwise, use std::cout.
    using tempTuple = std::tuple<Args...>;
    tempTuple paramPack { args... };

    using firstElementType = std::tuple_element<0, tempTuple>;

    std::ostream& out = std::cout;
    if constexpr(std::is_same<std::ostream&, firstElementType>::value)
    {
        out = std::get<0>(paramPack);
    }
    
    ((out << args), ...);
    out << std::endl;
}

/* Describes how long nonvolatile status effects may last (number of turns).
   -1 means the effect lasts indefinitely until cured. */
// using StatusMap = std::unordered_map<Status, int>;

// const StatusMap Statuses {
//     {Status::None, -1},
//     {Status::Burn, -1},
//     {Status::Freeze, -1},
//     {Status::Paralysis, -1},
//     {Status::Poison, -1},
//     {Status::Sleep, 3}
// };

// using VolatileMap = std::unordered_map<Volatile, int>;

// const VolatileMap Volatiles {
//     {Volatile::Bound, -1}
// };

} // namespace pokemon