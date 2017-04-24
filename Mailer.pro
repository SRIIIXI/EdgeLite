QT       += core gui widgets network sql

TARGET = Mailer
TEMPLATE = app

INCLUDEPATH += sources sources/data sources/gui sources/app sources/helpers sources/model sources/network sources/storage

RESOURCES += \
    resources/Resources.qrc

HEADERS += \
    sources/data/DataEntity.h \
    sources/data/DataInterface.h \
    sources/app/MailClient.h \
    sources/gui/AddressBookWindow.h \
    sources/gui/ContactAddDialog.h \
    sources/gui/MailBoxView.h \
    sources/gui/MailCompositionWindow.h \
    sources/gui/MailView.h \
    sources/gui/MainWindow.h \
    sources/gui/SettingsWindow.h \
    sources/helpers/ActionBuilder.h \
    sources/model/ContactEntity.h \
    sources/model/Mail.h \
    sources/model/MailBoxEntity.h \
    sources/model/SecurityTypes.h \
    sources/network/ImapClient.h \
    sources/network/SmtpClient.h \
    sources/storage/MailStorage.h \
    sources/gui/Calender.h \
    sources/gui/Files.h \
    sources/gui/Tasks.h \
    sources/helpers/ItemDelegates.h \
    sources/gui/AccountView.h \
    sources/model/AccountEntity.h \
    sources/gui/AccountsWindow.h \
    sources/helpers/ThemeManager.h \
    sources/gui/ImageLabel.h

SOURCES += \
    sources/data/DataEntity.cpp \
    sources/data/DataInterface.cpp \
    sources/app/MailClient.cpp \
    sources/app/mailClientMain.cpp \
    sources/gui/AddressBookWindow.cpp \
    sources/gui/ContactAddDialog.cpp \
    sources/gui/MailBoxView.cpp \
    sources/gui/MailCompositionWindow.cpp \
    sources/gui/MailView.cpp \
    sources/gui/MainWindow.cpp \
    sources/gui/SettingsWindow.cpp \
    sources/helpers/ActionBuilder.cpp \
    sources/model/ContactEntity.cpp \
    sources/model/Mail.cpp \
    sources/model/MailBoxEntity.cpp \
    sources/network/ImapClient.cpp \
    sources/network/SmtpClient.cpp \
    sources/storage/MailStorage.cpp \
    sources/gui/Calender.cpp \
    sources/gui/Files.cpp \
    sources/gui/Tasks.cpp \
    sources/helpers/ItemDelegates.cpp \
    sources/gui/AccountView.cpp \
    sources/model/AccountEntity.cpp \
    sources/gui/AccountsWindow.cpp \
    sources/helpers/ThemeManager.cpp \
    sources/gui/ImageLabel.cpp
