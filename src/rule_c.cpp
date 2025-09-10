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

    template< typename type >
    const std::unordered_map< std::string_view, std::function< bool( type, type ) > > operationsMap = {
        { "equal", []( type a, type b) { return a == b; } },
        { "greaterThan", std::greater<type>() },
        { "lesserThan", std::less<type>() }
    };

}

Rule_c::Rule_c( int id ): _id( id ) {}

const QString Rule_c::description(){
    return u"Rule "_s + QString::number( _id );
}

const int Rule_c::id(){
    return _id;
}

bool Rule_c::operator()( const std::string_view& op,int a, int b )
{
    auto it = ::operationsMap< int >.find( op );
    if ( it != ::operationsMap< int >.end() ) {
        bool result = filterByRule( it->second, a, b );
        return result;
    }
    qDebug() << u"Unknown operation"_s;
    return false;
}
