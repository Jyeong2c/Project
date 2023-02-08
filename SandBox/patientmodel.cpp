#include "patientmodel.h"

PatientModel::PatientModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void PatientModel::patientData(const QList<QString> &_contactName,
                                const QList<int> &_age,
                                const QList<QString> &_doctorID)
{
    contactName.clear();
    contactName = _contactName;
    age.clear();
    age = _age;
    doctorID.clear();
    doctorID = _doctorID;
    return;
}

int PatientModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return contactName.length();
}
int PatientModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}
QVariant PatientModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if(index.column() == 0) {
        return contactName[index.row()];
    }
    else if (index.column() == 1) {
        return age[index.row()];
    }
    else if (index.column() == 2) {
        return doctorID[index.row()];
    }
    return QVariant();
}

QVariant PatientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        if(section == 0) {
            return QString("Name");
        }
        else if(section == 1) {
            return QString("age");
        }
        else if(section == 2) {
            return QString("DoctorID");
        }
    }
    return QVariant();
}


