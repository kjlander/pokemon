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

class CompStatus : public MoveComponent
{
public:
    CompStatus(const Status& status) { m_Status = status; }

    virtual void execute(Battle&, const Mon&, const MonMove&) override;

private:
    Status m_Status;
};

} // namespace pokemon