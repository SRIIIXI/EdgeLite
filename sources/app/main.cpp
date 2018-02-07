#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <QDir>
#include <QFileDialog>
#include "Tracks.h"
#include "MainWindow.h"
#include "ThemeManager.h"

int main(int argc, char *argv[])
{   
    Tracks* app = new Tracks(argc, argv);
    Q_INIT_RESOURCE(Resources);

    //Create the main window
    //This will create the main window and the splashscreen pointer
    new MainWindow();

    app->showMessage("Initializing main window");
    if(!mainWindowPtr->initialize())
    {
        QMessageBox msg;
        msg.setText("The application has failed to initialize successfully");
        msg.exec();
    }

    app->showMessage("Creating sub windows");

    app->start();

    return app->exec();
}
