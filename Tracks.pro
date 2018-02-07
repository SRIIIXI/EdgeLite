#-------------------------------------------------
#
# Project created by QtCreator 2018-01-08T19:06:10
#
#-------------------------------------------------

QT       += core network sql widgets xml

TARGET = Tracks
TEMPLATE = app

win32
{
    LIBS += -lws2_32
    DEFINES += NOMINMAX
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += sources sources/app sources/gui sources/network

SOURCES += \
    sources/app/main.cpp \
    sources/gui/MainWindow.cpp \
    sources/gui/CustomWidgets.cpp \
    sources/gui/SideBar.cpp \
    sources/gui/ThemeManager.cpp \
    sources/gui/NetworkView.cpp \
    sources/gui/ConfigurationView.cpp \
    sources/gui/StatisticsView.cpp \
    sources/gui/About.cpp \
    sources/network/ServiceInterface.cpp \
    sources/network/DownloadManager.cpp \
    sources/app/Tracks.cpp \
    sources/gui/LiveStreamView.cpp

HEADERS += \
    sources/app/AppSettings.h \
    sources/gui/MainWindow.h \
    sources/gui/CustomWidgets.h \
    sources/gui/SideBar.h \
    sources/gui/ThemeManager.h \
    sources/gui/NetworkView.h \
    sources/gui/ConfigurationView.h \
    sources/gui/StatisticsView.h \
    sources/gui/About.h \
    sources/network/ONVIFCamera.h \
    sources/network/ServiceInterface.h \
    sources/network/RecordingStructures.h \
    sources/network/DownloadManager.h \
    sources/app/Tracks.h \
    sources/gui/LiveStreamView.h

RESOURCES += resources/Resources.qrc
