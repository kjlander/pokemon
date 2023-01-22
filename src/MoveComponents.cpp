#include "MoveComponents.hpp"
#include "MonUtilities.hpp"

namespace pokemon 
{
void CompAttack::execute(Battle& battle, const Mon& mon, const MonMove& owningMove)
{
    monLog("CompAttack::execute() called");
    battle.getOpponent(mon).receiveAttack(owningMove.m_Power);
}

void CompWeather::execute(Battle& battle, const Mon& mon, const MonMove& owningMove)
{
    monLog("CompWeather::execute() called");
}

void CompStatus::execute(Battle& battle, const Mon& mon, const MonMove& owningMove)
{
    monLog("CompStatus::execute() called");
    battle.inflictStatus<Status>(mon, battle.getOpponent(mon), m_Status);
}
} // namespace pokemon