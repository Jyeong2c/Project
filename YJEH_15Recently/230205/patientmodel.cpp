#include "patientmodel.h"

PatientModel::PatientModel(QObject *parent) : QAbstractTableModel(parent)
{

}

/*환자 데이터를 받는 함수*/
void PatientModel::patientData(const QList<QString> &_contactName,
                                const QList<int> &_age,
                                const QList<QString> &_doctorID)
{
    /*각 환자 데이터 초기화 후 입력*/
    contactName.clear();
    contactName = _contactName;
    age.clear();
    age = _age;
    doctorID.clear();
    doctorID = _doctorID;
    return;
}

/*환자 모델 테이블 rowCount*/
int PatientModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return contactName.length();            // 1행
}

/*환자 모델 테이블 columnCount*/
int PatientModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;                               // 3열(Name, age, DoctorID)
}

/*환자 테이블 내의 데이터*/
QVariant PatientModel::data(const QModelIndex &index, int role) const
{
    /*각 열에 입력되는 데이터를 반환 없으면 예외처리*/
    if(!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if(index.column() == 0) {
        return contactName[index.row()];        // 0번째 열에는 환자의 이름 반환
    }
    else if (index.column() == 1) {
        return age[index.row()];                // 1번째 열에는 환자의 나이 반환
    }
    else if (index.column() == 2) {
        return doctorID[index.row()];           // 2번째 열에는 환자의 담당의사 로그인 아이디 반환
    }
    return QVariant();
}

/*테이블 헤더 데이터*/
QVariant PatientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    /*테이블 헤더 데이터 표시*/
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        if(section == 0) {
            return QString("Name");             // 0번째 헤더 데이터 "Name"
        }
        else if(section == 1) {
            return QString("age");              // 1번째 헤더 데이터 "age"
        }
        else if(section == 2) {
            return QString("DoctorID");         // 2번째 헤더 데이터 "DoctorID"
        }
    }
    return QVariant();
}


