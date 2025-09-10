#include "rule_c.h"
#include <functional>
#include <qdebug.h>

using namespace Qt::StringLiterals;

namespace{
    template< typename Op,typename type >
    requires std::is_arithmetic< type >::value
    bool filterByRule( Op op, type x, type y ){
        return op( x, y );
    }
}

Rule_c::Rule_c( int id, const std::string_view& op, const std::shared_ptr< std::function<int() > > getA, const int b ): _id( id ),_op( op ), _a( getA ), _b( b ) {}

const QString Rule_c::description(){
    return u"Rule "_s + QString::number( _id );
}

const int Rule_c::id(){
    return _id;
}

const std::string_view &Rule_c::op(){
    return _op;
}

const int Rule_c::a(){
    if ( !_a ) {
        qDebug() << "Error: _a is not initialized!";
        return -1;
    }
    return (*_a)();
}

const int Rule_c::b(){
    return _b;
}

bool Rule_c::operator()()
{
    auto it = operationsMap< int >.find( _op );
    if ( it != operationsMap< int >.end() ) {
        const int a_value = a();
        bool result = filterByRule( it->second, a_value, _b );
        return result;
    }
    qDebug() << u"Unknown operation"_s;
    return false;
}
