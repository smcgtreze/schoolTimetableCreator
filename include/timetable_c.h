#ifndef TIMETABLE_C_H
#define TIMETABLE_C_H

#include <QObject>
#include <QDateTime>

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
    const Timetable_c &generate( const QString &teacherName );
    workDay_t getDay( const dayOfWeek day );
    void calculateTimetable();
    QString toCSV() const;
    QString csvHeader() const;
    QTime getTotalWorkday( const workDay_t &day );
    const int getTotalWorkWeekHours();
private:
    QString _teacherName;
    std::array< workDay_t, daysInAWeek > _workWeek;
};

#endif // TIMETABLE_C_H
