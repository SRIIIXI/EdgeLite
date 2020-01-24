QT += core network widgets

TARGET = IPCamDesktop
TEMPLATE = app

win32{
DEFINES += NOMINMAX
}

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += sources sources/app sources/gui sources/entity

SOURCES += \
    sources/app/IPCamDesktop.cpp \
    sources/app/main.cpp \
    sources/gui/CameraView.cpp \
    sources/gui/LicenseView.cpp \
    sources/gui/MainWindow.cpp \
    sources/gui/CustomWidgets.cpp \
    sources/gui/RecordingView.cpp \
    sources/gui/SideBar.cpp \
    sources/gui/ThemeManager.cpp \
    sources/gui/ConfigurationView.cpp \
    sources/gui/About.cpp \
    sources/gui/TimestampView.cpp \
    sources/gui/CamNameDialog.cpp

HEADERS += \
    sources/app/AppSettings.h \
    sources/app/IPCamDesktop.h \
    sources/entity/Recording.h \
    sources/gui/CameraView.h \
    sources/gui/LicenseView.h \
    sources/gui/MainWindow.h \
    sources/gui/CustomWidgets.h \
    sources/gui/RecordingView.h \
    sources/gui/SideBar.h \
    sources/gui/ThemeManager.h \
    sources/gui/ConfigurationView.h \
    sources/gui/About.h \
    sources/gui/TimestampView.h \
    sources/gui/CamNameDialog.h \
    sources/entity/Camera.h \
    sources/entity/Site.h

RESOURCES += resources/Resources.qrc
