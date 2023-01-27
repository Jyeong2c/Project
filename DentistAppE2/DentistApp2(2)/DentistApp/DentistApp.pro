QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fmx.cpp \
    layout.cpp \
    main.cpp \
    mainwindow.cpp \
#    panning.cpp \
 \#    scene.cpp
    scene.cpp

HEADERS += \
    fmx.h \
    layout.h \
    mainwindow.h \
#    panning.h \
 \#    scene.h
    scene.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Image.qrc \
    Image.qrc




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Desktop/opencv/build/x64/vc15/lib/ -lopencv_world454
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Desktop/opencv/build/x64/vc15/lib/ -lopencv_world454d

INCLUDEPATH += $$PWD/../../../../../../Desktop/opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../../../../Desktop/opencv/build/x64/vc15
INCLUDEPATH += $$PWD/../../../../../../Desktop/opencv/build/include
