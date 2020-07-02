#include "WeBookServer.h"
#include "WeBookServer.h"
/******************************************************************************
*  WeBookServer Constructor                                                   *
*******************************************************************************/
/*!
   \class WeBookServer
   \fn WeBookServer::WeBookServer(int &argc, char **argv) : Service(argc, argv)
   \brief Unix Daemon/Windows Service and HTTP Server.
 */
WeBookServer::WeBookServer(int &argc, char **argv) : Service(argc, argv)
{

    // WeBook Common has QtSettings and Crypto Functions Common between Client/Server
    weBookSettings = new WeBookSettings(this);
    weBookCrypto   = new WeBookCrypto(this);

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
    if (applicationName.isEmpty()) applicationName    = weBookSettings->getAppName();
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
    if (thatInitFile.isEmpty())     thatInitFile     = weBookSettings->getIniFileName();
    if (thatOrgName.isEmpty())      thatOrgName      = weBookSettings->getOrgName();
    if (thatOrgDomain.isEmpty())    thatOrgDomain    = weBookSettings->getOrgDomain();
    if (thatPort.isEmpty())         thatPort         = weBookSettings->portToString();
    if (thatLogPath.isEmpty())      thatLogPath      = weBookSettings->getLogPath();
    if (thatFolderData.isEmpty())   thatFolderData   = weBookSettings->getFilelPath();
    if (thatLogFolder.isEmpty())    thatLogFolder    = weBookSettings->getLogFolderName();
    //
    if (thatCrypoKey.isEmpty())     thatCrypoKey     = weBookCrypto->getCryptoKey();
    if (thatCryptoIv.isEmpty())     thatCryptoIv     = weBookCrypto->getCryptoIvVector();
    // organizationName, organizationDomain, applicationName and applicationName
    // are set in main.cpp, and passed into Constuctor, so they are set
    QCoreApplication::setOrganizationName(thatOrgName);
    QCoreApplication::setOrganizationDomain(thatOrgDomain);
    QCoreApplication::setApplicationName(applicationName);

    weBookSettings->setAppName(thatAppName);
    weBookSettings->setIniFileName(thatInitFile);
    weBookSettings->setOrgName(thatOrgName);
    weBookSettings->setOrgDomain(thatOrgDomain);
    weBookSettings->setPort(thatPort.toInt());
    weBookSettings->setLogPath(thatLogPath);
    weBookSettings->setFilePath(thatFolderData);
    weBookSettings->setLogFolderName(thatLogFolder);
    weBookCrypto->setCryptoKey(thatCrypoKey);
    weBookCrypto->setCryptoIvVector(thatCryptoIv);
    //
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
                thatDataPath = weBookSettings->ConstDefaultFileFolderName;
            }
            weBookSettings->findJoinFilePath(weBookSettings->ConstDefaultWeBookCatName, thatDataPath);
            QString thatCatPath = weBookSettings->getFilelPath();
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
    QCoreApplication::setApplicationName(thatAppName);
    // FIXME
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

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

} // end WeBookServer
/******************************************************************************
*  getCatFileName                                                             *
*******************************************************************************/
QString WeBookServer::getCatFileName()
{
    if (myCatFileName.isEmpty()) setCatFileName("");
    return myCatFileName;
} // end getCatFileName
/******************************************************************************
*  setIniFileName                                                             *
*******************************************************************************/
void WeBookServer::setCatFileName(const QString &thisCatFileName)
{
    if (thisCatFileName.isEmpty())
    {
        // FIXME
        weBookSettings->findJoinFilePath(weBookSettings->getFilelPath(), weBookSettings->ConstDefaultWeBookCatName);
    }
    if (myCatFileName.isEmpty() || myCatFileName != thisCatFileName)
    {
        // changing it
        myCatFileName = thisCatFileName;
    }
} // end setCatFileName
/******************************************************************************
 \fn bool WeBookServer::preStart()
 \brief PreStart
*******************************************************************************/
bool WeBookServer::preStart()
{
    qDebug() << Q_FUNC_INFO;
    qInfo() << "Service running with backend:" << backend();

    addCallback("SIGUSR1", []()
    {
        qDebug() << "SIGUSR1";
    });
    addCallback("SIGUSR2", []()
    {
        qDebug() << "SIGUSR2";
        return 42;
    });

    return true;
} // end preStart
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onStart()
 \brief onStart
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onStart()
{
    qDebug() << Q_FUNC_INFO;
    _server = new QTcpServer(this);
    connect(_server, &QTcpServer::newConnection, this, &WeBookServer::newConnection);

    auto socket = getSocket();
    auto ok = false;
    if(socket >= 0)
    {
        qDebug() << "Using activated socket descriptor:" << socket;
        ok = _server->setSocketDescriptor(socket);
    }
    else
    {
        qDebug() << "No sockets activated - creating normal socket";
        ok = _server->listen();
    }
    if(ok)
        qInfo() << "Started echo server on port" << _server->serverPort();
    else
    {
        qCritical().noquote() << "Failed to start server with error" << _server->errorString();
        qApp->exit(EXIT_FAILURE);
    }

    return CommandResult::Completed;
} // end onStart
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onStop(int &exitCode)
 \brief onStop
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onStop(int &exitCode)
{
    Q_UNUSED(exitCode)
    qDebug() << Q_FUNC_INFO;
    _server->close();
    return CommandResult::Completed;
} // end onStop
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onReload()
 \brief onReload
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onReload()
{
    qDebug() << Q_FUNC_INFO;
    _server->close();
    if(_server->listen())
        qInfo() << "Restarted echo server on port" << _server->serverPort();
    else
    {
        qCritical().noquote() << "Failed to restart server with error" << _server->errorString();
        qApp->exit(EXIT_FAILURE);
    }

    return CommandResult::Completed;
} // end onReload
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onPause()
 \brief onPause
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onPause()
{
    qDebug() << Q_FUNC_INFO;
    _server->pauseAccepting();
    return CommandResult::Completed;
} // end onPause
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onResume()
 \brief onResume
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onResume()
{
    qDebug() << Q_FUNC_INFO;
    _server->resumeAccepting();
    return CommandResult::Completed;
} // end onResume
/******************************************************************************
 \fn void WeBookServer::newConnection()
 \brief newConnection
*******************************************************************************/
void WeBookServer::newConnection()
{
    while(_server->hasPendingConnections())
    {
        auto socket = _server->nextPendingConnection();
        socket->setParent(this);
        connect(socket, &QTcpSocket::readyRead, socket, [socket]()
        {
            auto msg = socket->readAll();
            qDebug() << host(socket) << "Echoing:" << msg;
            socket->write(msg);
        });
        connect(socket, &QTcpSocket::disconnected, socket, [socket]()
        {
            qInfo() << host(socket) << "disconnected";
            socket->close();
            socket->deleteLater();
        });
        // FIXME 'error' is deprecated: Use QAbstractSocket::errorOccurred(QAbstractSocket::SocketError) instead
        // I do not see that this is deprecated but instead causes error
        // QAbstractSocket::error gives the same warning
        // https://doc.qt.io/qt-5/qabstractsocket.html#error
        connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), socket, [socket](QAbstractSocket::SocketError error)
        {
            qWarning() << host(socket) << "Socket-Error[" << error << "]:" << qUtf8Printable(socket->errorString());
        });
        qInfo() << host(socket) << "connected";
    }
} // end newConnection
/******************************************************************************
 \fn QByteArray WeBookServer::host(QTcpSocket *socket)
 \brief host
*******************************************************************************/
QByteArray WeBookServer::host(QTcpSocket *socket)
{
    return (QLatin1Char('<') + socket->peerAddress().toString() + QLatin1Char(':') + QString::number(socket->peerPort()) + QLatin1Char('>')).toUtf8();
} // end host
/******************************  End of File *********************************/
