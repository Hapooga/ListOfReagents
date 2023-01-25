#include "DBFacade.h"
#include <QTableView>


DBFacade::DBFacade(QTableView* _table, QObject *parent) :
    QObject(parent), table(_table)
{

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("ListOfReagents.sqlite");

    if(!database.open()){
        qDebug() << database.lastError().text();
    }

    query = new QSqlQuery(database);

    if (!database.tables().contains("list_of_reagents")){
        query->exec("CREATE TABLE 'list_of_reagents' ("
                        "'id'	INTEGER NOT NULL UNIQUE,"
                        "'category'	TEXT NOT NULL,"
                        "'name'	TEXT NOT NULL,"
                        "'article_number'	TEXT UNIQUE,"
                        "'amount'	INTEGER NOT NULL,"
                        "'type_amount'	TEXT NOT NULL,"
                        "'maker'	TEXT NOT NULL,"
                        "'expiration_date'	TEXT NOT NULL,"
                        "'residue'	NUMERIC NOT NULL,"
                        "'type_residue'	TEXT NOT NULL,"
                        "'location'	TEXT NOT NULL,"
                        "'comment'	TEXT,"
                        "PRIMARY KEY('id' AUTOINCREMENT)"
                    ");"
             );
    }

    model = new QSqlTableModel(this, database);

    tablesNames = database.tables();
    tablesNames.pop_front();

    if(!tablesNames.isEmpty()) {
    model->setTable(tablesNames.front());
    }
    model->select();
   //model->setHeaderData(0, Qt::Horizontal, tr("#"));
    model->setHeaderData(1, Qt::Horizontal, tr("Категория"));
    model->setHeaderData(2, Qt::Horizontal, tr("Наименование"));
    model->setHeaderData(3, Qt::Horizontal, tr("Артикул"));
    model->setHeaderData(4, Qt::Horizontal, tr("Кол-во"));
    model->setHeaderData(5, Qt::Horizontal, tr("Ед. измерения"));
    model->setHeaderData(6, Qt::Horizontal, tr("Производитель"));
    model->setHeaderData(7, Qt::Horizontal, tr("Срок годности"));
    model->setHeaderData(8, Qt::Horizontal, tr("Остаток"));
    model->setHeaderData(9, Qt::Horizontal, tr("Ед. измерения"));
    model->setHeaderData(10, Qt::Horizontal, tr("Местонахождение"));
    model->setHeaderData(11, Qt::Horizontal, tr("Комментарии"));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    table->setModel(model);


}


void DBFacade::LoadTable(const SheetName& name){
    model->setTable(name);
    model->select();
    // убрать дублирование
    model->setHeaderData(1, Qt::Horizontal, tr("Категория"));
    model->setHeaderData(2, Qt::Horizontal, tr("Наименование"));
    model->setHeaderData(3, Qt::Horizontal, tr("Артикул"));
    model->setHeaderData(4, Qt::Horizontal, tr("Кол-во"));
    model->setHeaderData(5, Qt::Horizontal, tr("Ед. измерения"));
    model->setHeaderData(6, Qt::Horizontal, tr("Производитель"));
    model->setHeaderData(7, Qt::Horizontal, tr("Срок годности"));
    model->setHeaderData(8, Qt::Horizontal, tr("Остаток"));
    model->setHeaderData(9, Qt::Horizontal, tr("Ед. измерения"));
    model->setHeaderData(10, Qt::Horizontal, tr("Местонахождение"));
    model->setHeaderData(11, Qt::Horizontal, tr("Комментарии"));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    table->setModel(model);
}


void ReLoadDBNames() {
    //tablesNames = database.tables();
    //tablesNames.pop_front();
}


// методы получения из конкретной таблицы уникального содержимого столбца
// ================================================================================
QStringList DBFacade::GetListOfUniqueElements(const SheetName& sheetName, const QString& queryStr) const {
    QStringList list;

    std::set<QString> setBuffer; // для фильтрации уникальных значений
    QString buffer;
    if(query->exec(queryStr)) {
        while (query->next()) {
            buffer = query->value(0).toString();
            if(not buffer.isEmpty()) {
            setBuffer.insert(std::move(buffer));
            }
        }
    }

    return {begin(setBuffer), end(setBuffer)};
}
// -------------------------------------------------------------------------------
QStringList DBFacade::GetProductNamesOfSheet(const SheetName& sheetName) const {
    return GetListOfUniqueElements(sheetName, "SELECT name FROM '" + sheetName + "';");
}
// -------------------------------------------------------------------------------
QStringList DBFacade::GetProductMakerOfSheet(const SheetName& sheetName) const {
    return GetListOfUniqueElements(sheetName,"SELECT maker FROM " + sheetName);
}
// -------------------------------------------------------------------------------
QStringList DBFacade::GetProductLocationOfSheet(const SheetName& sheetName) const {
     return GetListOfUniqueElements(sheetName,"SELECT location FROM " + sheetName);
}
// ================================================================================


QStringList DBFacade::GetTablesNamsFromDB() { return tablesNames; }
//QStringList DBFacade::GetProductNames() { return productNames};


QString CreateInsertQuery(const SheetName& name, const Sheet& sheet) {

    QString query = "INSERT INTO '" + name + "' ("
    "id, category, name, article_number, amount, maker, expiration_date, residue,"
    " type_residue, type_residue, location, comment) "
    "VALUES ('";

    bool isNotLast = true;
    ushort firstNotTitle = 1;
    const ushort LASTROW = sheet.size() - 1;
    const ushort LASTCOL = sheet[0].size() - 1;

    for(ushort ROW = firstNotTitle; ROW < sheet.size(); ++ROW) {
        isNotLast = true;
        for(ushort COL = 0; COL < sheet[ROW].size(); ++COL) {
            query += sheet[ROW][COL];
            if(isNotLast) {
                query += "','";
                if(COL == LASTCOL && ROW != LASTROW) {
                    query += "'), ('";
                   isNotLast = false;
                }
                if (COL == LASTCOL && ROW == LASTROW) {
                    query += "')";
                }

            }
        }
    }

    return query;
}





void DBFacade::CreateDBFromExcelFile(const ExcelFile* const excelFile) {

    for(const auto& [sheetName, sheet] : excelFile->GetSheets()) {

        if (!database.tables().contains(sheetName)){
            query->exec("CREATE TABLE '" + sheetName + "' ("
                            "'id'	INTEGER NOT NULL UNIQUE,"
                            "'category'	TEXT NOT NULL,"
                            "'name'	TEXT NOT NULL,"
                            "'article_number'	TEXT UNIQUE,"
                            "'amount'	INTEGER NOT NULL,"
                            "'type_amount'	TEXT NOT NULL,"
                            "'maker'	TEXT NOT NULL,"
                            "'expiration_date'	TEXT NOT NULL,"
                            "'residue'	NUMERIC NOT NULL,"
                            "'type_residue'	TEXT NOT NULL,"
                            "'location'	TEXT NOT NULL,"
                            "'comment'	TEXT,"
                            "PRIMARY KEY('id' AUTOINCREMENT)"
                        ");"
                 );


        }

        QString queryString = CreateInsertQuery(sheetName, sheet);
    }
}


void DBFacade::AddRow() {

    model->insertRow(model->rowCount());

}



DBFacade::~DBFacade() {
    database.close();
    delete query;
}
