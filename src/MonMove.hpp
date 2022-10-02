#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <typeinfo>
#include <vector>

#include "MonType.hpp"
#include "MoveComponent.hpp"

namespace pokemon 
{
    // forward declarations
    class Battle;
    class Mon;

    struct MoveStats
    {
        MoveStats() = default;
        explicit MoveStats(const MonType& moveType, int pp, int power, int accuracy, int priority)
            : m_Type(moveType), m_PP(pp), m_Power(power), m_Accuracy(accuracy), m_Priority(priority)
        { }

        MonType m_Type;
        int m_PP;
        int m_Power;
        int m_Accuracy;
        int m_Priority;
    };

    class MonMove
    {
        friend class Battle;
        friend class Mon;
        friend struct BattleEvent;

    public:
        MonMove() = default;
        MonMove(const MonMove& other) = default;
        MonMove(MonMove&&) = default;
        MonMove& operator=(const MonMove&) = default;
        MonMove& operator=(MonMove&&) = default;
        ~MonMove() = default;

        // args must be object(s) derived from MoveComponent
        template<typename... Args>
        explicit MonMove(const MonType& moveType, 
                        int pp, 
                        int power, 
                        int accuracy, 
                        int priority, 
                        Args&&... args)
            : m_PP(pp), m_Power(power), m_Accuracy(accuracy), m_Priority(priority)
        { 
            (m_Components.emplace_back(new Args(args)), ...); 
        }

        // args must be object(s) derived from MoveComponent
        template<typename... Args>
        void addComponents(Args&&... args)
        {
            (m_Components.emplace_back(new Args(args)), ...);
        }
        
        void doMove(Battle& battle, Mon& attacker);
        std::function<void (Battle&, Mon&)> getMoveEvent();

    private:

    public:
        std::vector<std::shared_ptr<MoveComponent>> m_Components;
        int m_PP;
        int m_Power;
        int m_Accuracy;
        int m_Priority;
    };
} // namespace pokemon