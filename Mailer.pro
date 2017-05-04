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
    sources/gui/ContactAddDialog.h \
    sources/gui/MailBoxView.h \
    sources/gui/MailView.h \
    sources/gui/MainWindow.h \
    sources/helpers/ActionBuilder.h \
    sources/model/ContactEntity.h \
    sources/model/Mail.h \
    sources/model/MailBoxEntity.h \
    sources/model/SecurityTypes.h \
    sources/network/ImapClient.h \
    sources/network/SmtpClient.h \
    sources/storage/MailStorage.h \
    sources/helpers/ItemDelegates.h \
    sources/gui/AccountView.h \
    sources/model/AccountEntity.h \
    sources/helpers/ThemeManager.h \
    sources/gui/ImageLabel.h \
    sources/gui/SettingsView.h \
    sources/gui/ContactsView.h \
    sources/gui/AccountsConfigView.h \
    sources/gui/MailNewView.h

SOURCES += \
    sources/data/DataEntity.cpp \
    sources/data/DataInterface.cpp \
    sources/app/MailClient.cpp \
    sources/app/mailClientMain.cpp \
    sources/gui/ContactAddDialog.cpp \
    sources/gui/MailBoxView.cpp \
    sources/gui/MailView.cpp \
    sources/gui/MainWindow.cpp \
    sources/helpers/ActionBuilder.cpp \
    sources/model/ContactEntity.cpp \
    sources/model/Mail.cpp \
    sources/model/MailBoxEntity.cpp \
    sources/network/ImapClient.cpp \
    sources/network/SmtpClient.cpp \
    sources/storage/MailStorage.cpp \
    sources/helpers/ItemDelegates.cpp \
    sources/gui/AccountView.cpp \
    sources/model/AccountEntity.cpp \
    sources/helpers/ThemeManager.cpp \
    sources/gui/ImageLabel.cpp \
    sources/gui/SettingsView.cpp \
    sources/gui/ContactsView.cpp \
    sources/gui/AccountsConfigView.cpp \
    sources/gui/MailNewView.cpp
