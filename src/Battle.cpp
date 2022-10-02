#include <algorithm>

#include "Battle.hpp"

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
        for (auto &event : m_EventStack) 
        {
            event.doEvent(*this);
        }
    }

    void Battle::makeEvent(Mon &mon, int moveIndex) 
    {
        m_EventStack.emplace_back(moveIndex, mon);
    }

    void Battle::applyAttackMon1(int attackPwr) 
    {
        m_Mon1.receiveAttack(attackPwr);
    }

    void Battle::initCombatants(const Mon &mon1, const Mon &mon2) 
    {
        m_Mon1 = mon1;
        m_Mon2 = mon2;
    }

    Mon& Battle::getOpponent(const Mon &mon) noexcept
    {
        if (&m_Mon1 == &mon)
            return m_Mon2;

        return m_Mon1;
    }

    void Battle::orderEvents()
    {
        // first sort events by move priority
        std::sort(m_EventStack.begin(), m_EventStack.end(), 
       [&](const BattleEvent& a, const BattleEvent& b) 
            { return a < b; });

        // sort elements by combatant speed
        std::stable_sort(m_EventStack.begin(), m_EventStack.end(),
       [&](const BattleEvent& a, const BattleEvent& b)
            { return a.m_Attacker.m_Speed < b.m_Attacker.m_Speed; });
    }

    BattleEvent::BattleEvent(const BattleEvent& other)
        : m_Target(other.m_Target), 
          m_Attacker(other.m_Attacker), 
          m_Priority(other.m_Priority) { }

    BattleEvent::BattleEvent(BattleEvent&& other) noexcept
        : m_Target(std::move(other.m_Target)), 
          m_Attacker(other.m_Attacker), 
          m_Priority(std::move(other.m_Priority)) { }

    BattleEvent& BattleEvent::operator=(const BattleEvent& other)
    {
        if (this != &other)
        {
            m_Target = other.m_Target;
            m_Attacker = other.m_Attacker;
            m_Priority = other.m_Priority;
        }
        return *this;
    }

    BattleEvent& BattleEvent::operator=(BattleEvent&& other) noexcept
    {
        if (this != &other)
        {
            m_Target = std::move(other.m_Target);
            m_Attacker = std::move(other.m_Attacker);
            m_Priority = std::move(other.m_Priority);
        }
        return *this;
    }

    bool BattleEvent::operator<(const BattleEvent& other) const 
    {
        return m_Priority < other.m_Priority ? true : false;
    }

    void swap(BattleEvent& lhs, BattleEvent& rhs)
    {
        using std::swap;
        swap(lhs.m_Target, rhs.m_Target);
        swap(lhs.m_Attacker, rhs.m_Attacker);
        swap(lhs.m_Priority, rhs.m_Priority);
    }

} // namespace pokemon