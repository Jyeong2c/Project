//#include "mainwindow.h"
//#include "layout.h"
//#include "fmx.h"
//#include "loginform.h"
//#include "keyfeaturesform.h"
//#include "teethform.h"
#include "mainwidget.h"
#include "patientmodel.h"
#include "patientview.h"
#include "downloader.h"
#include "blendingdlg.h"
#include "uploaddlg.h"
#include "deleteimage.h"
#include "uploader.h"

#include <QStackedWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget main;
    main.show();

    /*환자 모델 테스트*/
    PatientModel patientModel;
    //QTest::qExec(&patientModel);    // 테스트 통과

    /*환자 뷰어 테스트*/
    PatitentView patientView;
    //QTest::qExec(&patientView);     // 테스트 통과

    /*다운로더 테스트*/
    Downloader downloader;
    //QTest::qExec(&downloader);     // reply를 할당 할 수 있는 class가 존재하지 않음

    /*블랜드 다이얼로그 테스트*/
    BlendingDlg blending;
    //QTest::qExec(&blending);        // 테스트 통과 후 프로그램 종료 시 실패

    /*이미지 삭제 단위 테스트*/
    DeleteImage deleteImage;
    //QTest::qExec(&deleteImage);     // 테스트 통과

    /*업로드 단위 테스트*/
    Uploader uploader;
    //QTest::qExec(&uploader);        // 테스트 통과

    UploadDlg uploaderDlg;
    //QTest::qExec(&uploaderDlg);     // 테스트 통과 후 프로그램 종료 시 실패

    return a.exec();
}
