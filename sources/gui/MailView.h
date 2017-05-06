#ifndef _MAIL_VIEW
#define _MAIL_VIEW

#include <QtCore>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QListWidget>
#include <QPushButton>

#include "Mail.h"
#include "ThemeManager.h"

class MailView : public QWidget
{
    Q_OBJECT
public:
    MailView(QWidget *parent = 0);
    void setMail(MailHeader emlhdr, MailBody emlbdy);
    void clear();

private:
    QGridLayout _Layout;
    QGridLayout _WidgetLayout;

    QLabel _FromLbl;
    QLabel _ToLbl;
    QLabel _CCLbl;
    QLabel _AttachLbl;

    QLabel _FromTxt;
    QLabel _ToTxt;
    QLabel _CCTxt;
    QLabel _SubjectTxt;
    QLabel _TimeStampTxt;
    QListWidget _FileList;

    QTextBrowser _Viewer;

    MailBody _RawBody;
};

#endif
