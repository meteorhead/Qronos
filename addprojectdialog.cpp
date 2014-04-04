#include "addprojectdialog.h"
#include "ui_addprojectdialog.h"
#include <QMessageBox>

addProjectDialog::addProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addProjectDialog)
{
    ui->setupUi(this);
    ui->projectSync->setVisible(false);
    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(showExistingProjectsCombo(bool)));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(forgeDBQuery()));
}

void addProjectDialog::showExistingProjectsCombo(bool state)
{

    if (state)
    {
        // populate from database
        ui->projectSync->setVisible(true);

        QSqlQuery queryproduct("SELECT DISTINCT Project_Name from Projects where Project_Name != \"\" ORDER BY id DESC");
                    while(queryproduct.next())
                    {
                        ui->projectSync->addItem(queryproduct.value(0).toString());
                     }
    }
    else
        ui->projectSync->setVisible(false);



}

void addProjectDialog::forgeDBQuery()
{

    if (ui->projectName->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please enter a project name");
        dbQuery = "";

    }
    //if we have a valid project name, we can see if we should sync it with another existing project
   // if (!ui->projectSync->currentText().isEmpty())
      //  ;
    QTime tInitial(00,00,00);

    if (ui->spinBox->value() == 0)
    dbQuery = "INSERT INTO Projects(Project_Name, Project_Desc, Time_Elapsed , Date) values ('"+ ui->projectName->text() + "', '"
                                                                                + ui->projectDescription->toPlainText() +  "', '"
                                                                                + tInitial.toString()                                                 +  "', '"
                                                                                + QDate::currentDate().toString()            +  "')";
    else
    { int time = ui->spinBox->value();
      QTime timeElapsed(time/60,time%60,00);
        dbQuery = "INSERT INTO Projects(Project_Name, Project_Desc, Time_Elapsed , Date) values ('"+ ui->projectName->text() + "', '"
                                                                                    + ui->projectDescription->toPlainText() +  "', '"
                                                                                    + timeElapsed.toString()                                                +  "', '"
                                                                                    + QDate::currentDate().toString()            +  "')";
    }
    emit signalToProcessQuery();
}

QString addProjectDialog::queryStatement()
{
    return dbQuery;
}

addProjectDialog::~addProjectDialog()
{
    delete ui;
}
