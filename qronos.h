#ifndef QRONOS_H
#define QRONOS_H

#include <QWidget>
#include "database.h"
#include "addprojectdialog.h"
#include <QSqlTableModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QMap>
#include <QElapsedTimer>
#include <QStringListModel>

namespace Ui {
class Qronos;
}

class Qronos : public QWidget
{
    Q_OBJECT

public:
    explicit Qronos(QWidget *parent = 0);
    int identifySelectedItem();
    void layoutInit();
    QTime convertSecondsToTimeString(qint64 mSec);
    ~Qronos();
public slots:
    void addProject();
    void execProjectAddQuery();
    void exportDataToCSV();
    void removeSelectedEntry();
    void beginTimeTracking();
    void EndTimeTracking();
    void updateGuiActiveProjects();
private:
    Ui::Qronos *ui;
    Database db;
    addProjectDialog *addDialog;
    QStringListModel *activeProjects;
    QStringList projectList;
    QSqlTableModel *mProjects;
    QMap<int, QTime *> *timerMap;
     QTime *t;

};

#endif // QRONOS_H
