#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>


class TableModel : public QSqlTableModel
{
    Q_OBJECT
public:

    explicit TableModel(QObject *parent = 0);

    explicit TableModel(QSqlDatabase *database, QObject *parent = 0);

    QVariant data(const QModelIndex &idx, int role) const;

signals:

public slots:

};



#endif // TABLEMODEL_H
