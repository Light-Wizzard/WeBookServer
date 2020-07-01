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
        // myCatFileName = findFilePath(qLoggerCommon->getFilelPath(), QLogger::ConstDefaultWeBookCatName);
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
#ifdef THIS
/******************************************************************************
*  findFilePath(String thisFileName, QString thisDataFolderName)              *
*  All files must be in folder myAppFolderName                                *
*  This folder is different for debug vs release as well as deployed          *
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
#endif
/******************************  End of File *********************************/
