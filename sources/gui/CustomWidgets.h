#ifndef _CUSTOM_WIDGETS
#define _CUSTOM_WIDGETS

#include <QtCore>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();
    void addWidget(QWidget *widget);
private:
    QHBoxLayout _Layout;
};

////////////////////////////////////////

class OptionListItemDelegate : public QAbstractItemDelegate
{
public:
    OptionListItemDelegate(bool large = true, QObject *parent = 0);
    void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual ~OptionListItemDelegate();
private:
    void paintLarge(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void paintSmall(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    bool _Large;
    int  _Height;
};

class OptionList : public QListWidget
{
    Q_OBJECT
public:
    OptionList(bool large);
    virtual ~OptionList();
};

////////////////////////////////////////

class DataListItemDelegate : public QAbstractItemDelegate
{
public:
    DataListItemDelegate(QObject *parent = 0);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint (const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual ~DataListItemDelegate();
};

class DataList : public QListWidget
{
    Q_OBJECT
public:
    DataList();
    virtual ~DataList();
};

////////////////////////////////////////

class ActiveLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ActiveLabel( const QString& text="", QWidget* parent=0 );
    ~ActiveLabel();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

////////////////////////////////////////

class RichLabel : public QWidget
{
    Q_OBJECT
public:
    RichLabel(QWidget* ptr = nullptr);
    RichLabel(QString txt, QString fname, QWidget* ptr = nullptr);
    virtual ~RichLabel();
    void setText(QString txt);
    void setImageFile(QString fname);
    void setUnderline(bool fl = true);
    void setLarge(bool largef);
    void setBold(bool boldf);

signals:
    void clicked();

protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString _Text;
    QPixmap _Image;
    bool    _UnderLine;
    bool    _ClickTrackingOn;
    bool    _Large;
    int     _Height;
    bool    _Bold;
};

////////////////////////////////////////

class Button : public QWidget
{
    Q_OBJECT
public:
    Button(QWidget* ptr = nullptr);
    Button(QString txt, QString fname, QWidget* ptr = nullptr);
    virtual ~Button();
    void setText(QString txt);
    void setImageFile(QString fname);

signals:
    void clicked();

protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString _Text;
    QPixmap _Image;
    bool    _ClickTrackingOn;
};

////////////////////////////////////////

class VerticalLine : public QWidget
{
    Q_OBJECT
public:
    VerticalLine(QWidget* ptr = nullptr);
    virtual ~VerticalLine();
protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event) override;
};

#endif
