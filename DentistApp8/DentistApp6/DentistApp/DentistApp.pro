QT       += core gui sql network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fmx.cpp \
    layout.cpp \
    #listalbum.cpp \
    main.cpp \
    mainwindow.cpp \
 \    #qdownloader.cpp
    #textitem.cpp

HEADERS += \
    fmx.h \
    layout.h \
    #listalbum.h \
    mainwindow.h \
 \    #qdownloader.h
    #textitem.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Image.qrc \
    Image.qrc
