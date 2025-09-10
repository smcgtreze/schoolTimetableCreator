#ifndef RULE_C_H
#define RULE_C_H

#include <qobject.h>

class Rule_c
{
public:
    Rule_c( int id );
    const QString description();
    const int id();
    bool operator()( const std::string_view& op,int a, int b );

private:
    const int _id;
};

#endif // RULE_C_H
