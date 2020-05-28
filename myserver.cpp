#include "myserver.h"
/******************************************************************************
** MyServer Constructor                                                       *
*******************************************************************************/
MyServer::MyServer(QObject *parent)  : QTcpServer(parent)
{

} // end MyServer
/******************************************************************************
** MyServer Deconstructor                                                     *
*******************************************************************************/
MyServer::~MyServer()
{

} // end ~MyServer
/******************************************************************************
** getPort                                                                    *
*******************************************************************************/
quint16 MyServer::getPort()
{
    return myPort;
} // end getPort
/******************************************************************************
** setPort(int thisPort)                                                      *
*******************************************************************************/
void MyServer::setPort(quint16 thisPort)
{
    if (myPort != 0 && myPort != thisPort)
    {
        // Update
        myPort = thisPort;
    }
    else
    {
        // Set first time
        myPort = thisPort;
    }
} // end setPort
/******************************************************************************
** startServer                                                                *
*******************************************************************************/
void MyServer::startServer()
{
    if (!this->listen(QHostAddress::Any, myPort))
    {
        MyLogger::logMessage("Could not start server!\n");
        exit(1);
    }
    else
    {
        MyLogger::logMessage(QString("Listening to port...%1\n").arg(myPort));
    }
} // end startServer
/******************************************************************************
** incomingConnection(qintptr socketDescriptor)                               *
*******************************************************************************/
void MyServer::incomingConnection(qintptr socketDescriptor)
{
    MyLogger::logMessage(QString("%1 connecting... \n").arg(socketDescriptor));
    MyThread *pThread = new MyThread(socketDescriptor, this);
    connect(pThread, &QThread::finished, pThread, &QThread::deleteLater);
    pThread->start();
} // end incomingConnection
/* ***************************** End of File ******************************* */
