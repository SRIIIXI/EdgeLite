
#include "MailClient.h"
#include "MainWindow.h"
#include "DataInterface.h"
#include "ContactEntity.h"
#include "AccountEntity.h"
#include "MailBoxEntity.h"
#include "AddressBookWindow.h"
#include "MailCompositionWindow.h"
#include "AccountsWindow.h"
#include "SettingsWindow.h"
#include "ContactAddDialog.h"
#include "ThemeManager.h"
#include "ImageLabel.h"

int main(int argc, char *argv[])
{
    MailClient* app = new MailClient(argc, argv);
    Q_INIT_RESOURCE(Resources);

    ApplicationThemeManager.setAppThemeLight(app);
    ApplicationThemeManager.initFontPreference();

    //Create the main window
    //This will create the main window and the splashscreen pointer
    new MainWindow();

    //Create data interfaces
    new DataInterface();

    QString appnamestr = app->applicationName();

    //Auto generate the SQLite DB filename
    QString datalocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)+ "/" + appnamestr;
    QString dbfilename = datalocation + "/" + appnamestr + ".db";

    QDir dir(datalocation);

    //Check whether our data directory exists or not
    if(!dir.exists())
    {
        mailClientPtr->showMessage("Creating data directory");
        dir.mkpath(datalocation);
    }

    //Intialize the DB interface
    dataInterfacePtr->setParameters(dbfilename, appnamestr);

    //Check for SQLite DB's existence
    if(!dataInterfacePtr->open())
    {
        //Create if not present
        mailClientPtr->showMessage("Creating database");
        dataInterfacePtr->createFile(dbfilename);
    }

    //Now we can open the database
    dataInterfacePtr->open();

    //Check whether the account table is present or not
    new AccountEntity(dataInterfacePtr->getDatabase(), "Accounts");
    if(!dataInterfacePtr->contains("Accounts"))
    {
        mailClientPtr->showMessage("Creating Accounts table");
        accountEntityPtr->createAccountTable();
    }

    //Check whether the account table is present or not
    new MailBoxEntity(dataInterfacePtr->getDatabase(), "MailBox");
    if(!dataInterfacePtr->contains("MailBox"))
    {
        mailClientPtr->showMessage("Creating MailBox table");
        mailBoxEntityPtr->createMailBoxTable();
    }

    //Check whether the account table is present or not
    new ContactEntity(dataInterfacePtr->getDatabase(), "Contacts");
    if(!dataInterfacePtr->contains("Contacts"))
    {
        mailClientPtr->showMessage("Creating Contacts table");
        contactEntityPtr->createContactTable();
    }

    //Create global instances of all subsidiary windows
    new AddressBookWindow();
    new MailCompositionWindow();
    new AccountsWindow();
    new SettingsWindow();
    new ContactAddDialog();

    mailClientPtr->showMessage("Initializing main window");
    if(!mainWindowPtr->initialize())
    {
        QMessageBox msg;
        msg.setText("The application has failed to initialize successfully");
        msg.exec();
    }

    mailClientPtr->showMessage("Creating sub windows");

    mailClientPtr->startOnline();

    addressBookWindowPtr->loadContacts();

    return app->exec();
}
