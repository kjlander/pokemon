#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <type_traits>

#include <assert.h>

#include "Battle.hpp"
#include "MonUtilities.hpp"

namespace pokemon 
{
// Battle& Battle::operator=(const Battle& other)
// {
//     if (this != &other)
//     {
//         m_Mon1 = other.m_Mon1;
//         m_Mon2 = other.m_Mon2;
//         m_EventStack = other.m_EventStack;
//     }
//     return *this;
// }

// Battle& Battle::operator=(Battle&& other)
// {
//     if (this != &other)
//     {
//         m_Mon1 = std::move(other.m_Mon1);
//         m_Mon2 = std::move(other.m_Mon2);
//         m_EventStack = std::move(other.m_EventStack);
//     }
//     return *this;
// }

void Battle::beforeMove(Mon &mon)
{
    struct Statuses& statuses = mon.m_Statuses;

    if (statuses.NVStatus == Status::Freeze)
    {
        if (percentChance(20))
        {
            statuses.NVStatus = Status::None;
            monLog(mon.getName(), " has thawed!");
        }
    }
    if (statuses.NVStatus == Status::Sleep)
    {
        uint8_t& timer = statuses.NVtimer;
        if (timer > 0)
            --timer;
        else
            monLog(mon.getName(), " woke up!");
    }
    if (statuses.Curse)
    {
        mon.receiveFractionDmg(1 / 4.0f);
        monLog(mon.getName(), " took CURSE damage!");
    }
    if (statuses.LeechSeed)
    {
        int healAmount = mon.receiveFractionDmg(1 / 8.0f);
        // If the mon who was leech seeded has Liquid Ooze, the attacker gets damaged
        // instead of healed.
        if (mon.m_Ability != Ability::LiquidOoze)
        {
            statuses.LeechSeeder->heal(healAmount);
            monLog(mon.getName(), "'s health is sapped by LEECH SEED!");
        }
        else
        {
            statuses.LeechSeeder->receiveAttack(healAmount);
            monLog(mon.getName(), " has ability LIQUID OOZE!");
            monLog(statuses.LeechSeeder->getName(), " received damage from LIQUID OOZE!");
        }    
    }
    if (statuses.Nightmare && statuses.NVStatus == Status::Sleep)
    {
        mon.receiveFractionDmg(1 / 4.0f);
        monLog(mon.getName(), " is locked in a NIGHTMARE!");
    }
    if (statuses.Confusion)
    {
        monLog(mon.getName(), " is CONFUSED!");
        statuses.Confusion--;
        if (percentChance(50))
        {
            mon.receiveAttack(40);
            monLog(mon.getName(), " hurt itself in CONFUSION!");
        }
    }
    if (statuses.Drowsy)
    {
        statuses.Drowsy--;
        if (statuses.Drowsy == 0)
        {
            // A default constructed Mon is given since there is no attacker information
            // to pass here.
            inflictStatus(Mon(), mon, Status::Sleep);
        }
    }
    if (statuses.Flinch)
    {
        statuses.Flinch--;
        monLog(mon.getName(), " FLINCHED!");
    }
}


void Battle::doEvents()
{
    for (BattleEvent& event : m_EventStack) 
    {
        beforeMove(event.m_Attacker);
        
        //event.m_Attacker.get().update();
        switch (event.m_Attacker.get().m_Statuses.NVStatus)
        {
            case Status::None: 
                event.doEvent(*this);
                break;

            case Status::Burn: 
                monLog(event.getAttackerName(), " is hurt by its burn!");
                event.m_Attacker.get().receiveFractionDmg(1.0f/16);
                if (event.m_Attacker.get().m_Concious)
                {
                    event.doEvent(*this);
                }
                break;

            case Status::Freeze: 
                monLog(event.getAttackerName(), " is frozen solid!");
                break;

            case Status::Paralysis:
                monLog(event.getAttackerName(), " is paralyzed! It can't move!");
                break;

            case Status::Poison: 
                monLog(event.getAttackerName(), " is hurt by poison!");
                event.m_Attacker.get().receiveFractionDmg(1.0f/8);
                if (event.m_Attacker.get().m_Concious)
                {
                    event.doEvent(*this);
                }
                break;

            case Status::Sleep: 
                monLog(event.getAttackerName()," is asleep!");
                if (event.m_Name == "Snore" || event.m_Name == "Sleep Talk")
                {
                    event.doEvent(*this);
                }
                break;
        }
    }
}

void Battle::makeEvent(Mon& mon, int moveIndex) 
{
    m_EventStack.emplace_back(mon, moveIndex);
}

void Battle::applyAttackMon1(int attackPwr) 
{
    m_Mon1.receiveAttack(attackPwr);
}

// Status Battle::fetchStatus(const Status& status)
// {
//     StatusMap::const_iterator iter = Statuses.find(status);
//     if (iter != Statuses.end()) 
//     {
//         return status;
//     }
//     else 
//     {
//         throw std::invalid_argument("Invalid status supplied to StatusManager::fetchStatus()");
//     }
// }

void Battle::initCombatants(Mon& mon1, Mon& mon2) 
{
    m_Mon1 = mon1;
    m_Mon2 = mon2;
}

uint8_t Battle::setTurnTimer(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

// Returns true within the chance specified out of 100.
// For 50/50 odds, chance should be 50.
bool Battle::percentChance(int chance)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);
    return distr(gen) > chance;
}

