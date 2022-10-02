#include "MoveComponents.hpp"

namespace pokemon 
{
    void CompAttack::execute(Battle& battle, const Mon& mon, const MonMove& owningMove)
    {
        std::cout << "CompAttack::execute() called" << std::endl;
        battle.getOpponent(mon).receiveAttack(owningMove.m_Power);
    }

    void CompWeather::execute(Battle& battle, const Mon& mon, const MonMove& owningMove)
    {
        std::cout << "CompWeather::execute() called" << std::endl;
    }
} // namespace pokemon