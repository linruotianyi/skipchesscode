QT       += core gui
QT       +=network   #添加network库
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chess.cpp \
    choose.cpp \
    choose1.cpp \
    clientwindow.cpp \
    connectfai.cpp \
    connectsuc.cpp \
    gameover.cpp \
    illegalmove.cpp \
    joinfail.cpp \
    main.cpp \
    mainwindow.cpp \
    netchoose.cpp \
    network.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    newroom.cpp \
    play.cpp \
    room.cpp \
    serverwindow.cpp \
    widget1.cpp \
    win.cpp

HEADERS += \
    chess.h \
    choose.h \
    choose1.h \
    clientwindow.h \
    connectfai.h \
    connectsuc.h \
    gameover.h \
    illegalmove.h \
    joinfail.h \
    mainwindow.h \
    netchoose.h \
    network.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    newroom.h \
    play.h \
    room.h \
    serverwindow.h \
    widget1.h \
    win.h

FORMS += \
    Widget.ui \
    choose.ui \
    choose1.ui \
    clientwindow.ui \
    connectfai.ui \
    connectsuc.ui \
    gameover.ui \
    illegalmove.ui \
    joinfail.ui \
    mainwindow.ui \
    netchoose.ui \
    network.ui \
    newroom.ui \
    room.ui \
    serverwindow.ui \
    win.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
