#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <QDir>
#include <QFileDialog>
#include "IPCamDesktop.h"
#include "MainWindow.h"
#include "ThemeManager.h"

int main(int argc, char *argv[])
{   
    IPCamDesktop* app = new IPCamDesktop(argc, argv);
    Q_INIT_RESOURCE(Resources);

    //Create the main window
    //This will create the main window and the splashscreen pointer
    new MainWindow();

    bool licenseverification = false;

    if(app->licenseFile().length() < 5)
    {
        QString fileName = QFileDialog::getOpenFileName(mainWindowPtr, "Select a license file",  QDir::homePath(), "License files (*.lic)");

        if (!fileName.isEmpty())
        {
            licenseverification = app->verifyLicense(fileName);
        }
    }
    else
    {
        licenseverification = app->verifyLicense();
    }

    if(licenseverification)
    {
        app->showMessage("Initializing main window");
        if(!mainWindowPtr->initialize())
        {
            QMessageBox msg;
            msg.setText("The application has failed to initialize successfully");
            msg.exec();
        }

        app->showMessage("Creating sub windows");
    }
    else
    {
        QMessageBox msg;
        msg.setText("Missing or invalid license file\nInvalid entries (if any) deleted\nPlease restart and select a *.lic file");
        msg.exec();
        exit(0);
    }

    return app->exec();
}
