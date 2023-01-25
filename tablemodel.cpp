#include "tablemodel.h"
#include <QColor>



TableModel::TableModel(QObject *parent) : QSqlTableModel(parent)
{
}

TableModel::TableModel(QSqlDatabase *database, QObject *parent) : QSqlTableModel(parent, database)
{
}

QVariant TableModel::data(const QModelIndex &idx, int role) const
{
    if(role == Qt::BackgroundRole){
        if(QSqlTableModel::data(this->index(idx.row(), 3)).toInt() == 41){
            return QColor(Qt::red);
        }
    } else if(role == Qt::DisplayRole){
        return QSqlTableModel::data(idx);
    }
    return QVariant();
}
