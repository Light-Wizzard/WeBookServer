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

#include <iostream>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "QtService/QtService"
#include "QtHttpServer/QHttpServer"

/******************************************************************************
** class WeBookServer : public QtService<QCoreApplication>                    *
*******************************************************************************/
class WeBookServer : public QtService<QCoreApplication>
{
    public:
        WeBookServer(int argc, char **argv);
        ~WeBookServer();

        const QString constAppDataLocation       = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation); // App Data
        const QString constAppFolder             = "webookserver";     // Name of Folder this Project is in
        const QString constDataFolderName        = "data";             // Name of data Folder

        // I do not like hard coding variables into Applications
        // Const Ini File Name Full Path if want to change the Path to the ini or rename it, leaving it just the name, will look for it in the data folder
        const QString ConstIniFileNameFullPath   = "WeBook.ini";                                // normally WeBook.ini, but I make it an option, see above comment
        // These next 3 settings will set the Local Data Storage for this App to use, changing it after it has run, will create a new storage containtainer
        const QString ConstOrganizationName      = "Light Wizzard";                             // This is the User Name for GitHub account, but does not have to be
        const QString ConstOrganizationDomain    = "https://github.com/Light-Wizzard/WeBook";   // This is the User Name and Project or GitHub URL, but does not have to be
        const QString ConstAppName               = "WeBook";                                    // This is the Name of the GitHub Project, but does not have to be
        // I hard code these values here for a reason, not that I would use them, set the Link to include this data, pass in as Arguments
        // These values are for testing only
        const QString ConstDefaultCryptoKey      = "!1@2#3$4%5^6&7*8)9(0)NowPickRand";          // This is used as a Key for Password Encryption/Decryption
        const QString ConstDefaultCryptoIvVector = "PutMoveVector1!2@3#4$NowPickRand";          // This is used as the IV Vector for Password Encryption/Decryption

        QCoreApplication *getApp();
        // Ini File
        QString getIniFileName();                           // getIniFileName
        void setIniFileName(const QString &myValue);        // setIniFileName
        // Organization Name
        QString getOrgName();                               // getOrgName
        void setOrgName(const QString &myValue);            // setOrgName
        // Organization Domain
        QString getOrgDomain();                             // getOrgDomain
        void setOrgDomain(const QString &myValue);          // setOrgDomain
        // Application Name
        QString getAppName();                               // getAppName
        void setAppName(const QString &myValue);            // setAppName
        //
        QString getCryptoKey();                             // getCryptoKey
        void setCryptoKey(const QString &myValue);          // setCryptoKey
        //
        QString getCryptoIvVector();                        // getCryptoIvVector
        void setCryptoIvVector(const QString &myValue);     // setCryptoIvVector
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
        QCoreApplication    *myApp              = nullptr;
        QHttpServer         *httpServer         = nullptr;
        QString             myAppName;
        QString             myIniFileName;                                                     // Ini File Name
        QString             myOrganizationName;                                                // OrganizationName
        QString             myOrganizationDomain;                                              // OrganizationDomain
        QString             myApplicationName;                                                 // ApplicationName
        QString             myCryptoKey;                                                       // Argument to Constructor from main.cpp
        QString             myCryptoIvVector;                                                  // Argument to Constructor from main.cpp
        bool                isDebugMessage      = true;                                      // Set to true to show debug messages
        bool                isDebugAllMessages  = true;                                      // Set to true to show debug messages

}; // end class WeBookServer
#endif // WEBOOKSERVER_H
/******************************* End of File *********************************/
