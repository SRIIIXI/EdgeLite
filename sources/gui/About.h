#ifndef _ABOUT
#define _ABOUT

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
#include <QLabel>

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget* parent = nullptr);
private:
    QVBoxLayout _Layout;

    QHBoxLayout _AppLayout;
    QHBoxLayout _QtLayout;

    QLabel  _AppImage;
    QLabel  _AppText;

    QLabel  _QtImage;
    QLabel  _QtText;
};

#endif
