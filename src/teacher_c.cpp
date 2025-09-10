#include "teacher_c.h"

Teacher_c::Teacher_c(const QString &name, int age)
    : _name(name), _age(age) {}

Teacher_c::Teacher_c( const QString& csvLine) {
    QStringList parts = csvLine.split(',');
    if (parts.size() != 2)
        throw std::invalid_argument("Invalid CSV format");

    _name = parts[0];
    _age = parts[1].toInt();
}

const QString &Teacher_c::name(){
    return _name;
}

const int Teacher_c::age(){
    return _age;
}

QString Teacher_c::toCSV() const {
    return QString( "%1,%2" ).arg( _name ).arg( _age );
}

QString Teacher_c::csvHeader() const {
    return "Name,Age";
}
