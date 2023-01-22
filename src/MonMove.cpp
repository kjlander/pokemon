#include <functional>

#include "MonMove.hpp"
#include "MoveComponent.hpp"

namespace pokemon 
{
// forward declarations
class Battle;

using namespace std::placeholders;

void MonMove::doMove(Battle& battle, Mon& attacker)
{
    for (std::shared_ptr<MoveComponent>& component : m_Components)
    {
        component->execute(battle, attacker, *this);
    }
}

std::function<void (Battle&, Mon&)> MonMove::getMoveEvent()
{
    return std::bind(&MonMove::doMove, this, _1, _2);
}

} // namespace pokemon