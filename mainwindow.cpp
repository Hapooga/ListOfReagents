#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>


// упаковка штука коробка пакет ведро банка аликвоты набор


// больше 500 мкл     меньше 500 мкл    целая     меньше 100 мкл     больше 100 мкл
// меньше 20 мкл    больше 20 мкл  20 мкл  5 мкл    50 мкл   10 мкл     аликвоты
// больше 120 мкл   меньше 120 мкл   100 мкл


// добавить категорию
// удалить категорию




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    db_facade = new DBFacade(ui->tableView, this);

    QString fileName = "C:\\QTProjects\\List.xlsx";
    ExcelFile *excelFile = new ExcelFile(fileName, this);
    //CreateTables();

    db_facade->CreateDBFromExcelFile(excelFile);


    excelFile->CreateExcelDocument(excelFile->GetSheets(), "C:\\QTProjects\\List2.xlsx");

    SetCategoryComboBox(db_facade->GetTablesNamsFromDB());

    InstallTableViewColumnWidth();


    qDebug() << "end";
}



void MainWindow::SetCategoryComboBox(const QStringList& sheetsNames) {
        ui->category_comboBox->addItems(sheetsNames);
}

void MainWindow::InstallTableViewColumnWidth(){

    ui->tableView->setColumnWidth(0, 10);
    ui->tableView->setColumnWidth(1, 0);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 90);
    ui->tableView->setColumnWidth(6, 200);
    ui->tableView->setColumnWidth(7, 100);
    ui->tableView->setColumnWidth(8, 80);
    ui->tableView->setColumnWidth(9, 100);
    ui->tableView->setColumnWidth(10, 240);
    ui->tableView->setColumnWidth(11, 240);

}





MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_pushButton_clicked() {

    qDebug() << ui->names_comboBox->currentText();
    qDebug() << ui->article_number_lineEdit->text();
    qDebug() << ui->maker_comboBox->currentText();
    qDebug() << ui->amount_spinBox->text();
    qDebug() << ui->amount_comboBox->currentText();
    qDebug() << ui->residue_doubleSpinBox->text();
    qDebug() << ui->residue_comboBox->currentText();
    qDebug() << ui->expiration_date_dateEdit->text();
    qDebug() << ui->location_comboBox->currentText();
    qDebug() << ui->comment_lineEdit->text();


   db_facade->AddRow();
}


void MainWindow::on_category_comboBox_activated(int index) {
    QString tableName = db_facade->GetTablesNamsFromDB()[index];
    db_facade->LoadTable(tableName);
    ui->names_comboBox->addItems(db_facade->GetProductNamesOfSheet(tableName));
    ui->maker_comboBox->addItems(db_facade->GetProductMakerOfSheet(tableName));
    ui->location_comboBox->addItems(db_facade->GetProductLocationOfSheet(tableName));
    InstallTableViewColumnWidth();
}

