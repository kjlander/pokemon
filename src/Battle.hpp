#pragma once

#include <functional>
#include <vector>

#include "Mon.hpp"

namespace pokemon
{
    // forward declarations
    class Battle;

    struct BattleEvent
    {
        friend class Battle;
        friend void swap(BattleEvent& lhs, BattleEvent& rhs);

        BattleEvent(int moveIndex, Mon& attacker)
            : m_Target(attacker.getMoveEvent(moveIndex)), m_Attacker(attacker)
            {
                // copy priority for easy access for sorting
                m_Priority = (m_Attacker.m_Moves[moveIndex]).m_Priority;
            }

        BattleEvent(const BattleEvent& other);
        BattleEvent(BattleEvent&& other) noexcept;

        BattleEvent& operator=(const BattleEvent& other);
        BattleEvent& operator=(BattleEvent&& other) noexcept;
        bool operator<(const BattleEvent &other) const;

        void doEvent(Battle& battle) { m_Target(battle, m_Attacker); }

    private:
        std::function<void (Battle&, Mon&)> m_Target;
        Mon& m_Attacker;
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
        Mon& getOpponent(const Mon &mon) noexcept;

        void initCombatants(const Mon &mon1, const Mon &mon2);
        void applyAttackMon1(int attackPwr);
        void makeEvent(Mon &mon, int moveIndex);
        void doEvents();
        void orderEvents();

    private:
        Mon& m_Mon1;
        Mon& m_Mon2;
        std::vector<BattleEvent> m_EventStack;
    };

} // namespace pokemon