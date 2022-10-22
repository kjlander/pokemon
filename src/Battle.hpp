#pragma once

#include <functional>
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
    ~BattleEvent() { std::cout << "BattleEvent Destructor Called" << std::endl; }

    // We need these operators to support sorting
    bool operator<(const BattleEvent& other) const;
    bool operator>(const BattleEvent& other) const;

    void doEvent(Battle& battle) 
    { 
        m_Target(battle, m_Attacker);
        std::cout << m_Attacker.get().m_Name << " used " << m_Name << "!" << std::endl; 
    }

    const std::string& getAttackerName() const noexcept
    {
        return m_Attacker.get().getName();
    }
    

private:
    std::function<void (Battle&, Mon&)> m_Target;
    std::string m_Name;
    std::reference_wrapper<Mon> m_Attacker;
    int m_Priority;
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

    void initCombatants(const Mon& mon1, const Mon& mon2);
    void applyAttackMon1(int attackPwr);
    void makeEvent(Mon& mon, int moveIndex);
    void doEvents();
    void orderEvents();

private:
    void applyStatusDamage(const Status&, Mon& target);

private:
    std::vector<BattleEvent> m_EventStack;
    Mon& m_Mon1;
    Mon& m_Mon2;
};

} // namespace pokemon