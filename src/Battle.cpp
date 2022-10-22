#include <algorithm>

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

void Battle::doEvents() 
{
    for (auto& event : m_EventStack) 
    {
        switch (event.m_Attacker.get().status())
        {
            case Status::None : 
                event.doEvent(*this);
                break;

            case Status::Burn : 
                event.m_Attacker.get().receiveFractionDmg(1.0f/16);
                if (event.m_Attacker.get().m_Concious)
                    event.doEvent(*this);
                break;

            case Status::Freeze : 
                std::cout << event.getAttackerName() << " is frozen!" << std::endl;
                break;

            case Status::Paralysis :
                std::cout << event.getAttackerName() << " is paralyzed!" << std::endl;
                break;

            case Status::Poison : 
                event.m_Attacker.get().receiveFractionDmg(1.0f/8);
                if (event.m_Attacker.get().m_Concious)
                    event.doEvent(*this);
                break;

            case Status::Sleep : 
                std::cout << event.getAttackerName() << " is asleep!" << std::endl;
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

void Battle::initCombatants(const Mon& mon1, const Mon& mon2) 
{
    m_Mon1 = mon1;
    m_Mon2 = mon2;
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