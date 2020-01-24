#include "SignalHandler.h"
#include "Logger.h"

#if !defined(WIN32) || !defined(_WIN32)
int signalNumbers[] = {SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGSEGV, SIGPIPE, SIGTERM, SIGSTKFLT, SIGUSR1, SIGUSR2, SIGCHLD};
const char *signalNames[] = {"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGSEGV", "SIGPIPE", "SIGTERM", "SIGSTKFLT", "SIGUSR1", "SIGUSR2", "SIGCHLD"};
#else
int signalNumbers[] = {SIGINT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGTERM};
const char *signalNames[] = {"SIGINT", "SIGILL", "SIGABRT", "SIGFPE", "SIGSEGV", "SIGTERM"};
#endif

char signalNameString[16]={0};

SignalCallback *callback = NULL;


SignalHandler::SignalHandler()
{
}

SignalHandler::~SignalHandler()
{
}

void SignalHandler::registerCallbackClient(SignalCallback *clientptr)
{
    callback = clientptr;
}

void SignalHandler::registerSignalHandlers()
{
    registerSignals();
}

#if defined(WIN32) || defined (_WIN32)
void SignalHandler::shutdownCallback(int signo)
{
    if(callback && signo != SIGSEGV)
    {
        callback->shutdown();
    }
}
#endif

void SignalHandler::getSignalName(const int signum)
{
    int ctr = 0;

    memset((char*)&signalNameString[0], 0, sizeof(signalNameString));
    strcpy(signalNameString, "<Not Named>");

    #if !defined(WIN32) || !defined(_WIN32)
        for(ctr = 0; ctr < 15; ctr++)
        {
            if(signalNumbers[ctr] == signum)
            {
                memset((char*)&signalNameString[0], 0, sizeof(signalNameString));
                strcpy(signalNameString, signalNames[ctr]);
                break;
            }
        }
    #else
        for(ctr = 0; ctr < 6; ctr++)
        {
            if(signalNumbers[ctr] == signum)
            {
                memset((char*)&signalNameString[0], 0, sizeof(signalNameString));
                strcpy(signalNameString, signalNames[ctr]);
                break;
            }
        }
    #endif
}

bool SignalHandler::isShutdownSignal(const int signum)
{
    int ctr = 0;

    bool found = false;

    #if !defined(WIN32) || !defined(_WIN32)
        for(ctr = 0; ctr < 15; ctr++)
        {
            if(signalNumbers[ctr] == signum)
            {
                found = true;
                break;
            }
        }
    #else
        for(ctr = 0; ctr < 6; ctr++)
        {
            if(signalNumbers[ctr] == signum)
            {
                found = true;
                break;
            }
        }
    #endif

    return found;
}

#if defined(WIN32) || defined(_WIN32)
void SignalHandler::registerSignals()
{
    if(signal(SIGINT, SignalHandler::shutdownCallback) == SIG_ERR)
    {
    }

    if(signal(SIGILL, SignalHandler::shutdownCallback) == SIG_ERR)
    {
    }

    if(signal(SIGABRT, SignalHandler::shutdownCallback) == SIG_ERR)
    {
    }

    if(signal(SIGFPE, SignalHandler::shutdownCallback) == SIG_ERR)
    {
    }

    if(signal(SIGSEGV, SignalHandler::shutdownCallback) == SIG_ERR)
    {
    }

    if(signal(SIGTERM, SignalHandler::shutdownCallback) == SIG_ERR)
    {
    }
}
#else
void SignalHandler::registerSignals()
{
    struct sigaction act;

    for(int signum = 1; signum < 32; signum++)
    {
        getSignalName(signum);

        memset (&act, '\0', sizeof(act));
        act.sa_flags = SA_SIGINFO;

        switch(signum)
        {
            case SIGKILL:
            case SIGSTOP:
            {
                continue;
            }
            case SIGINT:
            case SIGQUIT:
            case SIGILL:
            case SIGTRAP:
            case SIGABRT:
            case SIGBUS:
            case SIGFPE:
            case SIGSEGV:
            case SIGPIPE:
            case SIGTERM:
            case SIGSTKFLT:
            {
                act.sa_sigaction = &SignalHandler::shutdownCallback;
                break;
            }
            case SIGALRM:
            {
                act.sa_sigaction = &SignalHandler::alarmCallback;
                break;
            }
            case SIGTSTP:
            {
                act.sa_sigaction = &SignalHandler::suspendCallback;
                break;
            }
            case SIGCONT:
            {
                act.sa_sigaction = &SignalHandler::resumeCallback;
                break;
            }
            case SIGHUP:
            {
                act.sa_sigaction = &SignalHandler::resetCallback;
                break;
            }
            case SIGCHLD:
            {
                act.sa_sigaction = &SignalHandler::childExitCallback;
                break;
            }
            case SIGUSR1:
            {
                act.sa_sigaction = &SignalHandler::user1Callback;
                break;
            }
            case SIGUSR2:
            {
                act.sa_sigaction = &SignalHandler::user2Callback;
                break;
            }
            default:
            {
                act.sa_sigaction = &SignalHandler::ignoredCallback;
            }
        }

        if (sigaction(signum, &act, NULL) < 0)
        {
        }
        else
        {
        }
    }
}

void SignalHandler::suspendCallback(int sig, siginfo_t *siginfo, void *context)
{
    if(callback)
    {
        callback->suspend();
    }
}

void SignalHandler::resumeCallback(int sig, siginfo_t *siginfo, void *context)
{
    if(callback)
    {
        callback->resume();
    }
}

void SignalHandler::shutdownCallback(int sig, siginfo_t *siginfo, void *context)
{
    if(sig == SIGSEGV)
    {
        exit(1);
    }

    if(callback)
    {
        callback->shutdown();
    }
}

void SignalHandler::ignoredCallback(int sig, siginfo_t *siginfo, void *context)
{
    if(SignalHandler::isShutdownSignal(sig))
    {
        shutdownCallback(sig, siginfo, context);
        return;
    }
}

void SignalHandler::alarmCallback(int sig, siginfo_t *siginfo, void *context)
{
    if(callback)
    {
        callback->alarm();
    }
}

void SignalHandler::resetCallback(int sig, siginfo_t *siginfo, void *context)
{
    if(callback)
    {
        callback->reset();
    }
}

void SignalHandler::childExitCallback(int sig, siginfo_t *siginfo, void *context)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
    }

    if(callback)
    {
        callback->childExit();
    }
}

void SignalHandler::user1Callback(int sig, siginfo_t *siginfo, void *context)
{
    if(callback)
    {
        callback->userdefined1();
    }
}

void SignalHandler::user2Callback(int sig, siginfo_t *siginfo, void *context)
{
    if(callback)
    {
        callback->userdefined2();
    }
}



#endif
