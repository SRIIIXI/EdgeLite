#ifndef _CAM_NAME_DIALOG
#define _CAM_NAME_DIALOG

#include <QDialog>
#include <QGridLayout>
#include <QtWidgets>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class CamNameDialog : public QDialog
{
    Q_OBJECT
public:
    CamNameDialog();
    CamNameDialog(QWidget * parent);

    virtual ~CamNameDialog();

    QString getCamFriendlyName();
    QString getCamSDCardPort();

    void setCamFriendlyName(QString str);
    void setCamSDCardPort(QString str);

private:
    QGridLayout dialogLayout;

    QLabel lblCamName;
    QLabel lblCamSDPort;

    QLineEdit txtCamName;
    QLineEdit txtCamSDPort;

    QPushButton cmdOk;

    void initialize();
private slots:
    void dialogClose();
};
#endif
