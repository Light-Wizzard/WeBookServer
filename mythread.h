#ifndef MYTHREAD_H
#define MYTHREAD_H
// multithreading
#include <QThread>
// network tools
#include <QTcpSocket>
#include <QTcpServer>

#include "mylogger.h" // logging

/******************************************************************************
** class MyThread                                                             *
** An extra interface to implement the unix 32 signals may be needed handle signals
*******************************************************************************/
class MyThread : public QThread
{
        Q_OBJECT
    public:
        explicit MyThread(int sockDescr, QObject *parent = nullptr);
        virtual ~MyThread();

        // override the thread runnable
        void run() Q_DECL_OVERRIDE;

    signals:
        void handleError(QTcpSocket::SocketError socketError);

    public slots:
        void onReadyRead();
        void onDisconnected();
        void onConnected();

    private:
        QTcpSocket *myTcpSocket = nullptr;
        int         mySocketFileDescriptor;
        QByteArray  myData;

}; // end class MyThread
#endif // MYTHREAD_H
/* ***************************** End of File ******************************* */

