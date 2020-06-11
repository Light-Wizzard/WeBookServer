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
#include "QtHttpServer/QHttpServer"
// QLogger
#include "QLogger.h"

//static QFile myLogFileHandle;
//static QString myLogPathFileName = "WeBookClient.log";
/******************************************************************************
** class WeBookServer : public QtService<QCoreApplication>                    *
*******************************************************************************/
class WeBookServer : public QtService<QCoreApplication>
{
    public:
        WeBookServer(int argc, char **argv);
        ~WeBookServer();

        const QString constAppDataLocation       = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation); // App Data

        QCoreApplication *getApp();                                 //
        // Ini File
        QString getIniFileName();                                   // myIniFileName
        void setIniFileName(const QString &thisIniFileName);        // myIniFileName
        // Organization Name
        QString getOrgName();                                       // myOrganizationName
        void setOrgName(const QString &thisOrgName);                // myOrganizationName
        // Organization Domain
        QString getOrgDomain();                                     // myOrganizationDomain
        void setOrgDomain(const QString &thisOrgDomain);            // myOrganizationDomain
        // Application Name
        QString getAppName();                                       // myApplicationName
        void setAppName(const QString &thisAppName);                // myApplicationName
        //
        QString getCryptoKey();                                     // myCryptoKey
        void setCryptoKey(const QString &thisCryptoKey);            // myCryptoKey
        //
        QString getCryptoIvVector();                                // myCryptoIvVector
        void setCryptoIvVector(const QString &thisCryptoIvVector);  // myCryptoIvVector
        //
        quint16 getPort();
        void setPort(quint16 thisPort);
        //
        QString getAppFolderName();                                 // myAppFolderName
        void setAppFolderName(const QString &thisAppFolderName);    // myAppFolderName
        //
        QString getCatFileName();                                   // myCatFileName
        void setCatFileName(const QString &thisCatFileName);        // myCatFileName
        //
        const char *getCharAppName();
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
        QString             myAppName;                      //
        QString             myAppFolderName;                //
        QString             myIniFileName;                  // Ini File Name
        QString             myCatFileName;                  // Ini File Name
        QString             myOrganizationName;             // OrganizationName
        QString             myOrganizationDomain;           // OrganizationDomain
        QString             myApplicationName;              // ApplicationName
        QString             myCryptoKey;                    // Argument to Constructor from main.cpp
        QString             myCryptoIvVector;               // Argument to Constructor from main.cpp
        bool                isDebugMessage      = true;     // Set to true to show debug messages
        bool                isDebugAllMessages  = true;     // Set to true to show debug messages
        quint16             myPort              = 9696;     //

}; // end class WeBookServer
#endif // WEBOOKSERVER_H
/******************************* End of File *********************************/
