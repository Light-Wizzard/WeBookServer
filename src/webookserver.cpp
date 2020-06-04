#include "webookserver.h"

/******************************************************************************
** WeBookServer Constructor                                                   *
*******************************************************************************/
WeBookServer::WeBookServer(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "WeBookServer"), myApp(nullptr)
{

    QString applicationName;
    // From *.pro file TARGET   = WeBook, maybe getTarget?
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({{"a", "appname"}, "Application Name if you want it different then executable name.", "appname"});
    parser.addOption({{"i", "ini"}, "The Ini File Name: settings.ini", "inifile"});
    parser.addOption({{"n", "name"}, "The Organization Name going to be the Window Title", "orgname"});
    parser.addOption({{"d", "domain"}, "The Organization Domain URL: https://github.com/USERNAME/PROJECT", "orgdomain"});
    parser.addOption({{"k", "key"}, "The Crypto Key String", "key"});
    parser.addOption({{"c", "cryptoiv"}, "The Crypto IV Vector String", "cryptoiv"});
    parser.addOption({{"w", "webook"}, "The WeBook.cat file name and full path: /path/WeBooks.cat", "webook"});
    parser.addOption({{"p", "pathdata"}, "The Path to the Data", "pathdata"});

    const QStringList positionalArguments = parser.positionalArguments();
    if (!positionalArguments.isEmpty())
    {
        parser.process(positionalArguments);
    }

    // QStringList
    applicationName = argv[0];
    if (applicationName.isEmpty()) applicationName    = ConstAppName;

    QString thatAppName                 = parser.value("appname");
    QString thatInitFile                = parser.value("inifile");
    QString thatOrgName                 = parser.value("orgname");
    QString thatOrgDomain               = parser.value("orgdomain");
    QString thatCrypoKey                = parser.value("key");
    QString thatCryptoIv                = parser.value("cryptoiv");
    QString thatPathData                = parser.value("pathdata");
    QString thatWeBookCatFilePathName   = parser.value("webook");
    //
    bool isArgEmpty = false;
    if (thatAppName.isEmpty())   thatAppName   = applicationName;
    if (thatInitFile.isEmpty())  thatInitFile  = ConstIniFileNameFullPath;
    if (thatOrgName.isEmpty())   thatOrgName   = ConstOrganizationName;
    if (thatOrgDomain.isEmpty()) thatOrgDomain = ConstOrganizationDomain;
    if (thatCrypoKey.isEmpty())  thatCrypoKey  = ConstDefaultCryptoKey;
    if (thatCryptoIv.isEmpty())  thatCryptoIv  = ConstDefaultCryptoIvVector;
    if (thatPathData.isEmpty())                 isArgEmpty = true;
    if (thatWeBookCatFilePathName.isEmpty())    isArgEmpty = true;

    if (isArgEmpty)
    {
        QString myDataPath;

        if (thatWeBookCatFilePathName.isEmpty())
        {
            QString thatDataPath;
            if (!thatPathData.isEmpty())
            {
                thatDataPath = thatPathData;
            }
            if (thatDataPath.isNull() || thatDataPath.isEmpty())
            {
                // constAppFolder constDataFolderName
                thatDataPath = findFilePath("", constDataFolderName);
            }
            QString thatIniPath = findFilePath(ConstIniFileNameFullPath, thatDataPath);
            if (!thatIniPath.isNull() && !thatIniPath.isEmpty())
            {
                thatWeBookCatFilePathName = thatIniPath;
            }
            else
            {
                // error FIXME
                thatWeBookCatFilePathName = ConstIniFileNameFullPath;
            }
        }


        // organizationName, organizationDomain, applicationName and applicationName
        // are set in main.cpp, and passed into Constuctor, so they are set
        QCoreApplication::setOrganizationName(thatOrgName);
        QCoreApplication::setOrganizationDomain(thatOrgDomain);
        QCoreApplication::setApplicationName(applicationName);
        // see *.pro file where it is: DEFINE  S     *= APP_VERSION=$${VERSION}
        // QCoreApplication::setApplicationVersion(QObject::tr(APP_VERSION));
        // dataFullPath is set in setDataPathToIniFile() called in constructor
        //QSettings *weBookSettings  = new QSettings(myDataPath, QSettings::IniFormat);

    }

    setAppName(thatAppName);
    setIniFileName(thatInitFile);
    setOrgName(thatOrgName);
    setOrgDomain(thatOrgDomain);
    setCryptoKey(thatCrypoKey);
    setCryptoIvVector(thatCryptoIv);



    QString thatWeBookList;

    if (QFile(thatWeBookCatFilePathName).exists())
    {
        QFile inputFile(thatWeBookCatFilePathName);
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
    const auto port = httpServer->listen(QHostAddress::Any, 9696);
    if (!port)
    {
        qDebug() << QCoreApplication::translate("QHttpServerExample", "Server failed to listen on a port.");
    }
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
void WeBookServer::setOrgName(const QString &myValue)
{
    if (!myOrganizationName.isNull() && !myOrganizationName.isEmpty() && myOrganizationName != myValue)
    {
        // changing it
        myOrganizationName = myValue;
    }
    else
    {
        // Setting it for first time
        myOrganizationName = myValue;
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
void WeBookServer::setOrgDomain(const QString &myValue)
{
    if (!myOrganizationDomain.isNull() && !myOrganizationDomain.isEmpty() && myOrganizationDomain != myValue)
    {
        // changing it
        myOrganizationDomain = myValue;
    }
    else
    {
        // Setting it for first time
        myOrganizationDomain = myValue;
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
void WeBookServer::setIniFileName(const QString &myValue)
{
    if (!myIniFileName.isNull() && !myIniFileName.isEmpty() && myIniFileName != myValue)
    {
        // changing it
        myIniFileName = myValue;
    }
    else
    {
        // Setting it for first time
        myIniFileName = myValue;
    }
} // end setIniFileName
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
void WeBookServer::setCryptoKey(const QString &myValue)
{
    if (!myCryptoKey.isNull() && !myCryptoKey.isEmpty() && myCryptoKey != myValue)
    {
        // changing it
        myCryptoKey = myValue;
    }
    else
    {
        // Setting it for first time
        myCryptoKey = myValue;
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
void WeBookServer::setCryptoIvVector(const QString &myValue)
{
    if (!myCryptoIvVector.isNull() && !myCryptoIvVector.isEmpty() && myCryptoIvVector != myValue)
    {
        // changing it
        myCryptoIvVector = myValue;
    }
    else
    {
        // Setting it for first time
        myCryptoIvVector = myValue;
    }
} // end setCryptoIvVector
/******************************************************************************
** findFilePath(String thisFileName, QString thisDataFolderName)              *
** All files must be in folder constAppFolder                                 *
** This folder is different for debug vs release as well as deployed          *
*******************************************************************************/
QString WeBookServer::findFilePath(QString thisFileName, QString thisDataFolderName)
{
    // Make sure Build Folder does not contain the App Name, this is default, change Projects Build Path
    if (constAppDataLocation.contains(constAppFolder))
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
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4%5").arg(QDir::separator()).arg(constAppFolder).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5%6").arg(QDir::separator()).arg(QDir::separator()).arg(constAppFolder).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        if (!QDir(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2..%3%4%5%6%7").arg(QDir::separator()).arg(QDir::separator()).arg(QDir::separator()).arg(constAppFolder).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        // look back before the constAppFolder
        if (!QDir(dataFullPath).exists())
        {
            dataFileDir = QDir(QCoreApplication::applicationDirPath());
            // ../thisDataFolderName/
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3").arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator())));
        }
        // look back before the constAppFolder
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
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4%5%6").arg(QDir::separator()).arg(constAppFolder).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2%3%4%5%6%7").arg(QDir::separator()).arg(QDir::separator()).arg(constAppFolder).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        if (!QFile(dataFullPath).exists())
        {
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1..%2..%3%4%5%6%7%8").arg(QDir::separator()).arg(QDir::separator()).arg(QDir::separator()).arg(constAppFolder).arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        // look back before the constAppFolder
        if (!QFile(dataFullPath).exists())
        {
            dataFileDir = QDir(QCoreApplication::applicationDirPath());
            // ../thisDataFolderName/thisFileName
            dataFullPath = dataFileDir.cleanPath(dataFileDir.absoluteFilePath(QString("..%1%2%3%4").arg(QDir::separator()).arg(thisDataFolderName).arg(QDir::separator()).arg(thisFileName)));
        }
        // look back before the constAppFolder
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
