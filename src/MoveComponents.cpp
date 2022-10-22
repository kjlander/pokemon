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

void CompStatus::execute(Battle& battle, const Mon& mon, const MonMove& owningMove)
{
    std::cout << "CompStatus::execute() called" << std::endl;
    battle.getOpponent(mon).setStatus(m_Status);
}
} // namespace pokemon