#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

#include "MonMove.hpp"
#include "MonType.hpp"
#include "MonUtilities.hpp"

namespace pokemon 
{
// forward declarations
class Battle;
class MonType;
class MonMove;

class Mon
{
    using voidType = MonType();
    friend class Battle;
    friend class BattleEvent;
    friend float calcAttackModifier(const Mon&, const Mon&);
    friend void swap(Mon& lhs, Mon& rhs);

public:
    Mon() : m_Type1(MonType()), 
            m_Type2(MonType()), 
            m_Name(""), 
            m_Nickname(""),
            m_Status(Status::None),
            m_MaxHp(0),
            m_CurrHp(0),
            m_Speed(0) { }

    explicit Mon(const MonType& type1,
                    const MonType& type2,
                    const std::string& name,
                    uint16_t maxHp,
                    int speed,
                    const std::string& nickname="",
                    Status status=Status::None,
                    int maxMoves=4)
                    : m_Type1(type1),
                    m_Type2(type2),
                    m_Name(name),
                    m_Nickname(nickname),
                    m_Status(status),
                    m_MaxHp(maxHp),
                    m_CurrHp(maxHp),
                    m_MaxMoves(maxMoves),
                    m_Speed(speed),
                    m_Concious(true) { }
    
    Mon(const Mon& other) = default;
    Mon(Mon&& other) = default;
    Mon& operator=(const Mon& other) = default;
    Mon& operator=(Mon&& other) = default;

    // possibly remove this method eventually
    void doMove(Battle& battle, int moveIndex, Mon& attacker);
    
    void receiveAttack(int attackPwr);

    void receiveFractionDmg(float fraction);

    const std::string& getName() const noexcept 
    { return m_Nickname.empty() ? m_Nickname : m_Name; }

    std::function<void (Battle&, Mon&)> getMoveEvent(int moveIndex);
    Status status() { return m_Status; }

    void setStatus(const Status& status) noexcept
    { m_Status = status; }
    
    template<typename... Args>
    void addMove(Args&&... args)
    {
        // ensures the maximum number of moves will not be exceeded by this call
        if (sizeof...(args) <= m_MaxMoves - m_Moves.size())
        {
            (m_Moves.push_back(std::forward<Args>(args)), ...);
        }
        else 
        {
            std::cout << "Can't have more than " << m_MaxMoves << " moves!" << std::endl;
        }
    }


private:
    void faint() noexcept;

private:
    std::vector<MonMove> m_Moves;
    MonType m_Type1;
    MonType m_Type2;
    std::string m_Name;
    std::string m_Nickname;
    Status m_Status;
    int m_MaxHp;
    int m_CurrHp;
    int m_MaxMoves;
    int m_Speed;
    bool m_Concious;
};

} // namespace pokemon