#include <cstdlib>
#include <functional>

#include "Mon.hpp"

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

    void Mon::doMove(Battle &battle, int moveIndex, Mon& attacker)
    {
        m_Moves[moveIndex].doMove(battle, attacker);
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

    std::function<void (Battle&, Mon&)> Mon::getMoveEvent(int moveIndex)
    {
        return m_Moves[moveIndex].getMoveEvent();
    }

    void Mon::faint() noexcept
    {
        m_CurrHp = 0;
        m_Faint = true;
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
        swap(lhs.m_Faint, rhs.m_Faint);
    }
} // namespace pokemon