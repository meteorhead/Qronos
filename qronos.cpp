#include "qronos.h"
#include "ui_qronos.h"

Qronos::Qronos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Qronos)
{
    ui->setupUi(this);
    if(db.openDB())
    {

        QSqlQuery query;
    query.exec("create table if not exists Projects  "
               "(id integer primary key AUTOINCREMENT, "
               "Project_Name varchar(30), "
               "Project_Desc varchar(200), "
               "Time_Elapsed TEXT, "
               "Date TEXT)");

    }

    layoutInit();

    //            Signals & SLots
    connect(ui->add,SIGNAL(clicked()),this,SLOT(addProject()));
    connect(ui->remove,SIGNAL(clicked()),SLOT(removeSelectedEntry()));
    connect(ui->boutonExport,SIGNAL(clicked()),this,SLOT(exportDataToCSV()));
    connect(ui->boutonStart,SIGNAL(clicked()),this,SLOT(beginTimeTracking()));
    connect(ui->boutonStop,SIGNAL(clicked()),this,SLOT(EndTimeTracking()));
}

void Qronos::addProject()
{
addDialog = new addProjectDialog(this);
addDialog->exec();
 connect(addDialog,SIGNAL(signalToProcessQuery()),this, SLOT(execProjectAddQuery()));
 QSqlQuery queryAddProject;

  queryAddProject.exec(addDialog->queryStatement());
  mProjects->select();

}

void Qronos::exportDataToCSV()
{

#ifdef Q_OS_LINUX
   // NOTE: We have to store database file into user home folder in Linux
   QString path(QDir::home().path());
   path.append(QDir::separator()).append("qronos.csv");
   path = QDir::toNativeSeparators(path);

   #else
   // NOTE: File exists in the application private folder, in Symbian Qt implementation
   path = "qronos.csv";
   #endif
        QFile file(path);
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            int rowCount = mProjects->rowCount();
            int columnCount = mProjects->columnCount();
            QTextStream out(&file);


            for (int j=0;j<columnCount;j++)
            {
            QString tr = mProjects->headerData(j,Qt::Horizontal).toString();
            out << tr;
            out << ";";
            }
            out << "\n";

            for(int row =0; row < rowCount; row++)
            {
            for(int column = 0; column < columnCount; column++) {
                QModelIndex index = mProjects->index(row,column);

                if(column == columnCount)
                out << "\n";

                out << mProjects->data(index).toString();
                out << ";";


            }
           out << ";";
           out << "\n";

    }


            file.close();

}

void Qronos::execProjectAddQuery()
{
    QSqlQuery queryAddProject;
    queryAddProject.exec(addDialog->queryStatement());
   mProjects->select();
}

void Qronos::removeSelectedEntry()
{

if (ui->treeView->selectionModel()->currentIndex().isValid())
{
    int currentIndex = ui->treeView->selectionModel()->currentIndex().row();
    QString selectedName = mProjects->index(currentIndex,1).data().toString();
    int idProject = identifySelectedItem();
    int choice = QMessageBox::question(this,"Notice",QString(
                                    " Do You really want to delete").append("\""+selectedName+"\" ? "),QMessageBox::Yes,QMessageBox::No);
                        if(choice == QMessageBox::Yes)
                        {
                            QString req = "DELETE FROM Projects WHERE id = ";
                            req.append(QString::number(idProject));
                            QSqlQuery deleteQuery(req);
                            deleteQuery.exec();
                            mProjects->select();
                        }
                        if(choice == QMessageBox::No)
                            return;
                   }
       else
           QMessageBox::information(this,"Notice","Please choose a project to delete",QMessageBox::Ok);

}

void Qronos::beginTimeTracking()
{
    int idProject = identifySelectedItem();
    if (ui->treeView->selectionModel()->currentIndex().isValid())
    {
      QSqlQuery fetchActiveP(QString("SELECT Project_Name from Projects WHERE id = ").append(("\""+QString::number(idProject)+"\" ")));
      fetchActiveP.exec();
      QString project ;
        while (fetchActiveP.next())
                    project= fetchActiveP.value(0).toString();
       int row = ui->treeView->currentIndex().row();
       projectList << project;
      activeProjects->setStringList(projectList);
       ui->listView->setModel(activeProjects);
       ui->listView->show();
        timerMap->insert(idProject,new QTime);
        t = timerMap->value(idProject);
        t->restart();

 }

}


void Qronos::EndTimeTracking()
{

   int idProject = identifySelectedItem();
    if (ui->treeView->selectionModel()->currentIndex().isValid())
    {
        QSqlQuery fetchLastestTime(QString("SELECT Time_Elapsed,Project_Name from Projects WHERE id = ").append(("\""+QString::number(idProject)+"\" ")));
        fetchLastestTime.exec();
       QString Time, NameToDelete ;
        while (fetchLastestTime.next())
        {
                    Time = fetchLastestTime.value(0).toString();
                    NameToDelete = fetchLastestTime.value(1).toString();
        }
        projectList.removeOne(NameToDelete);
        activeProjects->setStringList(projectList);
         ui->listView->setModel(activeProjects);
         ui->listView->show();
        QTime latestTime;

        latestTime = QTime::fromString(Time,"hh:mm:ss");
        QTime totalTime = latestTime.addMSecs(t->restart());

        QSqlQuery updateGui(QString("UPDATE Projects SET Time_Elapsed = ")
                                       .append("\""+ totalTime.toString("hh:mm:ss") +"\" ").append(
                                   " WHERE id =  ").append(("\""+QString::number(idProject)+"\" ")));
        bool result = updateGui.exec();
        qDebug() <<result ;
        mProjects->select();

    }


}

int Qronos::identifySelectedItem()
{
    int currentIndex = ui->treeView->selectionModel()->currentIndex().row();
    QString selectedName = mProjects->index(currentIndex,1).data().toString();
    QSqlQuery getProjectId(QString("SELECT id from Projects WHERE Project_Name = ").append(("\""+selectedName+"\" ")));
    getProjectId.exec();
    int idProject ;
    while (getProjectId.next())
                idProject = getProjectId.value(0).toInt();
    return idProject;
}

void Qronos::layoutInit()
{
    ui->boutonStart->setText("Start");
    ui->boutonStop->setText("Stop");
    ui->boutonExport->setText("Export");
    ui->add->setIcon(QIcon(":/add-icon.png"));
    ui->remove->setIcon(QIcon(":/remove-icon.png"));

    timerMap = new QMap<int, QTime *>();
    mProjects = new QSqlTableModel(this);
    mProjects->setTable("Projects");
    mProjects->setEditStrategy(QSqlTableModel::OnFieldChange);
    mProjects->select();
    mProjects->setHeaderData(1, Qt::Horizontal, tr("Project Name"));
    mProjects->setHeaderData(2, Qt::Horizontal, tr("Project Desc"));
    mProjects->setHeaderData(3, Qt::Horizontal, tr("Time Elapsed"));
    mProjects->setHeaderData(4, Qt::Horizontal, tr("Date"));
    ui->treeView->setModel(mProjects);
    ui->treeView->hideColumn(0);
   activeProjects = new QStringListModel;




}

QTime Qronos::convertSecondsToTimeString(qint64 mSec)
{
    qint64 sec = mSec / 1000;
    int H =  sec  / 3600;
    int MM = (sec  % 3600) / 60;
    int ss = (sec  % 216000);
    return  QTime(H,MM,ss);
}

Qronos::~Qronos()
{
    delete ui;
    delete timerMap;
    db.deleteDB();
}
