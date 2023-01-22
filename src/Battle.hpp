#pragma once

#include <cstdlib>
#include <functional>
#include <memory>
#include <random>
#include <sys/types.h>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Mon.hpp"
#include "MonUtilities.hpp"

namespace pokemon
{
// forward declarations
class Battle;

class BattleEvent
{
    friend class Battle;
    friend void swap(BattleEvent& lhs, BattleEvent& rhs);

public:
    BattleEvent(Mon& attacker, int moveIndex)
        : m_Target(attacker.getMoveEvent(moveIndex)), m_Attacker(attacker)
        {
            // copy priority and name for easy access for sorting
            m_Priority = (m_Attacker.get().m_Moves[moveIndex]).m_Priority;
            m_Name = (m_Attacker.get().m_Moves[moveIndex]).m_Name;
        }

    BattleEvent(const BattleEvent& other) = default;
    BattleEvent(BattleEvent&& other) = default;
    BattleEvent& operator=(const BattleEvent& other) = default;
    BattleEvent& operator=(BattleEvent&& other) = default;
    ~BattleEvent() { monLog("BattleEvent Destructor Called"); }

    // We need these operators to support sorting
    bool operator<(const BattleEvent& other) const;
    bool operator>(const BattleEvent& other) const;

    void doEvent(Battle& battle) noexcept
    {
        monLog(m_Attacker.get().m_Name, " used ", m_Name, "!"); 
        m_Target(battle, m_Attacker);
    }

    const std::string& getAttackerName() const noexcept
    {
        return m_Attacker.get().getName();
    }
    

private:
    int m_Priority;
    std::reference_wrapper<Mon> m_Attacker;
    std::function<void (Battle&, Mon&)> m_Target;
    std::string m_Name;
};

class Battle
{
public:
    Battle(Mon& mon1, Mon& mon2)
        : m_Mon1(mon1), m_Mon2(mon2) { }
    // Battle& operator=(const Battle&);
    // Battle& operator=(Battle&&);

    // possibly remove
    Mon& getMon1() noexcept { return m_Mon1; }
    Mon& getMon2() noexcept { return m_Mon2; }

    // determine who is the opponent
    Mon& getOpponent(const Mon& mon) noexcept;

    void initCombatants(Mon& mon1, Mon& mon2);

    void beforeMove(Mon& mon);

    void afterMove(Mon& mon)
    {
        struct Statuses& statuses = mon.m_Statuses;
        if (statuses.Bound)
        {
            // TODO
            statuses.Bound--;
            mon.receiveFractionDmg(1/16.0f);
        }
    }

