#ifndef _STATISTICS_VIEW
#define _STATISTICS_VIEW

#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QtCore>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include "CustomWidgets.h"

class StatisticsView : public QWidget
{
    Q_OBJECT
public:
    StatisticsView(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    RichLabel _Heading;
    QVBoxLayout _Layout;
};

#endif
