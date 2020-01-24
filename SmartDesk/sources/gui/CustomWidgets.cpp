#include "CustomWidgets.h"
#include "ThemeManager.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    setLayout(&_Layout);
    _Layout.setMargin(0);
    _Layout.setSpacing(0);
}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::addWidget(QWidget *widget)
{
    _Layout.addWidget(widget);
}

////////////////////////////////////////

OptionListItemDelegate::OptionListItemDelegate(bool large, QObject *parent)
{
    _Large = large;

    if(_Large)
    {
        _Height = 60;
    }
    else
    {
        _Height = 40;
    }
}

void OptionListItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(_Large)
    {
        paintLarge(painter, option, index);
    }
    else
    {
        paintSmall(painter, option, index);
    }
}

void OptionListItemDelegate::paintLarge(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole).toString();
    int imageSpace = 10;    QRect r = option.rect;

    QPen penTextNormal(ApplicationThemeManager.palette().text(), 1, Qt::SolidLine);
    QPen penTextSelected(ApplicationThemeManager.palette().highlight(), 1, Qt::SolidLine);

    QFont fontHeadingNormal(ApplicationThemeManager.preferredFont(), 11, QFont::Normal);
    QFont fontDescriptionNormal(ApplicationThemeManager.preferredFont(), 10, QFont::Normal);
    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 11, QFont::Bold);
    QFont fontDescriptionHighlighted(ApplicationThemeManager.preferredFont(), 10, QFont::Bold);

    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(ApplicationThemeManager.palette().highlight());
    }
    else
    {
        painter->setBrush(ApplicationThemeManager.palette().window());
    }

    painter->setPen(penTextSelected);

    if(option.state & QStyle::State_Selected)
    {
        painter->setPen(penTextSelected);
    }
    else
    {
        painter->setPen(penTextNormal);
    }

    if (!ic.isNull())
    {
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace = 55;
    }

    r = option.rect.adjusted(imageSpace, 0, -10, -30);

    if(option.state & QStyle::State_Selected)
    {
        painter->setFont(fontHeadingHighlighted);
    }
    else
    {
        painter->setFont(fontHeadingNormal);
    }

    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);

    r = option.rect.adjusted(imageSpace, 30, -10, 0);

    if(option.state & QStyle::State_Selected)
    {
        painter->setFont(fontDescriptionHighlighted);
    }
    else
    {
        painter->setFont(fontDescriptionNormal);
    }

    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
}

void OptionListItemDelegate::paintSmall(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole).toString();
    QRect r = option.rect;

    QPen penTextNormal(ApplicationThemeManager.palette().text(), 1, Qt::SolidLine);
    QPen penTextSelected(ApplicationThemeManager.palette().highlight(), 1, Qt::SolidLine);

    QFont fontHeadingNormal(ApplicationThemeManager.preferredFont(), 11, QFont::Normal);
    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 11, QFont::Bold);

    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(ApplicationThemeManager.palette().highlight());
    }
    else
    {
        painter->setBrush(ApplicationThemeManager.palette().window());
    }

    painter->setPen(penTextSelected);

    if(option.state & QStyle::State_Selected)
    {
        painter->setPen(penTextSelected);
    }
    else
    {
        painter->setPen(penTextNormal);
    }

    if(option.state & QStyle::State_Selected)
    {
        painter->setFont(fontHeadingHighlighted);
    }
    else
    {
        painter->setFont(fontHeadingNormal);
    }

    painter->drawText(r.left() + 55 , r.top(), r.width(), r.height(), Qt::AlignVCenter|Qt::AlignLeft, title, &r);
}

QSize OptionListItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize(200, _Height);
}

OptionListItemDelegate::~OptionListItemDelegate()
{

}

OptionList::OptionList(bool large)
{
    setItemDelegate(new OptionListItemDelegate(large, this));
}

OptionList::~OptionList()
{

}

////////////////////////////////////////

DataListItemDelegate::DataListItemDelegate(QObject *parent)
{

}

void DataListItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    bool imagedrawn = false;

    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole).toString();
    int imageSpace = 10;
    QRect r = option.rect;

    QPen penTextNormal(ApplicationThemeManager.palette().text(), 1, Qt::SolidLine);
    QPen penTextSelected(ApplicationThemeManager.palette().highlight(), 1, Qt::SolidLine);

    QFont fontHeadingNormal(ApplicationThemeManager.preferredFont(), 10, QFont::Normal);
    QFont fontDescriptionNormal(ApplicationThemeManager.preferredFont(), 8, QFont::Normal);
    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 10, QFont::Normal);
    QFont fontDescriptionHighlighted(ApplicationThemeManager.preferredFont(), 8, QFont::Normal);

    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(ApplicationThemeManager.palette().highlight());
    }
    else
    {
        painter->setBrush(ApplicationThemeManager.palette().window());
    }

    painter->setPen(penTextSelected);

    if(option.state & QStyle::State_Selected)
    {
        painter->setPen(penTextSelected);
    }
    else
    {
        painter->setPen(penTextNormal);
    }

    QPixmap pix = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));
    if(!pix.isNull())
    {
        QIcon ic = QIcon(pix.scaled(42, 42));
        if (!ic.isNull())
        {
            QRect rpix = r;
            rpix.setX(rpix.x()+9);
            ic.paint(painter, rpix, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace = 55;
            imagedrawn = true;
        }
    }

    if(imagedrawn)
    {
        r = option.rect.adjusted(imageSpace, 0, -10, -30);
    }
    else
    {
        r = option.rect.adjusted(10, 5, 0, 0);
    }

    if(option.state & QStyle::State_Selected)
    {
        painter->setFont(fontHeadingHighlighted);
    }
    else
    {
        painter->setFont(fontHeadingNormal);
    }

    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignTop|Qt::AlignLeft, title, &r);

    if(imagedrawn)
    {
        r = option.rect.adjusted(imageSpace, 30, -10, 0);
    }
    else
    {
        r = option.rect.adjusted(10, 20, 0, 0);
    }

    if(option.state & QStyle::State_Selected)
    {
        painter->setFont(fontDescriptionHighlighted);
    }
    else
    {
        painter->setFont(fontDescriptionNormal);
    }

    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, description, &r);
}

QSize DataListItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize(200, 40);
}

DataListItemDelegate::~DataListItemDelegate()
{

}

DataList::DataList()
{
    setItemDelegate(new DataListItemDelegate(this));
}

DataList::~DataList()
{

}

////////////////////////////////////////

ActiveLabel::ActiveLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ActiveLabel::~ActiveLabel()
{
}

void ActiveLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}

////////////////////////////////////////

RichLabel::RichLabel(QWidget* ptr) : QWidget(ptr)
{
    _UnderLine = true;
    _ClickTrackingOn = false;
    _Large = true;
    _Bold = false;
    _Height = 60;

    setMaximumHeight(_Height);
    setMinimumHeight(_Height);
}

RichLabel::RichLabel(QString txt, QString fname, QWidget* ptr) : QWidget(ptr)
{
    _UnderLine = true;
    _ClickTrackingOn = false;
    _Large = true;
    _Bold = false;
    _Height = 60;

    setMaximumHeight(_Height);
    setMinimumHeight(_Height);

    setText(txt);
    setImageFile(fname);
}

RichLabel::~RichLabel()
{
}

void RichLabel::setText(QString txt)
{
    _Text = txt;
}

void RichLabel::setImageFile(QString fname)
{
    _Image = QPixmap(fname).scaled(32, 32);
}

void RichLabel::setUnderline(bool fl)
{
    _UnderLine = fl;
}

void RichLabel::setLarge(bool largef)
{
    _Large = largef;

    if(_Large)
    {
        _Height = 60;
    }
    else
    {
        _Height = 40;
    }
}

void RichLabel::setBold(bool boldf)
{
    _Bold = boldf;
}

QSize RichLabel::sizeHint() const
{
    int wd = parentWidget()->width();

    return QSize(wd, _Height);
}

