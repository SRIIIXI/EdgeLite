#include "MailView.h"
#include "ThemeManager.h"

MailView::MailView(QWidget *parent) : QWidget(parent)
{
    _ControlGrid.setMargin(5);

    _ControlGrid.addWidget(&_FromPix, 1, 0, 2, 1);

    _ControlGrid.addWidget(&_FromTxt, 1, 1, 1, 1);

    _ControlGrid.addWidget(&_TimeStampTxt, 2, 1, 1, 1);

    _ControlGrid.addWidget(&_SubjectTxt, 4, 0, 1, 2);

    _ControlGrid.addWidget(&_ToLbl, 5, 0, 1, 1);
    _ControlGrid.addWidget(&_ToTxt, 5, 1, 1, 1);

    _ControlGrid.addWidget(&_CCLbl, 6, 0, 1, 1);
    _ControlGrid.addWidget(&_CCTxt, 6, 1, 1, 1);

    _ControlGrid.addWidget(&_AttachLbl, 7, 0, 1, 1);
    _ControlGrid.addWidget(&_FileList, 7, 1, 1, 1);

    _ControlGrid.addWidget(&_Viewer, 7, 0, 1, 2);

    _ToLbl.setText("To: ");
    _CCLbl.setText("Cc: ");
    _AttachLbl.setText("Attachments: ");

    _FromPix.setMaximumWidth(75);
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

    _FromPix.setVisible(false);
    _ToLbl.setVisible(false);
    _CCLbl.setVisible(false);
    _AttachLbl.setVisible(false);
    _FileList.setVisible(false);

    _WidgetLayout.setMargin(0);
    _WidgetLayout.setSpacing(0);
    _WidgetLayout.addWidget(new HorizontalLine());
    _WidgetLayout.addLayout(&_ControlGrid);
    setLayout(&_WidgetLayout);

    QFont ftfrom(ApplicationThemeManager.preferredFont(), 11);
    _FromTxt.setFont(ftfrom);

    QFont ftsubj(ApplicationThemeManager.preferredFont(), 9, true);
    ftsubj.setBold(true);
    _SubjectTxt.setFont(ftsubj);

    _FromPix.setPixmap(QPixmap(ApplicationThemeManager.unknown()).scaled(48, 48));
 }

void MailView::setMail(MailHeader emlhdr, MailBody emlbdy)
{
    _RawBody = emlbdy;

    _FromPix.setVisible(true);
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
                    QString str = emlbdy.mimeNodes()->at(ctr).Data;
                    str = str.replace("3D\"#", "\"#");
                    _Viewer.setHtml(str);
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

    _FromPix.setVisible(false);
    _ToLbl.setVisible(false);
    _CCLbl.setVisible(false);
    _AttachLbl.setVisible(false);
}
