#pragma once

#include <string>

namespace pokemon 
{
    // foward declarations
    class Battle;
    class Mon;

    class MonMove;
    class MoveComponent
    {
    public:
        // MoveComponent() = default;
        // MoveComponent(const MoveComponent&) = default;
        // MoveComponent(MoveComponent&&) = default;
        // MoveComponent& operator=(const MoveComponent&) = default;
        // MoveComponent& operator=(MoveComponent&&) = default;
        
        virtual void execute(Battle& battle, const Mon& mon, const MonMove& owningMove) = 0;
        // virtual ~MoveComponent() { }
    };

} // namespace pokemon