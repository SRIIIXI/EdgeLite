#include "ItemDelegates.h"
#include "ThemeManager.h"

AccountListItemDelegate::AccountListItemDelegate(QObject *parent)
{

}

void AccountListItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole).toString();
    int imageSpace = 10;    QRect r = option.rect;

    QPen penBlack(Qt::black, 1, Qt::SolidLine);
    QPen penWhite(Qt::white, 1, Qt::SolidLine);

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
        painter->setBrush(Qt::white);
    }

    painter->setPen(penWhite);
    painter->drawRect(r);

    if(option.state & QStyle::State_Selected)
    {
        painter->setPen(penWhite);
    }
    else
    {
        painter->setPen(penBlack);
    }

    if (!ic.isNull())
    {
        r = option.rect.adjusted(5, 10, -10, -10);
        //ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
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

QSize AccountListItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize(200, 60); // very dumb value
}

AccountListItemDelegate::~AccountListItemDelegate()
{

}

/////////////////////////////////////////////////////

DirectoryListItemDelegate::DirectoryListItemDelegate(QObject *parent)
{

}

void DirectoryListItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole).toString();
    QRect r = option.rect;

    QPen penBlack(Qt::black, 1, Qt::SolidLine);
    QPen penWhite(Qt::white, 1, Qt::SolidLine);

    QFont fontHeadingNormal(ApplicationThemeManager.preferredFont(), 11, QFont::Normal);
    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 11, QFont::Bold);

    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(ApplicationThemeManager.palette().highlight());
    }
    else
    {
        painter->setBrush(Qt::white);
    }

    painter->setPen(penWhite);
    painter->drawRect(r);

    if(option.state & QStyle::State_Selected)
    {
        painter->setPen(penWhite);
    }
    else
    {
        painter->setPen(penBlack);
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

QSize DirectoryListItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize(200, 40); // very dumb value
}

DirectoryListItemDelegate::~DirectoryListItemDelegate()
{

}

/////////////////////////////////////////////////////

MailListItemDelegate::MailListItemDelegate(QObject *parent)
{

}

void MailListItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));

    //if(ic.isNull())
    //{
    //    ic = QIcon(QPixmap(ApplicationThemeManager.unknown()).scaled(48, 48));
    //}

    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole+1).toString();
    int imageSpace = 10;    QRect r = option.rect;
    bool isnew = index.data(Qt::UserRole+3).toBool();

    QPen penBlack(Qt::black, 1, Qt::SolidLine);
    QPen penWhite(Qt::white, 1, Qt::SolidLine);
    QPen penNewMail(ApplicationThemeManager.palette().highlight(), 1, Qt::SolidLine);

    QFont fontHeadingNormal(ApplicationThemeManager.preferredFont(), 12, QFont::Normal);
    QFont fontDescriptionNormal(ApplicationThemeManager.preferredFont(), 9, QFont::Normal);
    QFont fontHeadingHighlighted(ApplicationThemeManager.preferredFont(), 12, QFont::Normal);
    QFont fontDescriptionHighlighted(ApplicationThemeManager.preferredFont(), 9, QFont::Normal);

    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(ApplicationThemeManager.palette().highlight());
    }
    else
    {
        painter->setBrush(ApplicationThemeManager.palette().base());
    }

    painter->setPen(penWhite);
    painter->drawRect(r);

    if(option.state & QStyle::State_Selected)
    {
        painter->setPen(penWhite);
    }
    else
    {
        if(isnew)
        {
            painter->setPen(penNewMail);
        }
        else
        {
            painter->setPen(penBlack);
        }
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

QSize MailListItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize(200, 60); // very dumb value
}

MailListItemDelegate::~MailListItemDelegate()
{

}
