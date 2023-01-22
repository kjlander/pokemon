#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>


#include "Mon.hpp"
#include "MonUtilities.hpp"

namespace pokemon
{
// forward declarations
class Battle;

// Mon::Mon(const Mon& other)
//     : m_Type1(other.m_Type1),
//       m_Type2(other.m_Type2),
//       m_Name(other.m_Name),
//       m_Nickname(other.m_Nickname),
//       m_MaxHp(other.m_MaxHp),
//       m_CurrHp(other.m_CurrHp),
//       m_MaxMoves(other.m_MaxMoves),
//       m_Faint(other.m_Faint) { }

// Mon::Mon(Mon&& other) noexcept
//     : m_Type1(std::move(other.m_Type1)),
//       m_Type2(std::move(other.m_Type2)),
//       m_Name(std::move(other.m_Name)),
//       m_Nickname(std::move(other.m_Nickname)),
//       m_MaxHp(std::move(other.m_MaxHp)),
//       m_CurrHp(std::move(other.m_CurrHp)),
//       m_MaxMoves(std::move(other.m_MaxMoves)),
//       m_Faint(std::move(other.m_Faint)) { }

// Mon& Mon::operator=(const Mon& other) noexcept
// {
//     if (this != &other)
//     {
//         Mon ret;
//         ret.m_Type1 = other.m_Type1;
//         ret.m_Type2 = other.m_Type2;
//         ret.m_Name = other.m_Name;
//         ret.m_Nickname = other.m_Nickname;
//         ret.m_MaxHp = other.m_MaxHp;
//         ret.m_CurrHp = other.m_CurrHp;
//         ret.m_MaxMoves = other.m_MaxMoves;
//         ret.m_Faint = other.m_Faint;
//     }
//     return *this;
// }

// Mon& Mon::operator=(Mon&& other) noexcept
// {
//     if (this != &other)
//     {
//         m_Moves    = std::move(other.m_Moves);
//         m_Type1    = std::move(other.m_Type1);
//         m_Type2    = std::move(other.m_Type2);
//         m_Name     = std::move(other.m_Name);
//         m_Nickname = std::move(other.m_Nickname);
//         m_MaxHp    = std::move(other.m_MaxHp);
//         m_CurrHp   = std::move(other.m_CurrHp);
//         m_MaxMoves = std::move(other.m_MaxMoves);
//         m_Faint    = std::move(other.m_Faint);
//     }
//     return *this;
// }

// StatusManager

// bool StatusManager::update() noexcept
// {
//     if (!m_IsTimed)
//         return false;
//     if (m_IsTimed && m_Turns > 0) 
//     { 
//         --m_Turns; 
//     }
//     if (m_Turns == 0) 
//     { 
//         return true; 
//     }
//     return false;
// }

// // Resets the member variables to a newly provided status.
// // This was split into a new function so that its caller can catch
// // an error if this throws.
// void StatusManager::fetchStatus(const Status& status)
// {
//     m_Status = status;

//     StatusMap::const_iterator iter = m_StatusMap->find(m_Status);
//     if (iter != m_StatusMap->end()) 
//     {
//         m_Turns = iter->second;
//     }
//     else 
//     {
//         throw std::invalid_argument("Invalid status supplied to StatusManager::fetchStatus()");
//     }
// }

// // Calls fetchStatus and finishes the setup work by resetting
// // the timer if applicable.
// void StatusManager::assign(const Status& status)
// {
//     try 
//     {
//         fetchStatus(status);
//     } 
//     catch (std::invalid_argument ex) 
//     {
//         // if this happened, something is wrong
//         std::cerr << ex.what() << std::endl;
//         std::exit(-1);
//     }   

//     // -1 indicates that this Status does not have a timer
//     if (m_Turns != -1)
//     {
//         m_IsTimed = true;
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> distr(1, m_Turns);
//         m_Turns = distr(gen);
//     }
//     else 
//     { 
//         m_IsTimed = false; 
//     }
// }

// Mon

void Mon::heal(int hp)
{
    int newHp = hp + m_CurrHp;
    m_CurrHp = newHp < m_MaxHp ? newHp : m_MaxHp;
}

void Mon::doMove(Battle& battle, int moveIndex, Mon& attacker)
{
    m_Moves[moveIndex].doMove(battle, attacker);
}

bool Mon::hasItem(const Item &item) const noexcept
{
    return m_Item == item ? true : false;
}

void Mon::receiveAttack(int attackPwr)
{
    if (m_CurrHp > attackPwr)
    {
        m_CurrHp -= attackPwr;
    }
    else 
    {
        faint();
    }
}

// Returns the amount of damage applied.
int Mon::receiveFractionDmg(float fraction)
{
    int dmg = round(m_MaxHp * fraction);
    if (m_CurrHp > dmg) 
    { 
        m_CurrHp -= dmg;
        return dmg; 
    }
    else 
    { 
        int finalHp = m_CurrHp;
        faint();
        // 1 is the minimum that should be returned.
        return finalHp > 1 ? finalHp : 1;
    }
}

std::function<void (Battle&, Mon&)> Mon::getMoveEvent(int moveIndex)
{
    return m_Moves[moveIndex].getMoveEvent();
}

void Mon::faint() noexcept
{
    m_CurrHp = 0;
    m_Concious = false;
    monLog(getName(), " fainted!");
}

void swap(Mon& lhs, Mon& rhs)
{
    using std::swap;
    swap(lhs.m_Moves, rhs.m_Moves);
    swap(lhs.m_Type1, rhs.m_Type1);
    swap(lhs.m_Type2, rhs.m_Type2);
    swap(lhs.m_Name, rhs.m_Name);
    swap(lhs.m_Nickname, rhs.m_Nickname);
    swap(lhs.m_MaxHp, rhs.m_MaxHp);
    swap(lhs.m_CurrHp, rhs.m_CurrHp);
    swap(lhs.m_MaxMoves, rhs.m_MaxMoves);
    swap(lhs.m_Speed, rhs.m_Speed);
    swap(lhs.m_Concious, rhs.m_Concious);
}

// void Mon::update() noexcept
// {
//     // StatusManager::update() will return true if a status timer has expired.
//     // In this case, we need to print a message indicating what happened.
//     if (m_NVStatus.update())
//     {
//         switch (m_NVStatus.name())
//         {
//             // If the status was Sleep, print a message and reset status to
//             // None.
//             case Status::Sleep:
//                 std::cout << getName() << " woke up!" << std::endl;
//                 m_NVStatus.assign(Status::None);
//                 break;

//             default:
//                 break;
//         }
//     }
// }

} // namespace pokemon