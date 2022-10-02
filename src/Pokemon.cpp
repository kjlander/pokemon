#include <algorithm>
#include <iostream>
#include <memory>
#include <type_traits>

#include "Battle.hpp"
#include "Mon.hpp"
#include "MonMove.hpp"
#include "MonType.hpp"
#include "MoveComponent.hpp"
#include "MoveComponents.hpp"
#include "MonUtilities.hpp"

namespace pokemon
{
    using typeMatchup = std::set<std::string>;

    /*
    float calcAttackModifier(const Mon& attacker, const Mon& defender)
    {
        
        MonType atkType1 = attacker.m_Type1;
        MonType atkType2 = attacker.m_Type2;
        auto defenderNVEBegIter = defender.m_Type1.m_NVEffective.cbegin();
        auto defenderNVEEndIter = defender.m_Type1.m_NVEffective.cend();
        float modifier = 1.0f;
        if (std::search(defenderNVEBegIter, 
                        defenderNVEEndIter, 
                        atkType1) != defenderNVEEndIter)
        {
            modifier *= 2.0f;
        }
    }
*/
}

int main()
{
    using namespace pokemon;
    MonType grass("grass", grassSE, grassNVE);
    MonType fire("fire", fireSE, fireNVE);

    Mon charmander(fire, MonType(), "charmander", 100, 1);
    Mon bulbasaur(grass, MonType(), "bulbasaur", 100, 0);

    MonMove tackle(grass, 35, 40, 100, 0, CompAttack());
    bulbasaur.addMove(tackle);
    charmander.addMove(tackle);

    Battle battle(bulbasaur, charmander);
    battle.getMon1().doMove(battle, 0, bulbasaur);

    battle.makeEvent(bulbasaur, 0);
    battle.doEvents();
    // std::cout << "This is Pokemon!" << std::endl;

    return 0;

}