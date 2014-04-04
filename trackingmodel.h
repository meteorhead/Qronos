#ifndef TRACKINGMODEL_H
#define TRACKINGMODEL_H

#include <QSqlQueryModel>
#include <QTime>

class TrackingModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit TrackingModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

#endif // TRACKINGMODEL_H
