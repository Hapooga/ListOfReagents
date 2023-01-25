#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DBFacade.h"
#include "ExcelFile.h"

#include <QDebug>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_add_pushButton_clicked();

    void on_category_comboBox_activated(int index);

private:

    void SetCategoryComboBox(const QStringList& sheetsNames);
    void InstallTableViewColumnWidth();


    Ui::MainWindow *ui;
    DBFacade *db_facade;
};
#endif // MAINWINDOW_H
