#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QDebug>

class Database : public QObject
{
    Q_OBJECT
public:
    //explicit Database(QObject *parent = 0);
    bool openDB();
    bool checkDB();
    bool deleteDB();
    QSqlError lastError();

signals:

public slots:

private:
    QSqlDatabase db;

};

#endif // DATABASE_H
