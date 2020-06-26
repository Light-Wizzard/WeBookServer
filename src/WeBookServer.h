#ifndef WEBOOKSERVER_H
#define WEBOOKSERVER_H

#include <QtCore>
#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QSettings>

#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Qt Service
#include "QtService/QtService"
#include "QHttpServer/httpserver/qhttpserver.h"
// QLogger
#include "QLogger/QLoggerCommon.h"
#include "QLogger/QLoggerCrypto.h"
#include "QLogger/QLoggerConstants.h"

/******************************************************************************
*  class WeBookServer : public QtService<QCoreApplication>                    *
*******************************************************************************/
class WeBookServer : public QtService<QCoreApplication>
{
    public:
        WeBookServer(int argc, char **argv);
        ~WeBookServer();

        QCoreApplication *getApp();                                 //
        //
        QString getCatFileName();                                   // myCatFileName
        void setCatFileName(const QString &thisCatFileName);        // myCatFileName
        //
        QString findFilePath(QString thisFileName, QString thisDataFolderName);

    protected:
        void start() override;
        void stop() override;
        void pause() override;
        void resume() override;
        void startHttpServer();

    private:
        QCoreApplication    *myApp              = nullptr;  //
        QHttpServer         *httpServer         = nullptr;  //
        QString             myCatFileName;                  // Ini File Name
        bool                isDebugMessage      = true;     // Set to true to show debug messages
        bool                isDebugAllMessages  = true;     // Set to true to show debug messages
        QLogger::QLoggerCommon *qLoggerCommon   = nullptr;  //
        QLogger::QLoggerCrypto *qLoggerCrypto   = nullptr;  //
}; // end class WeBookServer
#endif // WEBOOKSERVER_H
/******************************  End of File *********************************/
