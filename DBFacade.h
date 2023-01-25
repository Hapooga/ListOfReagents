#ifndef DBFACADE_H
#define DBFACADE_H

#include <QtSql/QtSql>
#include <QObject>
#include <set>

#include "ExcelFile.h"

class QTableView;
class QSqlTableModel;

class DBFacade : public QObject {

    Q_OBJECT

public:

    DBFacade(QTableView* table, QObject *parent = nullptr);

    void CreateDBFromExcelFile(const ExcelFile* const excelFile);

    QStringList GetTablesNamsFromDB();




    void LoadTable(const SheetName& name);

    void AddRow();

    virtual ~DBFacade();


signals:

public slots:
protected slots:
private slots:

// =================================================================================
private:
   QStringList GetListOfUniqueElements(const SheetName& sheetName, const QString& query) const;
public:
   QStringList GetProductNamesOfSheet(const SheetName& sheetName) const;
   QStringList GetProductMakerOfSheet(const SheetName& sheetName) const;
   QStringList GetProductLocationOfSheet(const SheetName& sheetName) const;
// =================================================================================
private:

   QTableView* table;

   QSqlDatabase database;

   QSqlQuery *query;

   QSqlRecord record;

   QSqlTableModel *model;

   QStringList tablesNames;

};








#endif // DBFACADE_H
