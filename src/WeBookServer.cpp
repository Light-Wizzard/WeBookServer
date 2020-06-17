#include "WeBookServer.h"

/******************************************************************************
** WeBookServer Constructor                                                   *
*******************************************************************************/
WeBookServer::WeBookServer(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "WeBookServer"), myApp(nullptr)
{
    qLoggerCommon = new QLogger::QLoggerCommon(true);

    QString thatWeBookList;

    if (QFile(getCatFileName()).exists())
    {
        QFile inputFile(myCatFileName);
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
              QString line = in.readLine();
              if (!line.isNull() && !line.isEmpty())
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
        QLog_Debug(qLoggerCommon->getModuleName(), "WeBookServer Constructor");
    }
    catch (...)
    {
        QLog_Error(qLoggerCommon->getModuleName(), "Error starting WeBookServer");
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
        if (!httpServer->listen(QHostAddress::Any, qLoggerCommon->getPort()))
        {
            QLog_Debug(qLoggerCommon->getModuleName(), QCoreApplication::translate("QHttpServerExample", QString("Server failed to listen on port %1").arg(qLoggerCommon->getPort()).toLocal8Bit()));
        }
        QLog_Debug(qLoggerCommon->getModuleName(), "WeBookServer start");
    }
    catch (...)
    {
        QLog_Error(qLoggerCommon->getModuleName(), "Error starting WeBookServer");
    }
} // end start
/******************************************************************************
** stop                                                                       *
*******************************************************************************/
void WeBookServer::stop()
{
    try
    {
        QLog_Debug(qLoggerCommon->getModuleName(), "WeBookServer stop");
    }
    catch (...)
    {
        QLog_Error(qLoggerCommon->getModuleName(), "Error stopping WeBookServer");
    }
} // end stop
/******************************************************************************
** pause                                                                      *
*******************************************************************************/
void WeBookServer::pause()
{
    try
    {
        QLog_Debug(qLoggerCommon->getModuleName(), "WeBookServer pause");
    }
    catch (...)
    {
        QLog_Error(qLoggerCommon->getModuleName(), "Error pauseing WeBookServer");
    }
} // end pause
/******************************************************************************
** resume                                                                     *
*******************************************************************************/
void WeBookServer::resume()
{
    try
    {
        QLog_Debug(qLoggerCommon->getModuleName(), "WeBookServer resume");
    }
    catch (...)
    {
        QLog_Error(qLoggerCommon->getModuleName(), "Error resumeing WeBookServer");
    }
} // end resume
/******************************************************************************
** startHttpServer                                                            *
*******************************************************************************/
void WeBookServer::startHttpServer()
{
} // end startHttpServer
/******************************************************************************
** getCatFileName                                                             *
*******************************************************************************/
QString WeBookServer::getCatFileName()
{
    if (myCatFileName.isEmpty()) setCatFileName("");
    return myCatFileName;
} // end getCatFileName
/******************************************************************************
** setIniFileName                                                             *
*******************************************************************************/
void WeBookServer::setCatFileName(const QString &thisCatFileName)
{
    if (thisCatFileName.isEmpty())
    {
        myCatFileName = findFilePath(qLoggerCommon->getFilelPath(), QLogger::ConstDefaultWeBookCatName);
    }
    if (myCatFileName.isEmpty() || myCatFileName != thisCatFileName)
    {
        // changing it
        myCatFileName = thisCatFileName;
    }
} // end setCatFileName
/******************************************************************************
** findFilePath(String thisFileName, QString thisDataFolderName)              *
** All files must be in folder myAppFolderName                                 *
** This folder is different for debug vs release as well as deployed          *
*******************************************************************************/
QString WeBookServer::findFilePath(QString thisFileName, QString thisDataFolderName)
{
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
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4%5").arg(QDir::separator()).arg(qLoggerCommon->getFileFolderName()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5%6").arg(QDir::separator()).arg(QDir::separator()).arg(qLoggerCommon->getFileFolderName()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2..%3%4%5%6%7").arg(QDir::separator()).arg(QDir::separator()).arg(QDir::separator()).arg(qLoggerCommon->getFileFolderName()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
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
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4%5%6").arg(QDir::separator()).arg(qLoggerCommon->getFileFolderName()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5%6%7").arg(QDir::separator()).arg(QDir::separator()).arg(qLoggerCommon->getFileFolderName()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2..%3%4%5%6%7%8").arg(QDir::separator()).arg(QDir::separator()).arg(QDir::separator()).arg(qLoggerCommon->getFileFolderName()).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
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