Mon& Battle::getOpponent(const Mon& mon) noexcept
{
    if (&m_Mon1 == &mon)
        return m_Mon2;

    return m_Mon1;
}

void Battle::orderEvents()
{
    // first sort events by move priority
    std::sort(m_EventStack.rbegin(), m_EventStack.rend(), 
    [](const BattleEvent& a, const BattleEvent& b) 
        { return a < b; });

    // then stable sort elements by combatant speed (speed only influences order
    // when move priority is equal)
    std::stable_sort(m_EventStack.rbegin(), m_EventStack.rend(),
    [](const BattleEvent& a, const BattleEvent& b)
        { return a.m_Attacker.get().m_Speed < b.m_Attacker.get().m_Speed; });
}


// BattleEvent::BattleEvent(const BattleEvent& other)
//     : m_Target(other.m_Target), 
//       m_Attacker(other.m_Attacker),
//       m_Name(other.m_Name),
//       m_Priority(other.m_Priority) 
//       { std::cout << "BattleEvent copy constructed" << std::endl; }

// BattleEvent::BattleEvent(BattleEvent&& other) noexcept
//     : m_Target(std::move(other.m_Target)), 
//       m_Attacker(std::move(other.m_Attacker)),
//       m_Name(std::move(other.m_Name)),
//       m_Priority(std::move(other.m_Priority)) 
//       { std::cout << "BattleEvent move assigned" << std::endl; }

// BattleEvent& BattleEvent::operator=(const BattleEvent& other)
// {
//     std::cout << "BattleEvent copy assigned" << std::endl;
//     //BattleEvent ret;
//     if (this != &other)
//     {
//         m_Target = other.m_Target;
//         m_Attacker = other.m_Attacker;
//         m_Name = other.m_Name,
//         m_Priority = other.m_Priority;
//     }
//     return *this;
// }

// BattleEvent& BattleEvent::operator=(BattleEvent&& other) noexcept
// {
//     std::cout << "BattleEvent move assigned" << std::endl;
//     if (this != &other)
//     {
//         m_Target = std::move(other.m_Target);
//         m_Attacker = std::move(other.m_Attacker);
//         m_Name = std::move(other.m_Name),
//         m_Priority = std::move(other.m_Priority);
//     }
//     return *this;
// }

bool BattleEvent::operator<(const BattleEvent& other) const 
{
    return m_Priority < other.m_Priority ? true : false;
}

bool BattleEvent::operator>(const BattleEvent& other) const
{
    return !(this < &other);
}

void swap(BattleEvent& lhs, BattleEvent& rhs)
{
    std::cout << "BattleEvent swap" << std::endl;
    using std::swap;
    swap(lhs.m_Target, rhs.m_Target);
    swap(lhs.m_Attacker, rhs.m_Attacker);
    swap(lhs.m_Name, rhs.m_Name);
    swap(lhs.m_Priority, rhs.m_Priority);
}

} // namespace pokemon