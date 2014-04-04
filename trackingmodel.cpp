#include "trackingmodel.h"

TrackingModel::TrackingModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QVariant TrackingModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole)
    {
        if (row == 0 && col == 0)
        {
            return QTime::currentTime().toString();
        }
    }
    return QVariant();
}
