#pragma once

#include <set>

#include "MonUtilities.hpp"

namespace pokemon 
{
class Mon;

class MonType
{
    using typeMatchup = std::set<std::string>;
    friend float calcAttackModifier(const Mon&, const Mon&);
public:
    MonType() : m_TypeName("void"), m_SEffective(), m_NVEffective() { }
    explicit MonType(const std::string& name, 
                    const typeMatchup& s_effective,
                    const typeMatchup& nv_effective) :
                    m_TypeName(name), 
                    m_SEffective(s_effective), 
                    m_NVEffective(nv_effective) { }
    
    MonType(const MonType&) = default;
    MonType(MonType&&) = default;
    MonType& operator=(const MonType&) = default;
    MonType& operator=(MonType&&) = default;
    ~MonType() = default;

private:
    std::string m_TypeName;
    typeMatchup m_SEffective;
    typeMatchup m_NVEffective;
    typeMatchup m_NoEffect;
};

} // namespace pokemon