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
                      const QList<QString> &_doctorID);     // 환자 데이터를 받는 함수
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;      // 환자 모델 테이블 rowCount
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;   // 환자 모델 테이블 columnCount

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override; // 환자 테이블 내의 데이터
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;          // 테이블 헤더 데이터

private:
    QList<QString> contactName;    // 환자 이름
    QList<int> age;                // 환자 나이
    QList<QString> doctorID;       // 의사 로그인 아이디
};

#endif // PATIENTMODEL_H
