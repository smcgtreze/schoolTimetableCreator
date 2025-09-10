#include "timetable_c.h"

#include <ranges>

using namespace Qt::StringLiterals;

namespace{
    int secondsSinceMidnight(const QTime& time) {
        return time.hour() * 3600 + time.minute() * 60 + time.second();
    }
}

Timetable_c::Timetable_c( const QString &teacherName ){
    _teacherName = teacherName;
}

void Timetable_c::calculateTimetable()
{
    for( const int day : std::views::iota( 0, static_cast<int>( dayOfWeek::Count ) ) )
        _workWeek[ day ] = { QTime{ 9,0 }, QTime{ 17,0 } };
}

workDay_t Timetable_c::getDay( const dayOfWeek day )
{
    return _workWeek[ static_cast< int >( day ) ];
}

QString Timetable_c::toCSV() const {
    QStringList workWeek;
    for( const auto &day : _workWeek )
        workWeek.append( day.first.toString() + " - " + day.second.toString() );

    return workWeek.join( ',' );
}

QString Timetable_c::csvHeader() const {
    return u"Monday,Tuesday,Wednesday,Thursday,Friday"_s;
}

void trimDayEnd( workDay_t &day, const QTime &end )
{
    day.second = ( day.second > end ) ? day.second : end;
}

void trimDayStart( workDay_t &day, const QTime &start )
{
    day.first = ( day.first < start ) ? day.first : start;
}

QTime Timetable_c::getTotalWorkday( const workDay_t &day )
{
    QTime duration( 0,0 );
    duration = duration.addSecs( day.first.secsTo( day.second ) );
    return duration;
}

const int Timetable_c::getTotalWorkWeekHours()
{
    unsigned int seconds = 0;
    for( const auto &day : _workWeek )
        seconds += secondsSinceMidnight( getTotalWorkday( day ) );

    return seconds / 3600;
}


