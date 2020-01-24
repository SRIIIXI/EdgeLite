#ifndef _TIMESTAMP_VIEW
#define _TIMESTAMP_VIEW

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
#include <QDialog>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QCalendarWidget>
#include <QTimeEdit>

class TimestampView : public QDialog
{
    Q_OBJECT
public:
    TimestampView(QWidget* parent = nullptr);
    bool timestampSelected();
    QDateTime timestamp();

private slots:
    void eventOk();
    void eventCancel();

private:
    QVBoxLayout _Layout;

    QCalendarWidget _Calender;
    QTimeEdit _TimeEd;

    QHBoxLayout _CmdLayout;
    QPushButton  _CmdOk;
    QPushButton  _CmdCancel;

    bool _IsTimestampSelected;
};

#endif
