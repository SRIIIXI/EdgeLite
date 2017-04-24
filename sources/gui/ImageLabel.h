#ifndef _IMAGE_LABEL
#define _IMAGE_LABEL

#include <QtCore>
#include <QWidget>
#include <QPainter>
#include <QPixmap>

class ImageLabel : public QWidget
{
    Q_OBJECT

public:
    ImageLabel(QWidget* ptr = nullptr);
    ImageLabel(QString txt, QString fname, QWidget* ptr = nullptr);
    virtual ~ImageLabel();
    void setText(QString txt);
    void setImageFile(QString fname);
    void setUnderline(bool fl = true);
protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event) override;
private:
    QString _Text;
    QPixmap _Image;
    bool    _UnderLine;
};

#endif
