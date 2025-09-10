#include "master.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QStandardPaths>

namespace{
    template<typename T>
    void writeToCSV(const QString& filename, const T& object )
    {
        QFile file( filename );
        bool fileExists = QFileInfo::exists( filename );

        if ( !file.open( QIODevice::Append | QIODevice::Text ) ) {
            qWarning( "Unable to open file: %s", qUtf8Printable( filename ) );
            return;
        }

        QTextStream out( &file );

        // Write header if file is new
        if ( !fileExists ) {
            out << object.csvHeader() << "\n";
        }

        // Write object data
        out << object.toCSV() << "\n";

        file.close();
    }

    template<typename T>
    QVector<T> readFromCSV(const QString& filename)
    {
        QVector<T> result;
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning("Unable to open file: %s", qUtf8Printable(filename));
            return result;
        }

        QTextStream in(&file);
        bool isFirstLine = true;

        while (!in.atEnd()) {
            QString line = in.readLine();

            // Skip header
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }

            try {
                T obj(line); // Construct directly from CSV line
                result.append(obj);
            } catch (const std::exception& e) {
                qWarning("Failed to parse line: %s", qUtf8Printable(line));
            }

        }

        file.close();
        return result;
    }


    const QString filePath( const QString &objectName ){
        return QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) + "/" +  objectName + u".csv"_s;
    }

}

Master_c::Master_c(QObject *parent)
    : QObject{parent}
{}

int Master_c::_ruleId = 0;

Teacher_c &Master_c::addTeacher( const QString &name, const int age )
{
    auto [ it, inserted ] = _teachers.try_emplace(name, std::make_shared<Teacher_c>(name, age));
    if ( !inserted )
        qDebug() << "Failed to insert teacher " << name;

    ::writeToCSV( filePath( name ), *it->second );

    return *( it->second );
}

Rule_c &Master_c::addRule()
{
    auto [ it, inserted ] = _rules.try_emplace( _ruleId, std::make_shared<Rule_c>( _ruleId ));
    if ( !inserted )
        qDebug() << "Failed to insert rule ";
    _ruleId++;

    return *( it->second );
}

void Master_c::displayTeacher( const QString &name )
{
    try {
        const auto &teacher{ _teachers.at( name ) };
        qDebug() << teacher->name() << "," << teacher->age();
    } catch ( std::out_of_range exception ) {
        qDebug() << "No element with name " << name;
    }
}

void Master_c::displayRule( const int id )
{
    try {
        const auto &rule{ _rules.at( id ) };
        qDebug() << rule->id() << "," << rule->description();
    } catch ( std::out_of_range exception ) {
        qDebug() << "No element with id " << id;
    }

}

Timetable_c &Master_c::addTimetable( const QString &teacherName )
{
    auto [ it, inserted ] = _timetables.try_emplace( teacherName, std::make_shared< Timetable_c >(teacherName ));
    if ( !inserted )
        qDebug() << "Failed to insert teacher " << teacherName;

    it->second->calculateTimetable();
    ::writeToCSV( filePath( u"Timetable" + teacherName ), *it->second );

    return *( it->second );
}
