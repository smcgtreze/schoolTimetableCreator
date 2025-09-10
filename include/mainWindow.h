#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtablewidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool teacherRowExpanded = false;
    bool rulesRowExpanded = false;
    void onGenerateClicked();
    void onRulesTableChanged(QTableWidgetItem *item);
    void onTeacherTableChanged(QTableWidgetItem *item);
};
#endif // MAINWINDOW_H
