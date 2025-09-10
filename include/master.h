#ifndef MASTER_H
#define MASTER_H

#include <QObject>

#include "rule_c.h"
#include "teacher_c.h"
#include "timetable_c.h"

using namespace Qt::StringLiterals;

class Master_c : public QObject
{
    Q_OBJECT
public:
    explicit Master_c(QObject *parent = nullptr);

    Teacher_c &addTeacher( const QString&, const int );
    Rule_c &addRule( const std::string_view& op, const std::shared_ptr< std::function<int()> > getA, const int b );
    Timetable_c &addTimetable( const QString &teacherName, const QVector< QString > &oldContent );
    void displayTeacher( const QString& );
    void displayRule( const int );
    void calculateData();

private:
    static int _ruleId;
    std::unordered_map< QString, std::shared_ptr< Teacher_c > > _teachers;
    std::unordered_map< int, std::shared_ptr< Rule_c > > _rules;
    std::unordered_map< QString, std::shared_ptr< Timetable_c > > _timetables;

    void calculateTimetable();

signals:
};

#endif // MASTER_H
