#include "timetable_c.h"
#include "rule_c.h"

#include <ranges>
#include <numeric>

using namespace Qt::StringLiterals;

namespace{
    int secondsSinceMidnight(const QTime& time) {
        return time.hour() * 3600 + time.minute() * 60 + time.second();
    }
    constexpr int halfAnHour = 1800;

    bool hasTimedOut(std::chrono::steady_clock::time_point start, int timeoutMs) {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= timeoutMs;
    }
}

Timetable_c::Timetable_c( const QString &teacherName ){
    _teacherName = teacherName;
}

Timetable_c::Timetable_c( const QString &teacherName, const QVector<QString> &oldContent ){
    _teacherName = teacherName;

    std::transform(oldContent.begin(), oldContent.end(), _workWeek.begin(),
                   [](const QString &oldString) {
                       const auto timeStrings = oldString.split(',');
                       return QPair<QTime, QTime>(
                           QTime::fromString(timeStrings.value(0), "hh:mm"),
                           QTime::fromString(timeStrings.value(1), "hh:mm")
                           );
                   });
}


void Timetable_c::calculateDraftTimetable()
{
    for( const int day : std::views::iota( 0, static_cast<int>( dayOfWeek::Count ) ) )
        _workWeek[ day ] = { QTime{ 9,0 }, QTime{ 17,0 } };

}

void Timetable_c::calculateTimetable( const std::unordered_map< int, std::shared_ptr< Rule_c > > &rules )
{
    auto start = std::chrono::steady_clock::now();
    const int timeoutMs = 30000;

    qDebug() << "Starting calculation of timetable";

    for( auto ruleIt = rules.begin(); ruleIt != rules.end(); ruleIt++ ){
        Rule_c &rule = *ruleIt->second;
        while( !rule() ){
            auto it = operationsMap< int >.find( rule.op() );
            for( auto &workDay : _workWeek ){
                const int totalWeekHours( getTotalWorkWeekHours() );
                if ( it != operationsMap< int >.end() && it->first == "lesserThan" )
                    trimDayEnd( workDay, workDay.second.addSecs( -halfAnHour ) );
                else if ( it != operationsMap< int >.end() && it->first == "greaterThan" )
                    extendDayEnd( workDay, workDay.second.addSecs( halfAnHour ) );
                else if ( it != operationsMap< int >.end() && it->first == "equal"
                         && rule.b() != totalWeekHours )
                {
                    rule.b() <= totalWeekHours ? trimDayEnd( workDay, workDay.second.addSecs( -halfAnHour ) ) :
                        extendDayEnd( workDay, workDay.second.addSecs( halfAnHour ) );
                }

                qDebug() << "Checking rule " << rule.a() << "/" << rule.b() << "-" << rule.op();

                if( rule() )
                    break;
                else if( ::hasTimedOut( start, timeoutMs ) )
                {
                    qDebug() << "Has timed out... ";
                    break;
                }
            }
        }
    }
    qDebug() << "Finished calculating timetable";
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

void Timetable_c::trimDayEnd( workDay_t &day, const QTime &end )
{
    day.second = ( day.second < end ) ? day.second : end;
}

void Timetable_c::trimDayStart( workDay_t &day, const QTime &start )
{
    day.first = ( day.first > start ) ? day.first : start;
}

void Timetable_c::extendDayEnd( workDay_t &day, const QTime &end )
{
    day.second = ( day.second > end ) ? day.second : end;
}

void Timetable_c::extendDayStart( workDay_t &day, const QTime &start )
{
    day.first = ( day.first < start ) ? day.first : start;
}

const QTime Timetable_c::getTotalWorkday( const workDay_t &day ) const
{
    QTime duration( 0,0 );
    duration = duration.addSecs( day.first.secsTo( day.second ) );
    return duration;
}

const int Timetable_c::getTotalWorkWeekHours() const
{
    unsigned int seconds = 0;
    for( const auto &day : _workWeek )
        seconds += secondsSinceMidnight( getTotalWorkday( day ) );

    return seconds / 3600;
}

void Timetable_c::displayTimetable(){
    for( const auto &week : _workWeek )
        qDebug() << week.first << "," << week.second;
}


