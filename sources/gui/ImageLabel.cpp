#include "ImageLabel.h"
#include "ThemeManager.h"

ImageLabel::ImageLabel(QWidget* ptr) : QWidget(ptr)
{
    setMaximumHeight(60);
    setMinimumHeight(60);
    setMinimumWidth(250);
    setMaximumWidth(250);

    _UnderLine = true;
}

ImageLabel::ImageLabel(QString txt, QString fname, QWidget* ptr) : QWidget(ptr)
{
    setMaximumHeight(60);
    setMinimumHeight(60);
    setMinimumWidth(250);
    setMaximumWidth(250);

    setText(txt);
    setImageFile(fname);
    _UnderLine = true;
}

ImageLabel::~ImageLabel()
{
}

void ImageLabel::setText(QString txt)
{
    _Text = txt;
}

void ImageLabel::setImageFile(QString fname)
{
    _Image = QPixmap(fname).scaled(32, 32);
}

void ImageLabel::setUnderline(bool fl)
{
    _UnderLine = fl;
}

QSize ImageLabel::sizeHint() const
{
    return QSize(250, 60);
}

void ImageLabel::paintEvent(QPaintEvent *event)
{
    QRect rcb(60, 0, 190, 60);
    QRect rc(0, 0, 250, 60);

    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 12, QFont::Normal);
    QPen penText(QBrush(ApplicationThemeManager.palette().text()), 1);
    QPen penBorder(QBrush(ApplicationThemeManager.palette().base()), 1);
    QPen penLine(QBrush(ApplicationThemeManager.palette().alternateBase()), 1);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(penBorder);
    painter.setBrush(ApplicationThemeManager.palette().base());
    painter.drawRect(rcb);

    painter.setPen(penText);
    painter.setFont(fontHeadingHighlighted);

    if(_Text.length()> 0)
    {
        painter.drawText(rc.left() + 65 , rc.top(), rc.width()-65, rc.height(), Qt::AlignVCenter|Qt::AlignLeft, _Text, &rc);
    }

    if(!_Image.isNull())
    {
        painter.drawPixmap(14, 14, 32, 32, _Image);
    }

    if(_UnderLine)
    {
        painter.setPen(penLine);
        painter.drawLine(0, 60, 250, 60);
    }
}
