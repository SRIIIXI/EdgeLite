#include "About.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"

About::About(QWidget *parent) : QWidget(parent)
{
    QPixmap pix1;
    pix1.load(":images/ipbum_small.png");
    QPixmap scaledpix1 = pix1.scaled(256, 256);

    _AppImage.setPixmap(scaledpix1);
    _AppText.setText("IPBuM Version 3.0.0 \nCopyright 2018 Nauri Tech Private Limited\nhttp://www.remotedrishtee.com/");
    _AppLayout.addWidget(&_AppImage, Qt::AlignLeft | Qt::AlignTop);
    _AppLayout.addWidget(&_AppText,  Qt::AlignLeft | Qt::AlignTop);

    _Layout.addWidget(new HorizontalLine());

    _AppLayout.setMargin(5);
    _AppLayout.setSpacing(5);

    _Layout.addLayout(&_AppLayout);
    _Layout.addStretch();
    _Layout.addWidget(new HorizontalLine());

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);
}
