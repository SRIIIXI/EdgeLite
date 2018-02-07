#ifndef _APP_SETTINGS
#define _APP_SETTINGS

#include <QtCore>

typedef struct AppSettings
{
   QString DatasourceName;
   QString DatabaseHost;
   QString DatabasePort;
   QString DatabaseUser;
   QString DatabasePass;
   QString Style;
   QString RecordingsDirectory;
}AppSettings;

#endif
