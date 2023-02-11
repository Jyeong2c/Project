#ifndef PATIENTMODEL_H
#define PATIENTMODEL_H

#include <QObject>
#include <QAbstractTableModel>


class PatientModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PatientModel(QObject *parent = 0);
    void patientData(const QList<QString> &_contactName,
                      const QList<int> &age,
                      const QList<QString> &_doctorID);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

private:
    QList<QString> contactName;
    QList<int> age;
    QList<QString> doctorID;
};

#endif // PATIENTMODEL_H
