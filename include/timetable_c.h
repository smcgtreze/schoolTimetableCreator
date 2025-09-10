#ifndef TIMETABLE_C_H
#define TIMETABLE_C_H

#include <QObject>
#include <QDateTime>

#include "rule_c.h"

using workDay_t = std::pair< QTime, QTime >;
constexpr size_t daysInAWeek = 5;

enum class dayOfWeek{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Count
};

constexpr std::array<dayOfWeek, daysInAWeek> dayOfWeeks = {
    dayOfWeek::Monday,
    dayOfWeek::Tuesday,
    dayOfWeek::Wednesday,
    dayOfWeek::Thursday,
    dayOfWeek::Friday };

class Timetable_c
{
public:
    Timetable_c( const QString &name );
    Timetable_c( const QString &teacherName, const QVector<QString> &oldContent );
    const Timetable_c &generate( const QString &teacherName );
    workDay_t getDay( const dayOfWeek day );
    void calculateDraftTimetable();
    void calculateTimetable( const std::unordered_map< int, std::shared_ptr< Rule_c > > &rules );
    void displayTimetable();
    QString toCSV() const;
    QString csvHeader() const;
    const QTime getTotalWorkday( const workDay_t &day ) const;
    const int getTotalWorkWeekHours() const;
    void trimDayEnd( workDay_t &day, const QTime &end );
    void trimDayStart( workDay_t &day, const QTime &start );
    void extendDayEnd( workDay_t &day, const QTime &end );
    void extendDayStart( workDay_t &day, const QTime &start );
private:
    QString _teacherName;
    std::array< workDay_t, daysInAWeek > _workWeek;
};

#endif // TIMETABLE_C_H
