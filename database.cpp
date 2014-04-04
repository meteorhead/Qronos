#include "database.h"

bool Database::openDB()
{
     db = QSqlDatabase::addDatabase("QSQLITE");
#ifdef Q_OS_LINUX
   // NOTE: We have to store database file into user home folder in Linux
   QString path(QDir::home().path());
   path.append(QDir::separator()).append("Qronos.db");
   path = QDir::toNativeSeparators(path);
   db.setDatabaseName(path);

   #else
   // NOTE: File exists in the application private folder, in Symbian Qt implementation
   db.setDatabaseName("my.db.sqlite");
   #endif

   // Open databasee
   return db.open();

}
bool Database::checkDB()
{
    return db.isOpen();
}

bool Database::deleteDB()
{

    // Close database
        db.close();

        QSqlDatabase::removeDatabase("QSQLITE");
}

QSqlError Database::lastError()
{
     return db.lastError();

}