void RichLabel::paintEvent(QPaintEvent *event)
{
    int wd = parentWidget()->width();
    QRect rcb(_Height, 0, 190, _Height);
    QRect rc(0, 0, wd, _Height);

    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 12, QFont::Normal);
    QPen penText(QBrush(ApplicationThemeManager.palette().text()), 1);
    QPen penBorder(QBrush(ApplicationThemeManager.palette().base()), 1);
    QPen penLine(QBrush(QColor(127,127,127)), 1);

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
        if(_Large)
        {
            painter.drawPixmap(14, 14, 32, 32, _Image);
        }
    }

    if(_UnderLine)
    {
        painter.setPen(penLine);
        painter.drawLine(0, _Height, wd, _Height);
    }
}

void RichLabel::mousePressEvent(QMouseEvent *event)
{
    _ClickTrackingOn = true;
}

void RichLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(_ClickTrackingOn)
    {
        emit clicked();
        _ClickTrackingOn = false;
    }
}

////////////////////////////////////////////////////////

Button::Button(QWidget* ptr) : QWidget(ptr)
{
    setMaximumHeight(60);
    setMinimumHeight(60);
    setMinimumWidth(250);
    setMaximumWidth(250);
    _ClickTrackingOn = false;
}

Button::Button(QString txt, QString fname, QWidget* ptr) : QWidget(ptr)
{
    setMaximumHeight(60);
    setMinimumHeight(60);
    setMinimumWidth(250);
    setMaximumWidth(250);

    setText(txt);
    setImageFile(fname);
    _ClickTrackingOn = false;
}

Button::~Button()
{
}

void Button::setText(QString txt)
{
    _Text = txt;
}

void Button::setImageFile(QString fname)
{
    _Image = QPixmap(fname).scaled(32, 32);
}

QSize Button::sizeHint() const
{
    return QSize(250, 60);
}

void Button::paintEvent(QPaintEvent *event)
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
}

void Button::mousePressEvent(QMouseEvent *event)
{
    _ClickTrackingOn = true;
}

void Button::mouseReleaseEvent(QMouseEvent *event)
{
    if(_ClickTrackingOn)
    {
        emit clicked();
        _ClickTrackingOn = false;
    }
}

////////////////////////////////////////

VerticalLine::VerticalLine(int sz, QWidget* ptr) : QWidget(ptr)
{
    setMinimumWidth(1);
    setMaximumWidth(1);
    _Height = sz;
}

VerticalLine::~VerticalLine()
{
}

void VerticalLine::setHeight(int sz)
{
    _Height = sz;
}

QSize VerticalLine::sizeHint() const
{
    int ht = 0;

    if(_Height == -1)
    {
        ht = parentWidget()->height();
    }
    else
    {
        ht = _Height;
    }

    return QSize(1, ht);
}

void VerticalLine::paintEvent(QPaintEvent *event)
{
    int ht = 0;

    if(_Height == -1)
    {
        ht = parentWidget()->height();
    }
    else
    {
        ht = _Height;
    }

    QPen penLine(QBrush(QColor(127,127,127)), 1);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(penLine);
    painter.drawLine(0, 0, 0, ht);
}

////////////////////////////////////////

HorizontalLine::HorizontalLine(int sz, QWidget* ptr) : QWidget(ptr)
{
    setMinimumHeight(1);
    setMaximumHeight(1);
    _Width = sz;
}

HorizontalLine::~HorizontalLine()
{
}

void HorizontalLine::setWidth(int sz)
{
    _Width = sz;
}

QSize HorizontalLine::sizeHint() const
{
    int wd = 0;

    if(_Width == -1)
    {
        wd = parentWidget()->width();
    }
    else
    {
        wd = _Width;
    }

    return QSize(wd, 1);
}

void HorizontalLine::paintEvent(QPaintEvent *event)
{
    int wd = 0;

    if(_Width == -1)
    {
        wd = parentWidget()->width();
    }
    else
    {
        wd = _Width;
    }

    QPen penLine(QBrush(QColor(127,127,127)), 1);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(penLine);
    painter.drawLine(0, 0, wd, 0);
}

