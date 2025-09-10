#include "mainWindow.h"
#include "master.h"
#include "timetable_c.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SchoolTimetableCreator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    auto calculateData = [](){
        Master_c master;
        Teacher_c teacher{ master.addTeacher( "teacher1", 45 ) };
        Rule_c rule { master.addRule() };

        qDebug() << rule.description() << ":" << rule( "lesserThan", teacher.age(), 50 );

        Timetable_c timetable{ master.addTimetable( teacher.name() ) };
        timetable.calculateTimetable();

        qDebug() << rule.description() << ":" << rule( "equal", timetable.getTotalWorkWeekHours() , 40 );

        for( const auto &dayOfWeek : dayOfWeeks ){
            qDebug() << rule.description() << ":" << rule( "lesserThan", timetable.getTotalWorkday( timetable.getDay( dayOfWeek ) ).hour() , 9 );
            qDebug() << rule.description() << ":" << rule( "greaterThan", timetable.getTotalWorkday( timetable.getDay( dayOfWeek ) ).hour() , 4 );
        }

        master.displayRule( rule.id() );
        master.displayTeacher( teacher.name() );
    };
    calculateData();

    return a.exec();
}
