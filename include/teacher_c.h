#ifndef TEACHER_C_H
#define TEACHER_C_H

#include <qobject.h>

class Teacher_c
{
public:
    Teacher_c( const QString &name, const int age );
    Teacher_c( const QString& csvLine );

    const QString &name();
    const int age();
    QString toCSV() const;
    QString csvHeader() const;

private:
    QString _name;
    int _age;
};

#endif // TEACHER_C_H
