#ifndef SERVER_H
#define SERVER_H

// qt bases
#include <QObject>
// network tools
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>

#include "mylogger.h" // logging

#include "mythread.h"

/******************************************************************************
** class MyServer                                                             *
*******************************************************************************/
class MyServer : public QTcpServer
{
        Q_OBJECT
    public:
        explicit MyServer(QObject *parent = nullptr);
        virtual ~MyServer();

        void startServer();

        quint16 getPort();
        void setPort(quint16 thisPort);

    protected:
        void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

    private:
        QString myMessage; // not sure what this is for
        quint16 myPort = 0;

}; // end class MyServer
#endif // TASK_H
/* ***************************** End of File ******************************* */
