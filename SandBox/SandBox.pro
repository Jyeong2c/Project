QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addimageserver.cpp \
    blending.cpp \
    deleteimage.cpp \
    downloader.cpp \
    logindlg.cpp \
    main.cpp \
    mainwidget.cpp \
    mainwindow.cpp \
    patientmodel.cpp \
    patitentview.cpp \
    uploader.cpp

HEADERS += \
    addimageserver.h \
    blending.h \
    deleteimage.h \
    downloader.h \
    logindlg.h \
    mainwidget.h \
    mainwindow.h \
    patientmodel.h \
    patitentview.h \
    uploader.h

FORMS += \
    addimageserver.ui \
    blending.ui \
    logindlg.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/build/x64/vc15/lib/ -lopencv_world454
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/build/x64/vc15/lib/ -lopencv_world454d

INCLUDEPATH += C:/opencv/build/x64/vc15
DEPENDPATH += C:/opencv/build/x64/vc15

INCLUDEPATH += C:/opencv/build/include
