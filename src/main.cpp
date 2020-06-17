/******************************************************************************
*  WeBookServer                                                               *
*  WeBook is Pronounced Web-Book, it is a Web-Book Content Management System  *
*******************************************************************************/
#include "WeBookServer.h"
/******************************************************************************
*  main                                                                       *
*******************************************************************************/
int main(int argc, char *argv[])
{
    // WeBook Common has QtSettings and Crypto Functions Common between Client/Server
    QLogger::QLoggerCommon *qLoggerCommon = new QLogger::QLoggerCommon(false);
    qLoggerCommon->setLogLevel(QLoggerLevel::LogLevel::Fatal);
    QLogger::QLoggerCrypto *weBookCrypto = new QLogger::QLoggerCrypto();

    QString applicationName;
    // From *.pro file TARGET   = WeBook, maybe getTarget?
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({{"a", "appname"},     "Application Name if you want it different then executable name.", "appname"});
    parser.addOption({{"i", "inifile"},     "The Ini File Name: settings.ini", "inifile"});
    parser.addOption({{"n", "orgname"},     "The Organization Name going to be the Window Title", "orgname"});
    parser.addOption({{"d", "orgdomain"},   "The Organization Domain URL: https://github.com/USERNAME/PROJECT", "orgdomain"});
    parser.addOption({{"k", "key"},         "The Crypto Key String", "key"});
    parser.addOption({{"c", "cryptoiv"},    "The Crypto IV Vector String", "cryptoiv"});
    parser.addOption({{"w", "webook"},      "The WeBook.cat file name and full path: /path/WeBooks.cat", "webook"});
    parser.addOption({{"f", "filepath"},    "The Folder to the Data", "filepath"});
    parser.addOption({{"p", "port"},        "The Port Number of Server", "port"});
    parser.addOption({{"l", "logpath"},     "The Full Path to Log Fils: /path", "logpath"});
    parser.addOption({{"b", "blogfolder"},  "The Log Folder Name: logs", "blogfolder"});


    const QStringList positionalArguments = parser.positionalArguments();
    if (!positionalArguments.isEmpty())
    {
        parser.process(positionalArguments);
    }


    // QStringList
    applicationName = argv[0];
    if (applicationName.isEmpty()) applicationName    = qLoggerCommon->getAppName();

    QString thatAppName                 = parser.value("appname");
    QString thatInitFile                = parser.value("inifile");
    QString thatOrgName                 = parser.value("orgname");
    QString thatOrgDomain               = parser.value("orgdomain");
    QString thatCrypoKey                = parser.value("key");
    QString thatCryptoIv                = parser.value("cryptoiv");
    QString thatFolderData              = parser.value("filepath");
    QString thatWeBookCatFilePathName   = parser.value("webook");
    QString thatPort                    = parser.value("port");
    QString thatLogPath                 = parser.value("logpath");
    QString thatLogFolder               = parser.value("blogfolder");
    //
    bool isArgEmpty = false;
    if (thatFolderData.isEmpty())                isArgEmpty = true;
    if (thatWeBookCatFilePathName.isEmpty())     isArgEmpty = true;

    if (thatAppName.isEmpty())      thatAppName      = applicationName;
    if (thatInitFile.isEmpty())     thatInitFile     = qLoggerCommon->getIniFileName();
    if (thatOrgName.isEmpty())      thatOrgName      = qLoggerCommon->getOrgName();
    if (thatOrgDomain.isEmpty())    thatOrgDomain    = qLoggerCommon->getOrgDomain();
    if (thatPort.isEmpty())         thatPort         = qLoggerCommon->portToString();
    if (thatLogPath.isEmpty())      thatLogPath      = qLoggerCommon->getLogPath();
    if (thatFolderData.isEmpty())   thatFolderData   = qLoggerCommon->getFilelPath();
    if (thatLogFolder.isEmpty())    thatLogFolder    = qLoggerCommon->getLogFolderName();
    //
    if (thatCrypoKey.isEmpty())     thatCrypoKey     = weBookCrypto->getCryptoKey();
    if (thatCryptoIv.isEmpty())     thatCryptoIv     = weBookCrypto->getCryptoIvVector();

    // organizationName, organizationDomain, applicationName and applicationName
    // are set in main.cpp, and passed into Constuctor, so they are set
    QCoreApplication::setOrganizationName(thatOrgName);
    QCoreApplication::setOrganizationDomain(thatOrgDomain);
    QCoreApplication::setApplicationName(applicationName);

    qLoggerCommon->setAppName(thatAppName);
    qLoggerCommon->setIniFileName(thatInitFile);
    qLoggerCommon->setOrgName(thatOrgName);
    qLoggerCommon->setOrgDomain(thatOrgDomain);
    qLoggerCommon->setPort(thatPort.toInt());
    qLoggerCommon->setLogPath(thatLogPath);
    qLoggerCommon->setFilePath(thatFolderData);
    qLoggerCommon->setLogFolderName(thatLogFolder);
    qLoggerCommon->setLogger();
    //
    weBookCrypto->setCryptoKey(thatCrypoKey);
    weBookCrypto->setCryptoIvVector(thatCryptoIv);


    WeBookServer service(argc, argv);

    if (isArgEmpty)
    {
        QString myDataPath;
        if (thatWeBookCatFilePathName.isEmpty())
        {
            QString thatDataPath;
            if (!thatFolderData.isEmpty())
            {
                thatDataPath = thatFolderData;
            }
            if (thatDataPath.isNull() || thatDataPath.isEmpty())
            {
                // constAppFolder constDataFolderName
                thatDataPath = service.findFilePath(QLogger::ConstDefaultWeBookCatName, QLogger::ConstDefaultFileFolderName);
            }
            QString thatCatPath = service.findFilePath(QLogger::ConstDefaultWeBookCatName, thatDataPath);
            if (!thatCatPath.isNull() && !thatCatPath.isEmpty())
            {
                thatWeBookCatFilePathName = thatCatPath;
            }
            else
            {
                // error FIXME
                thatWeBookCatFilePathName = thatFolderData;
            }
        }
    }
    service.setCatFileName(thatWeBookCatFilePathName);


    service.exec();


} // end main
/******************************  End of File *********************************/
