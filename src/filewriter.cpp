#include "filewriter.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

template<typename T>
void writeToCSV(const QString& filename, const T& object)
{
    QFile file(filename);
    bool fileExists = QFileInfo::exists(filename);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning("Unable to open file: %s", qUtf8Printable(filename));
        return;
    }

    QTextStream out(&file);

    // Write header if file is new
    if (!fileExists) {
        out << object.csvHeader() << "\n";
    }

    // Write object data
    out << object.toCSV() << "\n";

    file.close();
}

fileWriter::fileWriter() {}

