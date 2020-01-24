#ifndef _OS_DEFINITIONS
#define _OS_DEFINITIONS

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// C headers
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>

// C++ Headers
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <cctype>
#include <ctime>
#include <ios>
#include <fstream>

// OS/Platform specfic headers - typically Windows vs *NIX
#if defined(_WIN32) || defined(WIN32)
#undef UNICODE
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <WinBase.h>
#include <Windows.h>
#include <process.h>
#include <direct.h>
#include <TlHelp32.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pwd.h>
#endif

#if defined(_WIN32) || defined(WIN32)
    #define LAST_ERROR    ::GetLastError()
    #define SOCK_ERROR    ::WSAGetLastError()
    #define strtoull(str, endptr, base) _strtoui64(str, endptr, base)
    #define procsleep(n) ::Sleep(n*1000)
    #define getpid()	_getpid()
	#define pid_t    long
	#define tzset()	_tzset()
	#define snprintf _snprintf 
	#define vsnprintf _vsnprintf 
	#define strcasecmp _stricmp 
	#define strncasecmp _strnicmp 
#else
	#define procsleep(n) sleep(n)
    #define LAST_ERROR    errno
    #define SOCK_ERROR    errno
    #define LPVOID void*
    #define DWORD	unsigned long
    #define WINAPI	__stdcall
    #define LPARAM	long
    #define WPARAM	unsigned int
	#define SOCKET int
#endif

#endif
