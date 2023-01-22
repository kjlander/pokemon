#pragma once

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include "MonMove.hpp"
#include "MonType.hpp"
#include "MonUtilities.hpp"

namespace pokemon 
{

// forward declarations
class Battle;
class MonType;
class MonMove;

/* 
StatusManager objects' primary purpose is keeping track of status effects
that have a "timer" in the form of a number of turns that they last. It 
handles randomly choosing a number of turns a status should last (within
a specified range), and reporting when a status effect expires via the
bool returned by the update() method.
*/

// template<typename TStatus>
// class StatusManager
// {
// public:
//     StatusManager(const Status& status, const StatusMap<TStatus>* map=&NVStatus) 
//         : m_Status(status), m_StatusMap(map)
//     { 
//         assign(m_Status);
//     }

//     void assign(const Status& status);

//     const Status& name() const noexcept
//     { return m_Status; }

//     /* This function should be called every turn. It increments the class' 
//        internal turn timer (if the status has a timer) and returns a bool
//        indicating the status effect has worn off (true) or the effect is
//        still in effect (false). In the case of status effects that are not
//        timed, false will always be returned. */
//     bool update() noexcept;

// private:
//     void fetchStatus(const Status& status);

// private:
//     Status m_Status;
//     const StatusMap<TStatus>* m_StatusMap; 
//     int m_Turns;
//     bool m_IsTimed;
// };


class Mon
{
    friend class Battle;
    friend class BattleEvent;
    friend float calcAttackModifier(const Mon&, const Mon&);
    friend void swap(Mon& lhs, Mon& rhs);

public:
    Mon() : m_Type1(Type::None), 
            m_Type2(Type::None), 
            m_Name(""), 
            m_Nickname(""),
            //m_Statuses(Status::None),
            m_MaxHp(0),
            m_CurrHp(0),
            m_Speed(0) { }

    explicit Mon(Type type1,
                 Type type2,
                 const std::string& name,
                 uint16_t maxHp,
                 int speed,
                 const std::string& nickname="",
                 //Status status=Status::None,
                 int maxMoves=4)
                     : m_Type1(type1),
                       m_Type2(type2),
                       m_Name(name),
                       m_Nickname(nickname),
                       //m_Statuses(status),
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

    bool hasItem(const Item& item) const noexcept;

    void receiveAttack(int attackPwr);

    int receiveFractionDmg(float fraction);

    void heal(int hp);

    const std::string& getName() const noexcept 
    { return m_Nickname.empty() ? m_Name : m_Nickname; }

    std::function<void (Battle&, Mon&)> getMoveEvent(int moveIndex);

    // const Status& status() const noexcept 
    // { return m_Statuses; }

    void setStatus(const Status& status) noexcept
    { m_Statuses.NVStatus = status; }

    void update() noexcept;
    
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
            monLog(&std::cerr, " Can't have more than ", m_MaxMoves, " moves!");
            exit(EXIT_FAILURE);
        }
    }


private:
    void faint() noexcept;

private:
    std::vector<MonMove> m_Moves;
    Type m_Type1;
    Type m_Type2;
    std::string m_Name;
    std::string m_Nickname;
    struct Statuses m_Statuses;
    Ability m_Ability;
    Item m_Item;
    int m_MaxHp;
    int m_CurrHp;
    int m_MaxMoves;
    int m_Speed;
    bool m_Concious;
};

} // namespace pokemon