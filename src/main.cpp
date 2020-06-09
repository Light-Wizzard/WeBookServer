/******************************************************************************
** WeBookServer                                                               *
** WeBook is Pronounced Web-Book, it is a Web-Book Content Management System  *
*******************************************************************************/
#include "webookserver.h"
static bool isLogToFile = false;
/******************************************************************************
** WeBookMessenger                                                            *
** This uses Qt qInstallMessageHandler(WeBookMessenger);                      *
** I have no idea if this gets called in a Thread, but Qt should handle it,   *
** ensuring thread safe way to open and write to a log file.                  *
** Note: I close the file in mainEventHandler.                                *
*******************************************************************************/
void WeBookMessenger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QHash<QtMsgType, QString> msgLevelHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"}, {QtFatalMsg, "Fatal"}});
    QString txt = QString("%1 %2: %3 (%4:%5, %6)").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(msgLevelHash[type]).arg(msg).arg(context.file).arg(context.line).arg(context.function);
    if (isLogToFile)
    {

        RollingFileAppender *rollfile = new RollingFileAppender(QString("%1.log").arg(myLogPathFileName));
        //roll every minute
        rollfile->setDatePattern(RollingFileAppender::MonthlyRollover);
        // logfile been retained
        rollfile->setLogFilesLimit(0); // I think 0 or 1 will not delete logs
        cuteLogger->registerCategoryAppender("WeBookServerID", rollfile);
        // LOG_CERROR("WeBookServerID") << "roll every month and retain all files";
        LOG_INFO() << txt;
//        if (!myLogFileHandle.isOpen())
//        {
//            myLogFileHandle.setFileName(myLogPathFileName);
//            myLogFileHandle.open(QIODevice::WriteOnly | QIODevice::Append);
//        }
//        QTextStream ts(&myLogFileHandle);
//        ts << txt << endl;
    }
    else
    {
        QByteArray formattedMessage = txt.toLocal8Bit();
        fprintf(stderr, "%s\n", formattedMessage.constData());
        fflush(stderr);
    }
    if (type == QtFatalMsg) abort();
} // end WeBookMessenger
/******************************************************************************
** main                                                                       *
*******************************************************************************/
int main(int argc, char *argv[])
{
    const QString constAppFolder             = "WeBookServer";     // Name of Folder this Project is in
    const QString constDataFolderName        = "data";             // Name of data Folder
    const QString ConstAppName               = "WeBookServer";  // This is the Name of the GitHub Project, but does not have to be
    // I do not like hard coding variables into Applications
    // Const Ini File Name Full Path if want to change the Path to the ini or rename it, leaving it just the name, will look for it in the data folder
    const QString ConstIniFileNameFullPath   = "WeBook.ini";                                // normally WeBook.ini, but I make it an option, see above comment
    // These next 3 settings will set the Local Data Storage for this App to use, changing it after it has run, will create a new storage containtainer
    const QString ConstOrganizationName      = "Light Wizzard";                             // This is the User Name for GitHub account, but does not have to be
    const QString ConstOrganizationDomain    = "https://github.com/Light-Wizzard/WeBook";   // This is the User Name and Project or GitHub URL, but does not have to be
    // I hard code these values here for a reason, not that I would use them, set the Link to include this data, pass in as Arguments
    // These values are for testing only
    const QString ConstDefaultCryptoKey      = "!1@2#3$4%5^6&7*8)9(0)NowPickRand";          // This is used as a Key for Password Encryption/Decryption
    const QString ConstDefaultCryptoIvVector = "PutMoveVector1!2@3#4$NowPickRand";          // This is used as the IV Vector for Password Encryption/Decryption
    const QString ConstDefaultPort           = "9696";             // Port Number of WeBookServer

    QString applicationName;
    // From *.pro file TARGET   = WeBook, maybe getTarget?
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({{"a", "appname"},     "Application Name if you want it different then executable name.", "appname"});
    parser.addOption({{"i", "ini"},         "The Ini File Name: settings.ini", "inifile"});
    parser.addOption({{"n", "name"},        "The Organization Name going to be the Window Title", "orgname"});
    parser.addOption({{"d", "domain"},      "The Organization Domain URL: https://github.com/USERNAME/PROJECT", "orgdomain"});
    parser.addOption({{"k", "key"},         "The Crypto Key String", "key"});
    parser.addOption({{"c", "cryptoiv"},    "The Crypto IV Vector String", "cryptoiv"});
    parser.addOption({{"w", "webook"},      "The WeBook.cat file name and full path: /path/WeBooks.cat", "webook"});
    parser.addOption({{"f", "folderdata"},  "The Folder to the Data", "folderdata"});
    parser.addOption({{"p", "port"},        "The Port Number of Server", "port"});

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
    QString thatFolderData              = parser.value("folderdata");
    QString thatWeBookCatFilePathName   = parser.value("webook");
    QString thatPort                    = parser.value("port");
    //
    bool isArgEmpty = false;
    if (thatAppName.isEmpty())   thatAppName   = applicationName;
    if (thatInitFile.isEmpty())  thatInitFile  = ConstIniFileNameFullPath;
    if (thatOrgName.isEmpty())   thatOrgName   = ConstOrganizationName;
    if (thatOrgDomain.isEmpty()) thatOrgDomain = ConstOrganizationDomain;
    if (thatCrypoKey.isEmpty())  thatCrypoKey  = ConstDefaultCryptoKey;
    if (thatCryptoIv.isEmpty())  thatCryptoIv  = ConstDefaultCryptoIvVector;
    if (thatPort.isEmpty())      thatPort      = ConstDefaultPort;
    if (thatFolderData.isEmpty())                isArgEmpty = true;
    if (thatWeBookCatFilePathName.isEmpty())     isArgEmpty = true;
    // organizationName, organizationDomain, applicationName and applicationName
    // are set in main.cpp, and passed into Constuctor, so they are set
    QCoreApplication::setOrganizationName(thatOrgName);
    QCoreApplication::setOrganizationDomain(thatOrgDomain);
    QCoreApplication::setApplicationName(applicationName);

    isLogToFile = true;
    myLogPathFileName = QString("%1%2.log").arg(applicationName).arg(QDateTime::currentDateTime().toString("-Log-yyyy-MMMM-d-hh-mm-ss-zzz"));

    WeBookServer service(argc, argv);
    service.setAppName(thatAppName);
    service.setIniFileName(thatInitFile);
    service.setOrgName(thatOrgName);
    service.setOrgDomain(thatOrgDomain);
    service.setCryptoKey(thatCrypoKey);
    service.setCryptoIvVector(thatCryptoIv);
    service.setAppFolderName(constAppFolder);
    service.setPort(thatPort.toInt());
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
                thatDataPath = service.findFilePath("", constDataFolderName);
            }
            QString thatIniPath = service.findFilePath(ConstIniFileNameFullPath, thatDataPath);
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
        // see *.pro file where it is: DEFINE  S     *= APP_VERSION=$${VERSION}
        // QCoreApplication::setApplicationVersion(QObject::tr(APP_VERSION));
        // dataFullPath is set in setDataPathToIniFile() called in constructor
        //QSettings *weBookSettings  = new QSettings(myDataPath, QSettings::IniFormat);
    }
    service.setCatFileName(thatWeBookCatFilePathName);

    service.exec();

    qInstallMessageHandler(WeBookMessenger); // Install the Message handler

} // end main
/******************************* End of File *********************************/
