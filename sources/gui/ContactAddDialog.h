#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTimeEdit>
#include <QPixmap>
#include <QIcon>
#include <QDir>
#include <QFileDialog>
#include <QByteArray>
#include <QBuffer>
#include "ContactEntity.h"

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit ClickableLabel( const QString& text="", QWidget* parent=0 );
    ~ClickableLabel();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

class ContactAddDialog : public QDialog
{
    Q_OBJECT
public:
    ContactAddDialog(QWidget *parent = 0);
    
    Contact getRecord();
    void setRecord(const Contact &rec);
    void resetFields();

private slots:
    void pictureSelect();

private:
    QLabel firstnameLabel;
    QLabel middlenameLabel;
    QLabel lastnameLabel;
    QLabel nicknameLabel;
    QLabel emailLabel;

    QLabel addressLine1Label;
    QLabel addressLine2Label;
    QLabel cityLabel;
    QLabel stateLabel;
    QLabel countryLabel;
    QLabel zipcodeLabel;

    QLabel workphoneLabel;
    QLabel homephoneLabel;
    QLabel faxLabel;
    QLabel mobilenoLabel;
    QLabel websiteLabel;

    QLabel anniversaryLabel;
    QLabel notesLabel;
    QLabel pictureLabel;

    ////////////////////

    QLineEdit firstnameText;
    QLineEdit middlenameText;
    QLineEdit lastnameText;
    QLineEdit nicknameText;
    QLineEdit emailText;

    QLineEdit addressLine1Text;
    QLineEdit addressLine2Text;
    QLineEdit cityText;
    QLineEdit stateText;
    QLineEdit countryText;
    QLineEdit zipcodeText;

    QLineEdit workphoneText;
    QLineEdit homephoneText;
    QLineEdit faxText;
    QLineEdit mobilenoText;
    QLineEdit websiteText;

    QDateTimeEdit anniversaryText;
    QTextEdit notesText;
    QString pixBase64;

    /////////////////////

    QPushButton okButton;
    QPushButton cancelButton;
    ClickableLabel picWidget;

    QGridLayout gLayout;
    QHBoxLayout buttonLayout;
    QVBoxLayout mainLayout;
};

extern ContactAddDialog* contactAddDialogPtr;

#endif
