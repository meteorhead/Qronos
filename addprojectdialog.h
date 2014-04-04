#ifndef ADDPROJECTDIALOG_H
#define ADDPROJECTDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

namespace Ui {
class addProjectDialog;
}

class addProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addProjectDialog(QWidget *parent = 0);
    QString queryStatement();
    ~addProjectDialog();
signals:
   void signalToProcessQuery();
public slots:
    void showExistingProjectsCombo(bool);
    void forgeDBQuery();

private:
    Ui::addProjectDialog *ui;
    QString projectName, projectSync, projectDescription;
    qint16 selectedComboIndex;
    QString dbQuery;
};

#endif // ADDPROJECTDIALOG_H
