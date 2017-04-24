#ifndef _ITEM_DELEGATES
#define _ITEM_DELEGATES

#include <QPainter>
#include <QAbstractItemDelegate>

class AccountListItemDelegate : public QAbstractItemDelegate
{
    public:
        AccountListItemDelegate(QObject *parent = 0);

        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

        virtual ~AccountListItemDelegate();

};

/////////////////////////////////////////////////////

class DirectoryListItemDelegate : public QAbstractItemDelegate
{
    public:
        DirectoryListItemDelegate(QObject *parent = 0);

        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

        virtual ~DirectoryListItemDelegate();

};

/////////////////////////////////////////////////////

class MailListItemDelegate : public QAbstractItemDelegate
{
    public:
        MailListItemDelegate(QObject *parent = 0);

        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

        virtual ~MailListItemDelegate();

};

#endif
