#ifndef _LOGGER
#define _LOGGER

#include "OSDefinitions.h"
#include "StringEx.h"
#include "Directory.h"
#include "Timestamp.h"

#if defined(_WIN32) || defined(WIN32)
#define __FUNCTIONNAME__ __FUNCTION__
#else
#define __FUNCTIONNAME__ __PRETTY_FUNCTION__
#endif

typedef enum LogLevel
{
    LOG_INFO=0,
    LOG_ERROR=1,
    LOG_WARNING=2,
    LOG_CRITICAL=3,
    LOG_PANIC=4
}LogLevel;

class Logger
{
public:
	Logger();
	~Logger();

    void    startLogging();
    void    stopLogging();
    void    write(std::string logEntry, LogLevel llevel, const char* func, const char* file, int line);
    void    writeExtended(LogLevel llevel, const char* func, const char* file, int line, const char* format,...);
    void    setLogFileSize(int flsz);
    void    setLogDirectory(std::string dirpath);
    void    setModuleName(const char* mname);
    static Logger*  GetInstance();
private:
    void createBackupFileName(std::string &str);
	std::string _LogFilename;
	std::string  _LogDirectory;
	std::string  _LogBackupDirectory;
    int     _LogFileSize;
	std::string  _ModuleName;
    FILE*   _LogFile;
    std::map<LogLevel, std::string> _LogLevelMap;
};

#define writeLog(str,level) Logger::GetInstance()->write(str,level,__FUNCTIONNAME__,__FILE__,__LINE__);
#define writeLogNormal(str) Logger::GetInstance()->write(str,LOG_INFO,__FUNCTIONNAME__,__FILE__,__LINE__);

#endif

