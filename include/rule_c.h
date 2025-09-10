#ifndef RULE_C_H
#define RULE_C_H

#include <qobject.h>

template< typename type >
const std::unordered_map< std::string_view, std::function< bool( type, type ) > > operationsMap = {
    { "equal", []( type a, type b) { return a == b; } },
    { "greaterThan", std::greater<type>() },
    { "lesserThan", std::less<type>() }
};

class Rule_c
{
public:
    Rule_c( int id, const std::string_view& op, const std::shared_ptr< std::function<int()> > getA, const int b );
    const QString description();
    const int id();
    const std::string_view &op();
    const int a();
    const int b();
    bool operator()();

private:
    const int _id;
    std::string_view _op;
    const std::shared_ptr< std::function<int()> > _a;
    int _b;
};

#endif // RULE_C_H