    template<typename T, typename... Args, typename commonT = std::common_type<Args...>>
    void inflictStatus(const Mon& attacker, Mon& target, const T& status, Args... args)
    {
        if constexpr(std::is_same<T, Status>::value)
        {
            switch (status)
            {
                case Status::None:
                    break;
                case Status::Burn:
                    target.setStatus(Status::Burn);
                    monLog(target.getName(), " was BURNED!");
                    break;
                case Status::Freeze:
                    target.setStatus(Status::Freeze);
                    monLog(target.getName(), " was FROZEN!");
                    break;
                case Status::Paralysis:
                    target.setStatus(Status::Paralysis);
                    monLog(target.getName(), " was PARALYZED!");
                    break;
                case Status::Poison:
                    target.setStatus(Status::Poison);
                    monLog(target.getName(), " was POISONED!");
                    break;
                case Status::Sleep:
                    target.setStatus(Status::Sleep);
                    std::size_t packSize = sizeof...(args);
                    if (packSize == 2 && std::is_same<commonT, int>::value)
                    {
                        std::tuple<int, int> tup(args...);
                        target.m_Statuses.NVtimer = setTurnTimer(std::get<0>(tup), std::get<1>(tup));
                    }
                    else
                    {
                        // Default to between 1 and 3 turns of sleep if not specified.
                        target.m_Statuses.NVtimer = setTurnTimer(1, 3);
                    }
                    monLog(target.getName(), " is ASLEEP!");
                    break;
            }
        }
        else if constexpr(std::is_same<T, Volatile>::value)
        {
            std::size_t packSize = sizeof...(args);
            commonT arg = {args...};
            switch (status)
            {
                case Volatile::Bound:
                    if (attacker.hasItem(Item::GripClaw))
                    {
                        target.m_Statuses.Bound = 5;
                    }
                    else
                    {
                        target.m_Statuses.Bound = setTurnTimer(2, 5);
                    }
                    // Need to keep track of the attacker, if they exit battle
                    // the status needs to be removed.
                    target.m_Statuses.Binder = std::make_shared<Mon>(attacker);
                    monLog(target.getName(), " has been BOUND!");
                    
                    break;

                case Volatile::CantEscape:
                    target.m_Statuses.CantEscape = true;
                    monLog(target.getName(), " CAN'T ESCAPE!");
                    break;

                case Volatile::Confusion:
                    target.m_Statuses.Confusion = setTurnTimer(2, 5);
                    monLog(target.getName(), " is CONFUSED!");
                    break;

                case Volatile::Curse:
                    if (target.m_Statuses.Curse == true)
                    {
                        monLog(target.getName(), " is already CURSED!");
                    }
                    else
                    {
                        target.m_Statuses.Curse = true;
                        monLog(target.getName(), " has been CURSED!");
                    }
                    break;

                /* ARCEUS ONLY
                case Volatile::Drowsy:
                    // An int is used since Drowsy behaves like a timed status, inflicting Sleep
                    // on the end of the next turn if no other non-volatiles are active.
                    if (target.m_Statuses.Drowsy == 0)
                    {
                        target.m_Statuses.Drowsy = 2;
                    }
                    break;
                */

                /* GEN 4
                case Volatile::Embargo:
                    if (target.m_Statuses.Embargo == 0)
                    {
                        target.m_Statuses.Embargo = 5;
                    }
                    break;
                */

                case Volatile::Encore:
                    if (target.m_Statuses.Encore == 0 && target.m_Statuses.MagicCoat == false)
                    {
                        target.m_Statuses.Encore = setTurnTimer(4, 8);
                    }
                    break;

                case Volatile::Flinch:
                    if ((attacker.m_Item == Item::KingsRock || attacker.m_Item == Item::RazorFang) 
                        && arg == 1)
                    {
                        target.m_Statuses.Flinch = 1;
                        monLog(target.getName(), " is FLINCHING!");
                    }
                    break;

                /* GEN 4
                case Volatile::HealBlock:
                    if (target.m_Statuses.HealBlock != 0)
                    {
                        target.m_Statuses.HealBlock = 5;
                        monLog(target.getName(), " was prevented from healing!");
                    }
                    else 
                    {
                        monLog(target.getName(), " is already HEAL BLOCKED!");
                    }
                    break;
                */

                case Volatile::Identified:
                    target.m_Statuses.Identified = true;
                    monLog(target.getName(), " has been IDENTIFIED!");
                    break;

                case Volatile::Infatuation:
                    if (!(target.m_Statuses.Infatuator == nullptr))
                    {
                        target.m_Statuses.Infatuator = std::make_shared<Mon>(attacker);
                        monLog(target.getName(), " is INFATUATED by ", attacker.getName());
                    }
                    break;

                case Volatile::LeechSeed:
                    if (target.m_Type1 != Type::Grass && target.m_Type2 != Type::Grass)
                    {
                        target.m_Statuses.LeechSeed = true;
                        target.m_Statuses.LeechSeeder = std::make_shared<Mon>(attacker);
                        monLog(target.getName(), " was seeded!");
                    }
                    else
                    {
                        monLog(target.getName(), " is immune to LEECH SEED!");
                    }
                    break;

                case Volatile::Nightmare:
                    if (target.m_Statuses.NVStatus == Status::Sleep)
                    {
                        target.m_Statuses.Nightmare = true;
                        monLog(target.getName(), " fell into a NIGHTMARE!");
                    }
                    else 
                    {
                        monLog(target.getName(), " is not asleep!");
                    }
                    break;

                case Volatile::PerishSong:
                    // if (!(target.m_Ability == Ability::Soundproof))
                    // Perish Song cannot reset itself.
                    if (target.m_Statuses.PerishSong == 0)
                    {
                        target.m_Statuses.PerishSong = 3;
                        monLog("All affected POKéMON will faint in 3 turns!");
                    }
                    break;

                case Volatile::Taunt:
                    target.m_Statuses.Taunt = setTurnTimer(2, 4);
                    monLog(target.getName(), " fell for the TAUNT!");
                    break;

                /* GEN 5
                case Volatile::Telekinesis:
                    if (target.m_Statuses.Telekinesis == 0)
                    {
                        target.m_Statuses.Telekinesis = 3;
                        monLog(target.getName(), " was hurled into the air!");
                    }
                    break;
                */

                case Volatile::Torment:
                    target.m_Statuses.Torment = true;
                    monLog(target.getName(), " was subjected to TORMENT!");
                    break;

                case Volatile::TypeChange:
                    if (std::is_same<commonT, Type>::value)
                        target.m_Statuses.TypeChange = arg;
                    break;
            }
        }
    }

    void applyAttackMon1(int attackPwr);
    void makeEvent(Mon& mon, int moveIndex);
    void doEvents();
    void orderEvents();
    void handleStatuses();

private:
    void applyStatusDamage(const Status&, Mon& target);
    Status fetchStatus(const Status& status);
    uint8_t setTurnTimer(int min, int max);
    bool percentChance(int odds);

private:
    std::vector<BattleEvent> m_EventStack;
    Mon& m_Mon1;
    Mon& m_Mon2;
};

} // namespace pokemon