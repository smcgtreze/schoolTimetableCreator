#include "mainWindow.h"
#include "./ui_mainWindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>

#include "mainWindow.h"
#include "./ui_mainWindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup Teacher Table
    ui->teacherTable->setColumnCount(1);
    ui->teacherTable->setHorizontalHeaderLabels({"Teacher Name"});
    ui->teacherTable->setRowCount(1);
    ui->teacherTable->setItem(0, 0, new QTableWidgetItem());

    // Setup Rules Table
    ui->rulesTable->setColumnCount(3);
    ui->rulesTable->setHorizontalHeaderLabels({"Op", "A", "B"});
    ui->rulesTable->setRowCount(1);
    for (int j = 0; j < 3; ++j)
        ui->rulesTable->setItem(0, j, new QTableWidgetItem());

    // Connect dynamic row expansion
    connect(ui->teacherTable, &QTableWidget::itemChanged, this, &MainWindow::onTeacherTableChanged);
    connect(ui->rulesTable, &QTableWidget::itemChanged, this, &MainWindow::onRulesTableChanged);

    // Connect Generate Button
    connect(ui->generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGenerateClicked() {
    int teacherRows = ui->teacherTable->rowCount();
    for (int i = 0; i < teacherRows - 1; ++i) {
        auto item = ui->teacherTable->item(i, 0);
        if (!item || item->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "All teacher names must be filled.");
            return;
        }
    }

    int rulesRows = ui->rulesTable->rowCount();
    for (int i = 0; i < rulesRows - 1; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto item = ui->rulesTable->item(i, j);
            if (!item || item->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, "Input Error", "All rule fields must be filled.");
                return;
            }
        }
    }

    QMessageBox::information(this, "Success", "Timetable data received successfully!");

    // Reset flags so new rows can be added again
    teacherRowExpanded = false;
    rulesRowExpanded = false;
}

void MainWindow::onTeacherTableChanged(QTableWidgetItem *item) {
    int row = item->row();
    if (row == ui->teacherTable->rowCount() - 1 && !item->text().trimmed().isEmpty()) {
        ui->teacherTable->blockSignals(true);
        ui->teacherTable->insertRow(row + 1);
        ui->teacherTable->setItem(row + 1, 0, new QTableWidgetItem());
        ui->teacherTable->blockSignals(false);
    }
}

void MainWindow::onRulesTableChanged(QTableWidgetItem *item) {
    int row = item->row();
    if (row == ui->rulesTable->rowCount() - 1) {
        bool filled = true;
        for (int j = 0; j < ui->rulesTable->columnCount(); ++j) {
            auto cell = ui->rulesTable->item(row, j);
            if (!cell || cell->text().trimmed().isEmpty()) {
                filled = false;
                break;
            }
        }

        if (filled) {
            ui->rulesTable->blockSignals(true);
            int newRow = row + 1;
            ui->rulesTable->insertRow(newRow);
            for (int j = 0; j < ui->rulesTable->columnCount(); ++j)
                ui->rulesTable->setItem(newRow, j, new QTableWidgetItem());
            ui->rulesTable->blockSignals(false);
        }
    }
}
