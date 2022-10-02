#pragma once

#include <functional>
#include <iostream>

#include "Battle.hpp"
#include "MoveComponent.hpp"


namespace pokemon 
{
    // forward declarations
    class Mon;

    class CompAttack : public MoveComponent
    {
    public:
        CompAttack() { }

        virtual void execute(Battle&, const Mon&, const MonMove&) override;
    };

    class CompWeather : public MoveComponent
    {
    public:
        CompWeather() { }

        virtual void execute(Battle&, const Mon&, const MonMove&) override;

    private:
        int m_Weather;
    };

} // namespace pokemon