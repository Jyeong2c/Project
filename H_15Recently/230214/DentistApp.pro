QT       += core gui sql network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 qtestlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    #fmx.cpp \
    #item.cpp \
    blendingdlg.cpp \
    deleteimage.cpp \
    downloader.cpp \
    keyfeaturesform.cpp \
    layout.cpp \
    #listalbum.cpp \
    loginform.cpp \
    main.cpp \
    mainwidget.cpp \
    mainwindow.cpp \
 \    #newwindow.cpp
    maxlayout.cpp \
    patientmodel.cpp \
    patientview.cpp \
    scene.cpp \
    #menuitem.cpp \
    #scene.cpp \
    teethform.cpp \
    uploaddlg.cpp \
    uploader.cpp \
    view.cpp

HEADERS += \
    #fmx.h \
    #item.h \
    blendingdlg.h \
    deleteimage.h \
    downloader.h \
    keyfeaturesform.h \
    layout.h \
    #listalbum.h \
    loginform.h \
    mainwidget.h \
    mainwindow.h \
 \    #newwindow.h
    maxlayout.h \
    patientmodel.h \
    patientview.h \
    scene.h \
    #menuitem.h \
    #scene.h \
    teethform.h \
    uploaddlg.h \
    uploader.h \
    view.h

FORMS += \
    blendingdlg.ui \
    keyfeaturesform.ui \
    loginform.ui \
    mainwindow.ui \
    teethform.ui \
    uploaddlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Image.qrc
    win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/build/x64/vc15/lib/ -lopencv_world454
    else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/build/x64/vc15/lib/ -lopencv_world454d

    INCLUDEPATH += C:/opencv/build/x64/vc15
    DEPENDPATH += C:/opencv/build/x64/vc15

    INCLUDEPATH += C:/opencv/build/include
