#include "webookserver.h"

/******************************************************************************
** WeBookServer Constructor                                                   *
*******************************************************************************/
WeBookServer::WeBookServer(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "WeBookServer"), myApp(nullptr)
{

    QString thatWeBookList;

    if (QFile(myCatFileName).exists())
    {
        QFile inputFile(myCatFileName);
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
              QString line = in.readLine();
              if (!line.isNull() && !line.isEmpty()) // FIXME do I need to do both checks?
              {
                  thatWeBookList.append(line);
              }
              //
           }
           inputFile.close();
        }
    }


    try
    {
        setServiceDescription("WeBookServer");
        setServiceFlags(QtServiceBase::CanBeSuspended);
        qDebug() << "WeBookServer Constructor";
    }
    catch (...)
    {
        qCritical() << "Error starting WeBookServer";
    }

} // end WeBookServer
/******************************************************************************
** WeBookServer Deconstructor                                                 *
*******************************************************************************/
WeBookServer::~WeBookServer()
{

} // end ~WeBookServer
/******************************************************************************
** getApp                                                                     *
*******************************************************************************/
QCoreApplication *WeBookServer::getApp()
{
    return myApp;
} // end getApp
/******************************************************************************
** start                                                                      *
*******************************************************************************/
void WeBookServer::start()
{
    try
    {
        myApp = application();
        if (!httpServer->listen(QHostAddress::Any, myPort))
        {
            qDebug() << QCoreApplication::translate("QHttpServerExample", QString("Server failed to listen on port %1").arg(myPort).toLocal8Bit());
        }
        qDebug() << "WeBookServer start";
    }
    catch (...)
    {
        qCritical() << "Error starting WeBookServer";
    }
} // end start
/******************************************************************************
** stop                                                                       *
*******************************************************************************/
void WeBookServer::stop()
{
    try
    {

        qDebug() << "WeBookServer stop";
    }
    catch (...)
    {
        qCritical() << "Error stopping WeBookServer";
    }

} // end stop
/******************************************************************************
** pause                                                                      *
*******************************************************************************/
void WeBookServer::pause()
{
    try
    {
        qDebug() << "WeBookServer pause";
    }
    catch (...)
    {
        qCritical() << "Error pauseing WeBookServer";
    }

} // end pause
/******************************************************************************
** resume                                                                     *
*******************************************************************************/
void WeBookServer::resume()
{
    try
    {
        qDebug() << "WeBookServer resume";
    }
    catch (...)
    {
        qCritical() << "Error resumeing WeBookServer";
    }

} // end resume
/******************************************************************************
** startHttpServer                                                            *
*******************************************************************************/
void WeBookServer::startHttpServer()
{
} // end startHttpServer
/******************************************************************************
** getCharAppName                                                             *
*******************************************************************************/
const char *WeBookServer::getCharAppName()
{
    return myAppName.toLocal8Bit();
} // end getCharAppName
/******************************************************************************
** getAppName                                                                 *
*******************************************************************************/
QString WeBookServer::getAppName()
{
    return myAppName;
} // end getAppName
/******************************************************************************
** setAppName                                                                 *
*******************************************************************************/
void WeBookServer::setAppName(const QString &thisAppName)
{
    if (!myAppName.isNull() && !myAppName.isEmpty() && myAppName != thisAppName)
    {
        // changing it
        myAppName = thisAppName;
    }
    else
    {
        // Setting it for first time
        myAppName = thisAppName;
    }
} // end setAppName
/******************************************************************************
** getOrgName                                                                 *
*******************************************************************************/
QString WeBookServer::getOrgName()
{
    return myOrganizationName;
} // end getOrgName
/******************************************************************************
** setOrgName(myValue)                                                        *
*******************************************************************************/
void WeBookServer::setOrgName(const QString &thisOrgName)
{
    if (!myOrganizationName.isNull() && !myOrganizationName.isEmpty() && myOrganizationName != thisOrgName)
    {
        // changing it
        myOrganizationName = thisOrgName;
    }
    else
    {
        // Setting it for first time
        myOrganizationName = thisOrgName;
    }
} // end setOrgName
/******************************************************************************
** getOrgDomain                                                               *
*******************************************************************************/
QString WeBookServer::getOrgDomain()
{
    return myOrganizationDomain;
} // end getOrgDomain
/******************************************************************************
** setOrgDomain(myValue)                                                      *
*******************************************************************************/
void WeBookServer::setOrgDomain(const QString &thisOrgDomain)
{
    if (!myOrganizationDomain.isNull() && !myOrganizationDomain.isEmpty() && myOrganizationDomain != thisOrgDomain)
    {
        // changing it
        myOrganizationDomain = thisOrgDomain;
    }
    else
    {
        // Setting it for first time
        myOrganizationDomain = thisOrgDomain;
    }
} // end setOrgDomain
/******************************************************************************
** getIniFileName                                                             *
*******************************************************************************/
QString WeBookServer::getIniFileName()
{
    return myIniFileName;
} // end getIniFileName
/******************************************************************************
** setIniFileName                                                             *
*******************************************************************************/
void WeBookServer::setIniFileName(const QString &thisIniFileName)
{
    if (!myIniFileName.isNull() && !myIniFileName.isEmpty() && myIniFileName != thisIniFileName)
    {
        // changing it
        myIniFileName = thisIniFileName;
    }
    else
    {
        // Setting it for first time
        myIniFileName = thisIniFileName;
    }
} // end setIniFileName
/******************************************************************************
** getCatFileName                                                             *
*******************************************************************************/
QString WeBookServer::getCatFileName()
{
    return myCatFileName;
} // end getCatFileName
/******************************************************************************
** setIniFileName                                                             *
*******************************************************************************/
void WeBookServer::setCatFileName(const QString &thisCatFileName)
{
    if (!myCatFileName.isNull() && !myCatFileName.isEmpty() && myCatFileName != thisCatFileName)
    {
        // changing it
        myCatFileName = thisCatFileName;
    }
    else
    {
        // Setting it for first time
        myCatFileName = thisCatFileName;
    }
} // end setCatFileName
/******************************************************************************
** getCryptoKey                                                               *
*******************************************************************************/
QString WeBookServer::getCryptoKey()
{
    return myCryptoKey;
} // end getCryptoKey
/******************************************************************************
** setCryptoKey                                                               *
*******************************************************************************/
void WeBookServer::setCryptoKey(const QString &thisCryptoKey)
{
    if (!myCryptoKey.isNull() && !myCryptoKey.isEmpty() && myCryptoKey != thisCryptoKey)
    {
        // changing it
        myCryptoKey = thisCryptoKey;
    }
    else
    {
        // Setting it for first time
        myCryptoKey = thisCryptoKey;
    }
} // end setCryptoKey
/******************************************************************************
** getCryptoIvVector                                                          *
*******************************************************************************/
QString WeBookServer::getCryptoIvVector()
{
    return myCryptoIvVector;
} // end getCryptoIvVector
/******************************************************************************
** setCryptoIvVector                                                          *
*******************************************************************************/
void WeBookServer::setCryptoIvVector(const QString &thisCryptoIvVector)
{
    if (!myCryptoIvVector.isNull() && !myCryptoIvVector.isEmpty() && myCryptoIvVector != thisCryptoIvVector)
    {
        // changing it
        myCryptoIvVector = thisCryptoIvVector;
    }
    else
    {
        // Setting it for first time
        myCryptoIvVector = thisCryptoIvVector;
    }
} // end setCryptoIvVector
/******************************************************************************
** getPort                                                                    *
*******************************************************************************/
quint16 WeBookServer::getPort()
{
    return myPort;
} // end getPort
/******************************************************************************
** setPort(int thisPort)                                                      *
*******************************************************************************/
void WeBookServer::setPort(quint16 thisPort)
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
** getAppFolderName                                                           *
*******************************************************************************/
QString WeBookServer::getAppFolderName()
{
    return myAppFolderName;
} // end getAppFolderName
/******************************************************************************
** setAppFolderName                                                           *
*******************************************************************************/
void WeBookServer::setAppFolderName(const QString &thisAppFolderName)
{
    if (!myAppFolderName.isNull() && !myAppFolderName.isEmpty() && myAppFolderName != thisAppFolderName)
    {
        // changing it
        myAppFolderName = thisAppFolderName;
    }
    else
    {
        // Setting it for first time
        myAppFolderName = thisAppFolderName;
    }
} // end setAppFolderName
/******************************************************************************
** findFilePath(String thisFileName, QString thisDataFolderName)              *
** All files must be in folder myAppFolderName                                 *
** This folder is different for debug vs release as well as deployed          *
*******************************************************************************/
QString WeBookServer::findFilePath(QString thisFileName, QString thisDataFolderName)
{
    // Make sure Build Folder does not contain the App Name, this is default, change Projects Build Path
    if (constAppDataLocation.contains(myAppFolderName))
    {
        if (QDir(constAppDataLocation).exists())
        {
            // I have no idea where this is at till I deploy it
        }
    }
    // If running from within Qt Creator, this path is outside of the root application folder
    QDir dataFileDir(QCoreApplication::applicationDirPath());
    QString dataFullPath;
    if (thisFileName.isEmpty())
    {
        dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("%1%2").arg(thisDataFolderName).arg(QDir::separator())));
        // If ran from within Qt Creator
        if (!QDir(dataFullPath).exists())
        {
            // APP_FOLDER/databaseFolderName/databaseFileName
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4%5").arg(QDir::separator()).arg(myAppFolderName).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5%6").arg(QDir::separator()).arg(QDir::separator()).arg(myAppFolderName).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2..%3%4%5%6%7").arg(QDir::separator()).arg(QDir::separator()).arg(QDir::separator()).arg(myAppFolderName).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        // look back before the myAppFolderName
        if (!QDir(dataFullPath).exists())
        {
            dataFileDir = QDir(QCoreApplication::applicationDirPath());
            // ../thisDataFolderName/
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3").arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        // look back before the myAppFolderName
        if (!QDir(dataFullPath).exists())
        {
            // ../../thisDataFolderName/
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4").arg(QDir::separator()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = ""; // FIXME
        }
    }
    else
    {
        // full_path/databaseFolderName/databaseFileName
        dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("%1%2%3").arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        // If ran from within Qt Creator
        if (!QFile(dataFullPath).exists())
        {
            // APP_FOLDER/databaseFolderName/databaseFileName
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4%5%6").arg(QDir::separator()).arg(myAppFolderName).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5%6%7").arg(QDir::separator()).arg(QDir::separator()).arg(myAppFolderName).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2..%3%4%5%6%7%8").arg(QDir::separator()).arg(QDir::separator()).arg(QDir::separator()).arg(myAppFolderName).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        // look back before the myAppFolderName
        if (!QFile(dataFullPath).exists())
        {
            dataFileDir = QDir(QCoreApplication::applicationDirPath());
            // ../thisDataFolderName/thisFileName
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4").arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        // look back before the myAppFolderName
        if (!QFile(dataFullPath).exists())
        {
            // ../../thisDataFolderName/thisFileName
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5").arg(QDir::separator()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = ""; // FIXME
        }
    }
    return dataFullPath;
} // end findFilePath
/******************************* End of File *********************************/
