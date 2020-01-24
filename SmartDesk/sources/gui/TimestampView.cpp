#include "TimestampView.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"

TimestampView::TimestampView(QWidget *parent) : QDialog(parent)
{
    _Calender.setGridVisible(true);
    connect(&_CmdOk, &QPushButton::clicked, this, &TimestampView::eventOk);
    connect(&_CmdCancel, &QPushButton::clicked, this, &TimestampView::eventCancel);

    _CmdOk.setText("OK");
    _CmdCancel.setText("Cancel");
    _CmdLayout.addWidget(&_CmdCancel);
    _CmdLayout.addWidget(&_CmdOk);

    _CmdLayout.setMargin(5);
    _CmdLayout.setSpacing(5);

    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_Calender);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_TimeEd);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_CmdLayout);
    _Layout.addStretch();

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);

    setModal(true);
}

bool TimestampView::timestampSelected()
{
    return _IsTimestampSelected;
}

QDateTime TimestampView::timestamp()
{
    QDateTime dt;
    dt.setDate(_Calender.selectedDate());
    dt.setTime(_TimeEd.time());

    return dt;
}

void TimestampView::eventOk()
{
    _IsTimestampSelected = true;
    close();
}

void TimestampView::eventCancel()
{
    _IsTimestampSelected = false;
    close();
}
