#ifndef _APP_SETTINGS
#define _APP_SETTINGS

#include <QtCore>

typedef struct AppSettings
{
   QString Host;
   QString Port;
   QString User;
   QString Pass;
   QString LicenseFile;
}AppSettings;

#endif
