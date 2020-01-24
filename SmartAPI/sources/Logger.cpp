#include "Logger.h"

Logger objLogger;

Logger*  Logger::GetInstance()
{
    return &objLogger;
}

Logger::Logger()
{
    _LogDirectory = "";
    _LogFileSize = 1024;
    _LogFile = NULL;

    char pidstr[16];
    memset((char*)&pidstr[0],0,16);
    sprintf(pidstr,"%d",getpid());
    _ModuleName = pidstr;

    _LogLevelMap.clear();

    _LogLevelMap[LOG_INFO]       ="Information";
    _LogLevelMap[LOG_WARNING]    ="Warning    ";
    _LogLevelMap[LOG_ERROR]      ="Error      ";
    _LogLevelMap[LOG_CRITICAL]   ="Critical   ";
    _LogLevelMap[LOG_PANIC]      ="Panic      ";
}

Logger::~Logger()
{
    stopLogging();
}

void Logger::stopLogging()
{
    if(_LogFile!=NULL)
    {
        fflush(_LogFile);
        fclose(_LogFile);
    }
    _LogLevelMap.clear();
}

void Logger::createBackupFileName(std::string &str)
{
    Timestamp ts;
	std::string tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
    char temp[1024];
    memset((char*)&temp[0],0,16);
    sprintf(temp,"%s_%s.log",_ModuleName.c_str(),tstamp.c_str());
    str = temp;
}

void Logger::startLogging()
{
    if(_LogDirectory.empty() || _LogDirectory.length()<1)
    {
		std::string parent_dir, current_dir;
		dircurrentdirectory(current_dir);
		dirgetparentdirectory(current_dir, parent_dir);

		_LogDirectory = parent_dir + "/log/";

        if(!dirisdirectory(_LogDirectory))
        {
            dircreatedirectory(_LogDirectory);
        }
    }

    _LogFilename = _LogDirectory + _ModuleName + ".log";

    _LogFile = fopen(_LogFilename.c_str(),"w+");
}

void Logger::write(std::string logEntry, LogLevel llevel, const char* func, const char* file, int line)
{
    if(_LogFile!=NULL)
    {
        int sz = ftell(_LogFile);

        if(sz >= _LogFileSize*1024)
        {
			std::string temp;
            createBackupFileName(temp);
			std::string backupfile = _LogBackupDirectory + temp;
            stopLogging();
            rename(_LogFilename.c_str(),backupfile.c_str());
            startLogging();
        }

		std::string sourcefile;
		dirgetname(file, sourcefile);
		std::string lvel = _LogLevelMap[llevel];

        Timestamp ts;
		std::string tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
        char temp[1024];
        memset((char*)&temp[0],0,16);

        char fname[256]={0};
        memcpy(fname,func,255);
        #if defined(_WIN32) || defined(WIN32)
        #else
        int pos = strcharacterpos(fname,'(');
        fname[pos]=0;
        #endif

		std::string left, right;

        strsplit(fname, "::", left, right);
        if(right.length()>1)
        {
            strcpy(fname,right.c_str());
        }

        strsplit(fname, " ", left, right);
        if(right.length()>1)
        {
            strcpy(fname,right.c_str());
        }

        sprintf(temp,"%s|%s|%05d|%s|%s| ",tstamp.c_str(),lvel.c_str(),line,fname,sourcefile.c_str());

        logEntry = temp + logEntry;
        fprintf(_LogFile,"%s\n",logEntry.c_str());
        fflush(_LogFile);
    }
}

void Logger::setModuleName(const char *mname)
{
    int len = strlen(mname);

    int ctr = 0;

    int pos1 = 0;
    int pos2 = 0;

    pos1 = strcharacterpos(mname, '/');
    pos2 = strcharacterpos(mname, '\\');

    if(pos1 > -1 || pos2 > -1)
    {
        for(ctr = len; ; ctr--)
        {
            if(mname[ctr] == '/' || mname[ctr] == '\\')
            {
                break;
            }
        }
        char buffer[32]={0};

        strncpy((char*)&buffer[0], (char*)&mname[ctr+1], 32);

        _ModuleName = buffer;
    }
    else
    {
        _ModuleName = mname;
    }

    strreplace(_ModuleName, ".exe", "");
    strreplace(_ModuleName, ".EXE", "");
}

void Logger::setLogFileSize(int flsz)
{
    _LogFileSize = flsz;
}

void Logger::setLogDirectory(std::string dirpath)
{
    _LogDirectory = dirpath;

    char buffer[2048]={0};

    strcpy(buffer, _LogDirectory.c_str());

    if(buffer[strlen(buffer)-1]== '/' || buffer[strlen(buffer)-1]== '\\')
    {
        buffer[strlen(buffer)-1] = 0;
    }

    strcat(buffer, ".bak/");

    _LogBackupDirectory = buffer;

    if(!dirisdirectory(buffer))
    {
        dircreatedirectory(buffer);
    }
}

void Logger::writeExtended(LogLevel llevel, const char *func, const char *file, int line, const char* format,...)
{
    char tempbuf[1024];
    memset((char*)&tempbuf[0],0,1024);
    va_list args;
    va_start(args, format);
    vsprintf(tempbuf, format, args);
    tempbuf[1023]=0;
    write(tempbuf,llevel,func,file,line);
}


