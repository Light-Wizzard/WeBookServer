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


    tcpSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);
    fileServer = new QTcpServer(this);
    fileServer->listen(QHostAddress::Any, 8888);
    connect(fileServer, &QTcpServer::newConnection, this, &WeBookServer::onAcceptFileConnection);


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
    tcpServer = new QTcpServer(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &WeBookServer::newConnection);

    auto socket = getSocket();
    auto ok = false;
    if(socket >= 0)
    {
        qDebug() << "Using activated socket descriptor:" << socket;
        ok = tcpServer->setSocketDescriptor(socket);
    }
    else
    {
        qDebug() << "No sockets activated - creating normal socket";
        ok = tcpServer->listen(QHostAddress::Any, 6666);
    }
    if(ok)
    {
        qInfo() << "Started echo server on port" << tcpServer->serverPort();
    }
    else
    {
        qCritical().noquote() << "Failed to start server with error" << tcpServer->errorString();
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

    if(tcpServer->isListening())
    {
        disconnect(tcpServer, &QTcpServer::newConnection, this, &WeBookServer::newConnection);

        QList<QTcpSocket *> clients = getClients();
        for(int i = 0; i < clients.count(); i++)
        {
            sendToClient(clients.at(i), "Connection closed");
            //sendToClient(clients.at(i), "0");
        }
        tcpServer->close();
    }
    return CommandResult::Completed;
} // end onStop
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onReload()
 \brief onReload
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onReload()
{
    qDebug() << Q_FUNC_INFO;
    tcpServer->close();
    if(tcpServer->listen())
    {
        qInfo() << "Restarted echo server on port" << tcpServer->serverPort();
    }
    else
    {
        qCritical().noquote() << "Failed to restart server with error" << tcpServer->errorString();
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
    tcpServer->pauseAccepting();
    return CommandResult::Completed;
} // end onPause
/******************************************************************************
 \fn QtService::Service::CommandResult WeBookServer::onResume()
 \brief onResume
*******************************************************************************/
QtService::Service::CommandResult WeBookServer::onResume()
{
    qDebug() << Q_FUNC_INFO;
    tcpServer->resumeAccepting();
    return CommandResult::Completed;
} // end onResume
/******************************************************************************
 \fn void WeBookServer::newConnection()
 \brief newConnection
*******************************************************************************/
void WeBookServer::newConnection()
{
    while(tcpServer->hasPendingConnections())
    {
        auto socket = tcpServer->nextPendingConnection();
        socket->setParent(this);
        connect(socket, &QTcpSocket::readyRead,    this,   &WeBookServer::onReceiveData);
        connect(socket, &QTcpSocket::disconnected, socket, [socket]()
        {
            qInfo() << host(socket) << "disconnected";
            socket->close();
            socket->deleteLater();
        });
        connect(socket, &QAbstractSocket::errorOccurred, this, &WeBookServer::onDisplayError);
        qInfo() << host(socket) << "connected";
        connect(socket, &QTcpSocket::disconnected, this,   &WeBookServer::onGotDisconnection);
        clients << socket;
        sendToClient(socket, "Reply: connection established");
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
/**************************************************************************
* getClients
* \fn QList<QTcpSocket *> WeBookServer::getClients()
***************************************************************************/
QList<QTcpSocket *> WeBookServer::getClients()
{
    return clients;
} // end getClients
/**************************************************************************
* sendToClient
* \fn qint64 WeBookServer::sendToClient(QTcpSocket *socket, const QString &str)
***************************************************************************/
qint64 WeBookServer::sendToClient(QTcpSocket *socket, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    //out << quint16(0) << QTime::currentTime() << str;
    out << quint16(0) << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    return socket->write(arrBlock);
} // end sendToClient
/**************************************************************************
* onGotDisconnection
* \fn void WeBookServer::onGotDisconnection()
***************************************************************************/
void WeBookServer::onGotDisconnection()
{
    clients.removeAt(clients.indexOf((QTcpSocket*)sender()));
} // end onGotDisconnection
/**************************************************************************
* onAcceptFileConnection
* \fn void Server::onAcceptFileConnection()
***************************************************************************/
void WeBookServer::onAcceptFileConnection()
{
    bytesWritten = 0;
    fileSocket = fileServer->nextPendingConnection();
    connect(fileSocket, &QTcpSocket::readyRead, this, &WeBookServer::onUpdateFileProgress);
    connect(fileSocket, &QTcpSocket::disconnected, fileSocket, &QTcpSocket::deleteLater);
    connect(fileSocket, &QAbstractSocket::errorOccurred, this, &WeBookServer::onUpdateFileProgressBytes);
    connect(fileSocket, &QTcpSocket::bytesWritten, this, &WeBookServer::onUpdateFileProgressBytes);
} // end onAcceptFileConnection
/**************************************************************************
* onReceiveData
* \fn void Server::onReceiveData()
***************************************************************************/
void WeBookServer::onReceiveData()
{
    tcpSocket = (QTcpSocket*)sender();

    QString str = tcpSocket->readAll();
    qDebug() << "Recieved " << str;
    //ui->textEditMessages->append(QString("Client %1 %2 \n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), str));
    if (sendToClient(tcpSocket, QString("Reply: received [%1]").arg(str)) == -1)
    {
        qDebug() << "Some error occured";
    }
} // end onReceiveData
/**************************************************************************
* onUpdateFileProgress
* \fn void Server::onUpdateFileProgress()
***************************************************************************/
void WeBookServer::onUpdateFileProgress()
{
    QDataStream inFile(fileSocket);
    // Qt_4_9 Qt_5_0 Qt_5_15
    inFile.setVersion(QDataStream::Qt_4_8);

    if (bytesReceived <= (qint64)sizeof(qint64) * 2)
    {
        if ((fileSocket->bytesAvailable() >= ((qint64)sizeof(qint64)) * 2) && (fileNameSize == 0))
        {
            inFile >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if ((fileSocket->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
        {
            inFile >> fileName;
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "Server::open file error!";
                return;
            }
        }
        else
        {
            return;
        }
    }
    if (bytesReceived < totalBytes)
    {
        bytesReceived += fileSocket->bytesAvailable();
        inBlock = fileSocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    //ui->progressBarFile->setMaximum(totalBytes);
    //ui->progressBarFile->setValue(bytesReceived);
    qDebug() << "Receiving file " << bytesReceived << " of " << totalBytes;
    if (bytesReceived == totalBytes)
    {
        qDebug() << "Receive file successfully!";
        //ui->textEditMessages->append("Receive file successfully!");
        bytesReceived = 0;
        totalBytes    = 0;
        fileNameSize  = 0;
        localFile->close();
        //filesocket->close();
        tcpSocket = (QTcpSocket*)sender();
        sendToClient(tcpSocket, "Reply: received file");
    }
} // end onUpdateFileProgress
/**************************************************************************
* onUpdateFileProgressBytes
* \fn void Server::onUpdateFileProgressBytes(qint64 numBytes)
***************************************************************************/
void WeBookServer::onUpdateFileProgressBytes(qint64 numBytes)
{
    bytesWritten += (int)numBytes;

    if (bytesToWrite > 0)
    {
        outBlock = localFile->read(qMin(bytesToWrite, perDataSize));
        bytesToWrite -= ((int)fileSocket->write(outBlock));
        outBlock.resize(0);
    }
    else
    {
        localFile->close();
    }

    if (bytesWritten == totalBytes)
    {
        localFile->close();
        //filesocket->close();
        tcpSocket = (QTcpSocket*)sender();
        sendToClient(tcpSocket, "Reply: received file");
    }
} // end onUpdateFileProgressBytes
/**************************************************************************
* onDisplayError
* \fn void Server::onDisplayError(QAbstractSocket::SocketError socketError)
***************************************************************************/
void WeBookServer::onDisplayError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    tcpSocket = (QTcpSocket*)sender();
    qDebug() << tcpSocket->errorString();
    tcpSocket->close();
} // end onDisplayError
/**************************************************************************
* onSendFile
* \fn void Server::onSendFile()
***************************************************************************/
void WeBookServer::onSendFile()
{
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly))
    {
        return;
    }
    totalBytes = localFile->size();
    QDataStream sendout(&outBlock, QIODevice::WriteOnly);
    sendout.setVersion(QDataStream::Qt_4_8);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);

    sendout << qint64(0) << qint64(0) << currentFileName;
    totalBytes += outBlock.size();
    sendout.device()->seek(0);
    sendout << totalBytes << qint64((outBlock.size() - sizeof(qint64)* 2));

    bytesToWrite = totalBytes - fileSocket->write(outBlock);
    outBlock.resize(0);
} // end onSendFile
/******************************  End of File *********************************/
