#include "MailView.h"
#include "ThemeManager.h"

MailView::MailView(QWidget *parent) : QWidget(parent)
{
    _Layout.addWidget(&_FromLbl, 0, 0, 2, 1);

    _Layout.addWidget(&_FromTxt, 0, 1, 1, 1);

    _Layout.addWidget(&_TimeStampTxt, 1, 1, 1, 1);

    _Layout.addWidget(&_SubjectTxt, 3, 0, 1, 2);

    _Layout.addWidget(&_ToLbl, 4, 0, 1, 1);
    _Layout.addWidget(&_ToTxt, 4, 1, 1, 1);

    _Layout.addWidget(&_CCLbl, 5, 0, 1, 1);
    _Layout.addWidget(&_CCTxt, 5, 1, 1, 1);

    _Layout.addWidget(&_AttachLbl, 6, 0, 1, 1);
    _Layout.addWidget(&_FileList, 6, 1, 1, 1);

    _Layout.addWidget(&_Viewer, 8, 0, 1, 2);

    _ToLbl.setText("To: ");
    _CCLbl.setText("Cc: ");
    _AttachLbl.setText("Attachments: ");

    _FromLbl.setMaximumWidth(75);
    _ToLbl.setMaximumWidth(75);
    _CCLbl.setMaximumWidth(75);
    _AttachLbl.setMaximumWidth(75);

    _FileList.setFlow(QListWidget::LeftToRight);
    _FileList.setViewMode(QListView::ListMode);
    _FileList.setMovement(QListView::Static);
    _FileList.setMaximumHeight(48);
    _FileList.setMinimumHeight(48);
    _FileList.setSpacing(0);
    _Viewer.setFrameStyle(QFrame::NoFrame);

    _FromLbl.setVisible(false);
    _ToLbl.setVisible(false);
    _CCLbl.setVisible(false);
    _AttachLbl.setVisible(false);
    _FileList.setVisible(false);

    _Layout.setMargin(0);
    _Layout.setSpacing(0);
    _WidgetLayout.addLayout(&_Layout, 0, 0);
    setLayout(&_WidgetLayout);

    QFont ftfrom(ApplicationThemeManager.preferredFont(), 11);
    _FromTxt.setFont(ftfrom);

    QFont ftsubj(ApplicationThemeManager.preferredFont(), 9, true);
    ftsubj.setBold(true);
    _SubjectTxt.setFont(ftsubj);

    _FromLbl.setPixmap(QPixmap(ApplicationThemeManager.unknown()).scaled(48, 48));
 }

void MailView::setMail(MailHeader emlhdr, MailBody emlbdy)
{
    _RawBody = emlbdy;

    _FromLbl.setVisible(true);
    _ToLbl.setVisible(true);

    _FromTxt.setText(emlhdr.fromDisplayName());
    _ToTxt.setText(emlhdr.toList().join(","));
    _SubjectTxt.setText(emlhdr.subject());
    _TimeStampTxt.setText(emlhdr.timeStamp());

    if(emlhdr.ccList().count() > 0)
    {
        _CCLbl.setVisible(true);
        _CCTxt.setText(emlhdr.ccList().join(","));
    }

    bool bodyset = false;

    if(emlbdy.hasMimeData())
    {
        emlbdy.deSerialize();

        if(emlbdy.mimeNodes()->count() >= 1)
        {
            for (int ctr = 0; ctr < emlbdy.mimeNodes()->count(); ctr++)
            {
                if(emlbdy.mimeNodes()->at(ctr).NodeType == MimeType::Html)
                {
                    _Viewer.setHtml(emlbdy.mimeNodes()->at(ctr).Data);
                    bodyset = true;
                    break;
                }
            }
        }
    }

    if(!bodyset)
    {
        _Viewer.setText(emlbdy.message());
    }

    if(emlbdy.attachments().count() > 0)
    {
        _AttachLbl.setVisible(false);
        _FileList.setVisible(false);

        for(int idx = 0; idx < emlbdy.attachments().count(); idx++)
        {
            _FileList.addItem(emlbdy.attachments().at(idx));
        }
    }
    else
    {
        _AttachLbl.setVisible(false);
        _FileList.setVisible(false);
    }
}

void MailView::clear()
{
    _FromTxt.setText("");
    _ToTxt.setText("");
    _CCTxt.setText("");
    _SubjectTxt.setText("");
    _TimeStampTxt.setText("");
    _Viewer.setText("");
    _FileList.clear();

    _FromLbl.setVisible(false);
    _ToLbl.setVisible(false);
    _CCLbl.setVisible(false);
    _AttachLbl.setVisible(false);
}
