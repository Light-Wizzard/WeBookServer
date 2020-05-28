#include "mythread.h"
/******************************************************************************
** MyThread Constructor                                                       *
*******************************************************************************/
MyThread::MyThread(int sockDescr, QObject *parent) : QThread(parent), mySocketFileDescriptor(sockDescr)
{
    mySocketFileDescriptor = sockDescr;
} // end MyThread
/******************************************************************************
** MyThread Deconstructor                                                     *
*******************************************************************************/
MyThread::~MyThread()
{
    if (myTcpSocket->isValid()) myTcpSocket->close();
    myData.clear();
    myData = nullptr;
    mySocketFileDescriptor = -1;
    myTcpSocket = nullptr;
} // end ~MyThread
/******************************************************************************
** run                                                                        *
*******************************************************************************/
/** overriding the run form thread
 * @brief MyThread::run
 */
void MyThread::run()
{

    MyLogger::logMessage(QString("Thread %1 id starterd...\n").arg(*(int*)QThread::currentThreadId()));

    myTcpSocket = new QTcpSocket();
    if (!myTcpSocket->setSocketDescriptor(mySocketFileDescriptor))
    {
        MyLogger::logMessage(QString("Error: could not set sock descriptor\n"));
        emit handleError(myTcpSocket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    // This makes the slot to be invoked immediately, when the signal is emitted.
    connect(myTcpSocket, &QTcpSocket::readyRead,    this, &MyThread::onReadyRead, Qt::DirectConnection);
    connect(myTcpSocket, &QTcpSocket::disconnected, this, &MyThread::onDisconnected);
    connect(myTcpSocket, &QTcpSocket::connected,    this, &MyThread::onConnected);

    MyLogger::logMessage(QString("%1 Client connected\n").arg(mySocketFileDescriptor));

    exec(); // make this thread an event loop
} // end run
/******************************************************************************
** onReadyRead                                                                *
*******************************************************************************/
void MyThread::onReadyRead()
{
    // if (myTcpSocket->canReadLine())
    myData = myTcpSocket->readAll();
    MyLogger::logMessage(QString("Writing data...\n%1 Data in: %2 \n").arg(mySocketFileDescriptor).arg(QString(myData)));
    myTcpSocket->write(myData);
} // end
/******************************************************************************
** onConnected                                                                *
*******************************************************************************/
void MyThread::onConnected()
{
    if (myTcpSocket->isValid())
    {
        MyLogger::logMessage(QString("Connected to %1 \n").arg(mySocketFileDescriptor));
    }
} // end onConnected
/******************************************************************************
** onDisconnected                                                             *
*******************************************************************************/
void MyThread::onDisconnected()
{
    if (myTcpSocket->isValid())
    {
        MyLogger::logMessage(QString("%1 Disconnected!\n").arg(mySocketFileDescriptor));
    }
    myTcpSocket->deleteLater();
    exit(0);
} // end onDisconnected
/* ***************************** End of File ******************************* */
