#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H

#include <QtService/Service>
#include <QtNetwork/QTcpServer>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>
// Qt Http Server
#define QHTTPSERVER
#ifdef QHTTPSERVER
    // This is getting this from the System,
    // thus it has to be installed
    #include <QtHttpServer/QHttpServer>
#endif
// QLogger
//#include "QLogger/QLoggerConstants.h"
//#include "QLogger/QLoggerCommon.h"
//#include "QLogger/QLoggerCrypto.h"
/******************************************************************************
*  class WeBookServer : public QtService<QCoreApplication>                    *
*******************************************************************************/
class WeBookServer : public QtService::Service
{
        Q_OBJECT

    public:
        explicit WeBookServer(int &argc, char **argv);

        QString getCatFileName();                                   // myCatFileName
        void setCatFileName(const QString &thisCatFileName);        // myCatFileName
        //
        //QString findFilePath(QString thisFileName, QString thisDataFolderName);

    protected:
        bool preStart() override;
        CommandResult onStart() override;
        CommandResult onStop(int &exitCode) override;
        CommandResult onReload() override;
        CommandResult onPause() override;
        CommandResult onResume() override;

    private Q_SLOTS:
        void newConnection();

    private:
        QTcpServer *_server = nullptr;
        static QByteArray host(QTcpSocket *socket);
#ifdef QHTTPSERVER
        QHttpServer         *httpServer         = nullptr;  //
#endif
        QString             myCatFileName;                  // Ini File Name
        //QLogger::QLoggerCommon *qLoggerCommon   = nullptr;  //
        //QLogger::QLoggerCrypto *qLoggerCrypto   = nullptr;  //
}; // end class WeBookServer
#endif // WEBOOKSERVER_H
/******************************  End of File *********************************/
