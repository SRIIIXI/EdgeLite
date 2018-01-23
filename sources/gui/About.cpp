#include "About.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"

About::About(QWidget *parent) : QWidget(parent)
{
    QPixmap pix1;
    pix1.load(":images/ipbum.png");
    QPixmap scaledpix1 = pix1.scaled(72, 72);

    QPixmap pix2;
    pix2.load(":images/qtlogo.png");
    QPixmap scaledpix2 = pix2.scaled(72, 72);

    _AppImage.setMaximumHeight(72);
    _AppImage.setMaximumWidth(72);
    _AppImage.setPixmap(scaledpix1);
    _AppText.setText("IPBuM Version 2.0.0 \nCopyright 2018 Nauri Tech Private Limited");
    _AppLayout.addWidget(&_AppImage, Qt::AlignLeft | Qt::AlignTop);
    _AppLayout.addWidget(&_AppText,  Qt::AlignLeft | Qt::AlignTop);

    _QtImage.setMaximumHeight(72);
    _QtImage.setMaximumWidth(72);
    _QtImage.setPixmap(scaledpix2);
    _QtText.setText("IPBuM uses Qt 5.10 from The Qt Company \nCopyright 2018 The Qt Company. All rights reserved.");
    _QtLayout.addWidget(&_QtImage,  Qt::AlignLeft | Qt::AlignTop);
    _QtLayout.addWidget(&_QtText, Qt::AlignLeft | Qt::AlignTop);

    _Layout.addWidget(new HorizontalLine());

    _AppLayout.setMargin(5);
    _AppLayout.setSpacing(5);
    _QtLayout.setMargin(5);
    _QtLayout.setSpacing(5);

    _Layout.addLayout(&_AppLayout);
    _Layout.addLayout(&_QtLayout);
    _Layout.addStretch();

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);
}
