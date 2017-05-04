#include "ContactAddDialog.h"
#include "ThemeManager.h"

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}

ContactAddDialog::ContactAddDialog(QWidget *parent)
    : QDialog(parent)
{
    firstnameLabel.setText("First Name");
    middlenameLabel.setText("Middle Name");
    lastnameLabel.setText("Last Name");
    nicknameLabel.setText("Nick Name");
    emailLabel.setText("Email");

    workphoneLabel.setText("Work Phone");
    homephoneLabel.setText("Home Phone");
    faxLabel.setText("Fax");
    mobilenoLabel.setText("Mobile Phone");
    websiteLabel.setText("Website");

    addressLine1Label.setText("Address Line 1");
    addressLine2Label.setText("Address Line 2");
    cityLabel.setText("City");
    stateLabel.setText("State");
    countryLabel.setText("Country");
    zipcodeLabel.setText("Zip Code");

    anniversaryLabel.setText("Date Of Birth");
    notesLabel.setText("Notes");
    pictureLabel.setText("Picture");

    okButton.setText("OK");
    cancelButton.setText("Cancel");
    picWidget.setText("Click to \n attach picture");
    picWidget.setFixedSize(QSize(150, 150));

    gLayout.setColumnStretch(1, 2);

    gLayout.addWidget(&firstnameLabel, 0, 0);
    gLayout.addWidget(&firstnameText, 0, 1);
    gLayout.addWidget(&middlenameLabel, 1, 0);
    gLayout.addWidget(&middlenameText, 1, 1);
    gLayout.addWidget(&lastnameLabel, 2, 0);
    gLayout.addWidget(&lastnameText, 2, 1);
    gLayout.addWidget(&nicknameLabel, 3, 0);
    gLayout.addWidget(&nicknameText, 3, 1);
    gLayout.addWidget(&emailLabel, 4, 0);
    gLayout.addWidget(&emailText, 4, 1);

    gLayout.addWidget(&workphoneLabel, 0, 2);
    gLayout.addWidget(&workphoneText, 0, 3);
    gLayout.addWidget(&homephoneLabel, 1, 2);
    gLayout.addWidget(&homephoneText, 1, 3);
    gLayout.addWidget(&faxLabel, 2, 2);
    gLayout.addWidget(&faxText, 2, 3);
    gLayout.addWidget(&mobilenoLabel, 3, 2);
    gLayout.addWidget(&mobilenoText, 3, 3);
    gLayout.addWidget(&websiteLabel, 4, 2);
    gLayout.addWidget(&websiteText, 4, 3);

    gLayout.addWidget(&addressLine1Label, 6, 0);
    gLayout.addWidget(&addressLine1Text, 6, 1);
    gLayout.addWidget(&addressLine2Label, 6, 2);
    gLayout.addWidget(&addressLine2Text, 6, 3);
    gLayout.addWidget(&cityLabel, 7, 0);
    gLayout.addWidget(&cityText, 7, 1);
    gLayout.addWidget(&stateLabel, 7, 2);
    gLayout.addWidget(&stateText, 7, 3);
    gLayout.addWidget(&countryLabel, 8, 0);
    gLayout.addWidget(&countryText, 8, 1);
    gLayout.addWidget(&zipcodeLabel, 8, 2);
    gLayout.addWidget(&zipcodeText, 8, 3);

    gLayout.addWidget(&notesLabel, 9, 1);
    gLayout.addWidget(&notesText, 10, 1);
    gLayout.addWidget(&anniversaryLabel, 9, 2, Qt::AlignTop);
    gLayout.addWidget(&anniversaryText, 9, 3, Qt::AlignTop);
    gLayout.addWidget(&picWidget, 10, 3, Qt::AlignCenter);

    buttonLayout.addWidget(&okButton);
    buttonLayout.addWidget(&cancelButton);

    gLayout.addLayout(&buttonLayout, 11, 3, Qt::AlignRight);

    mainLayout.addLayout(&gLayout);
    setLayout(&mainLayout);

    connect(&okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(&cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
    connect(&picWidget, &ClickableLabel::clicked, this, &ContactAddDialog::pictureSelect);

    setWindowTitle(tr("Add a Contact"));
    QIcon icon(":/images/contact.png");
    setWindowIcon(icon);
}

void ContactAddDialog::pictureSelect()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Set picture",  QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");

    if(fileName.length() < 1)
    {
        return;
    }

    QPixmap pix;
    pix.load(fileName);
    QPixmap scaledpix = pix.scaled(150, 150);

    picWidget.setPixmap(scaledpix);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    scaledpix.save(&buffer, "PNG");
    pixBase64 = QString::fromLatin1(byteArray.toBase64().data());
}

Contact ContactAddDialog::getRecord()
{
    Contact struc;

    struc.FirstName = firstnameText.text();
    struc.MiddleName = middlenameText.text();
    struc.LastName = lastnameText.text();
    struc.EmailId = emailText.text();
    struc.WebSite = websiteText.text();
    struc.AddressLine1 = addressLine1Text.text();
    struc.AddressLine2 = addressLine2Text.text();
    struc.City = cityText.text();
    struc.State = stateText.text();
    struc.Country = countryText.text();
    struc.ZipCode = zipcodeText.text();
    struc.WorkPhone = workphoneText.text();
    struc.HomePhone = homephoneText.text();
    struc.Fax = faxText.text();
    struc.MobileNo = mobilenoText.text();
    struc.NickName = nicknameText.text();
    struc.Anniversary = anniversaryText.dateTime().toUTC().toString();
    struc.Notes = notesText.toPlainText();
    struc.Photo = pixBase64;

    return struc;
}

void ContactAddDialog::setRecord(const Contact &rec)
{
    firstnameText.setText(rec.FirstName);
    middlenameText.setText(rec.MiddleName);
    lastnameText.setText(rec.LastName);
    emailText.setText(rec.EmailId);
    websiteText.setText(rec.WebSite);
    addressLine1Text.setText(rec.AddressLine1);
    addressLine2Text.setText(rec.AddressLine2);
    cityText.setText(rec.City);
    stateText.setText(rec.State);
    countryText.setText(rec.Country);
    zipcodeText.setText(rec.ZipCode);
    workphoneText.setText(rec.WorkPhone);
    homephoneText.setText(rec.HomePhone);
    faxText.setText(rec.Fax);
    mobilenoText.setText(rec.MobileNo);
    nicknameText.setText(rec.NickName);
    anniversaryText.dateTime().toUTC().toString();
    notesText.setPlainText(rec.Notes);
    pixBase64 = rec.Photo;

    QByteArray base64buff;
    base64buff.append(pixBase64);
    QByteArray by = QByteArray::fromBase64(base64buff);
    QImage image = QImage::fromData(by, "PNG");
    QPixmap scaledpix = QPixmap::fromImage(image);
    picWidget.setPixmap(scaledpix);

    emailText.setEnabled(false);
}

void ContactAddDialog::resetFields()
{
    firstnameText.setText("");
    middlenameText.setText("");
    lastnameText.setText("");
    emailText.setText("");
    websiteText.setText("");
    addressLine1Text.setText("");
    addressLine2Text.setText("");
    cityText.setText("");
    stateText.setText("");
    countryText.setText("");
    zipcodeText.setText("");
    workphoneText.setText("");
    homephoneText.setText("");
    faxText.setText("");
    mobilenoText.setText("");
    nicknameText.setText("");
    anniversaryText.dateTime().toUTC().toString();
    notesText.setPlainText("");

    picWidget.setText("Click to \n attach picture");

    emailText.setEnabled(true);
}


