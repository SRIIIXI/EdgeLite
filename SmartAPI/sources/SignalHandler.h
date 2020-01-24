#ifndef _SIGNAL_HANDLER
#define _SIGNAL_HANDLER

#include "OSDefinitions.h"

// + BSD specific starts
#ifndef SIGSTKFLT
#define SIGSTKFLT 16
#endif
// + BSD specific ends

// + Windows specific starts
// Dummy signal for Windows
#ifndef ERESTART
#define ERESTART 999
#endif
// + Windows specific ends

extern int signalNumbers[];
extern const char *signalNames[];

class SignalCallback
{
public:
    SignalCallback(){}
    virtual void suspend()=0;
    virtual void resume()=0;
    virtual void shutdown()=0;
    virtual void alarm()=0;
    virtual void reset()=0;
    virtual void childExit()=0;
    virtual void userdefined1()=0;
    virtual void userdefined2()=0;
};

class SignalHandler
{
public:
    SignalHandler();
    ~SignalHandler();
    void registerCallbackClient(SignalCallback *clientptr);
    void registerSignalHandlers();
private:
    static bool isShutdownSignal(const int signum);
    void getSignalName(const int signum);
#if defined(WIN32) || defined (_WIN32)
    static void shutdownCallback(int signo);
    void registerSignals();
#else
    void registerSignals();
    static void alarmCallback(int sig, siginfo_t *siginfo, void *context);
    static void suspendCallback(int sig, siginfo_t *siginfo, void *context);
    static void resumeCallback(int sig, siginfo_t *siginfo, void *context);
    static void shutdownCallback(int sig, siginfo_t *siginfo, void *context);
    static void ignoredCallback(int sig, siginfo_t *siginfo, void *context);
    static void resetCallback(int sig, siginfo_t *siginfo, void *context);
    static void user1Callback(int sig, siginfo_t *siginfo, void *context);
    static void user2Callback(int sig, siginfo_t *siginfo, void *context);
    static void childExitCallback(int sig, siginfo_t *siginfo, void *context);
#endif
};

#endif
