#include <QCoreApplication>

// this is the simples qt daemon example
#include <unistd.h>

// tasks
#include "myserver.h"
/******************************************************************************
** main                                                                       *
*******************************************************************************/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // daemonize it
    switch (fork())
    {
        case 0:
            break;
        case 1:
            exit(0);
        default:
            exit(0);
    }
    // we daemonized the ev loop
    // we must create a signal handler or timer event to do something
    // a class to handle something for a test

    MyServer *myServer = new MyServer(a.parent());
    myServer->setPort(6633);
    myServer->startServer();
    return a.exec();
} // end main
/* ***************************** End of File ******************************* */
