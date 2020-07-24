#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H

#include <QtService/Service>
#include <QtNetwork/QTcpServer>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>
#include <QCommandLineParser>
// Qt Http Server
#define QHTTPSERVER
#ifdef QHTTPSERVER
    // This is getting this from the System,
    // thus it has to be installed
    #include <QtHttpServer/QHttpServer>
#endif
#include "WeBookSettings.h"
#include "WeBookCrypto.h"
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
        qint64 sendToClient(QTcpSocket *tcpSocket, const QString &str);
        QList<QTcpSocket *> getClients();

        QTcpServer *tcpServer = nullptr;
        QTcpSocket *tcpSocket = nullptr;
        QTcpServer *fileServer = nullptr;
        QTcpSocket *fileSocket = nullptr;

    protected:
        bool preStart() override;
        CommandResult onStart() override;
        CommandResult onStop(int &exitCode) override;
        CommandResult onReload() override;
        CommandResult onPause() override;
        CommandResult onResume() override;

    private Q_SLOTS:
        void newConnection();
        void onReceiveData();
        void onAcceptFileConnection();
        void onUpdateFileProgress();
        void onUpdateFileProgressBytes(qint64);
        void onDisplayError(QAbstractSocket::SocketError socketError);
        void onSendFile();
        void onGotDisconnection();

    private:
        static QByteArray host(QTcpSocket *socket);
#ifdef QHTTPSERVER
        QHttpServer         *httpServer       = nullptr;  //
#endif
        QString             myCatFileName;                // Cat File Name
        WeBookSettings     *weBookSettings    = nullptr;  //
        WeBookCrypto       *weBookCrypto      = nullptr;  //

        QString fileName;
        QFile *localFile        = nullptr;
        QByteArray inBlock;
        QByteArray outBlock;
        qint64 totalBytes       = 0;
        qint64 bytesReceived    = 0;
        qint64 bytesToWrite     = 0;
        qint64 bytesWritten     = 0;
        qint64 fileNameSize     = 0;
        qint64 perDataSize      = 64 * 1024;
        qint64 nextBlockSize    = 0;
        QList<QTcpSocket*> clients;
}; // end class WeBookServer
#endif // WEBOOKSERVER_H
/******************************  End of File *********************************/
