# WeBookServer.pro
# Specifies the name of the template to use when generating the project.
# The allowed values are: app, lib, subdirs, aux, vcapp or vclib
TEMPLATE     = "app"
# Specifies the name of the target file. Contains the base name of the project file by default.
# Do not use Spaces in Target Name
# This pro(ject) file is based on a pattern
TARGET       = "WeBookServer"
#
INCLUDEPATH     += .
INCLUDEPATH     += src/
INCLUDEPATH     += "${QT_INSTALL_LIBS}/qt"
#
DEPENDPATH      += src/

#
QT              -= gui
QT              *= core
QT              *= core-private
#!win32:QT       *= network
QT              *= network

CONFIG          *= qt
CONFIG          *= console
CONFIG          *= cmdline
CONFIG          -= app_bundle
CONFIG          *= "c++11"
#CONFIG          *= "c++14"
#CONFIG         *= "c++1z"
#CONFIG          *= "c++17"
#CONFIG         *= "c++2a"
#CONFIG         *= "c++latest"
#
HEADERS         += src/WeBookServer.h
SOURCES         += src/WeBookServer.cpp src/main.cpp

unix:DISTFILES  += tools/haproxy.cfg tools/monit.conf tools/webookserver.ini tools/webookserver.sh tools/webooksetup.sh
DISTFILES       += README.md
DISTFILES       += WeBookServer.qdocconf
DISTFILES       += data/WeBook.toc
DISTFILES       += data/WeBook/ItemId1.1.html
DISTFILES       += data/WeBook/TitlePageID.html
DISTFILES       += data/WeBookClient.ini
DISTFILES       += data/WeBooks.cat
TRANSLATIONS    += src/WeBookServer_en_US.ts

win32:VERSION    = 0.1.0.0 # major.minor.patch.build
else:VERSION     = 0.1.0   # major.minor.patch
DEFINES          = APP_VERSION=\\\"$${VERSION}\\\"
win32:LIBS      += -luser32
###############################################################################
# Forked: https://github.com/qt-labs/qthttpserver                             #
# https://github.com/Light-Wizzard/qthttpserver                               #
# FIXME Setup as a proper subdir
###############################################################################
INCLUDEPATH     *= src/QHttpServer src/QHttpServer/httpserver src/QHttpServer/sslserver
DEPENDSPATH     *= src/QHttpServer src/QHttpServer/httpserver src/QHttpServer/sslserver
#
#
HEADERS         += src/QHttpServer/httpserver/qabstracthttpserver.h
HEADERS         += src/QHttpServer/httpserver/qabstracthttpserver_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserver.h
HEADERS         += src/QHttpServer/httpserver/qhttpserver_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverliterals_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrequest.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrequest_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverresponder.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverresponder_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverresponse.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverresponse_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrouter.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrouter_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrouterrule.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrouterrule_p.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverrouterviewtraits.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverviewtraits.h
HEADERS         += src/QHttpServer/httpserver/qhttpserverviewtraits_impl.h
HEADERS         += src/QHttpServer/httpserver/qthttpserverglobal.h
HEADERS         += src/QHttpServer/sslserver/qsslserver.h
HEADERS         += src/QHttpServer/sslserver/qsslserver_p.h
# Private
HEADERS         += src/QHttpServer/httpserver/private/qabstracthttpserver_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserver_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserverliterals_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserverrequest_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserverresponder_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserverresponse_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserverrouter_p.h
HEADERS         += src/QHttpServer/httpserver/private/qhttpserverrouterrule_p.h
HEADERS         += src/QHttpServer/sslserver/private/qsslserver_p.h
#
SOURCES         += src/QtService/qtservice.cpp
SOURCES         += src/QHttpServer/httpserver/qabstracthttpserver.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserver.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserverliterals.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserverrequest.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserverresponder.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserverresponse.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserverrouter.cpp
SOURCES         += src/QHttpServer/httpserver/qhttpserverrouterrule.cpp
SOURCES         += src/QHttpServer/sslserver/qsslserver.cpp
include(src/QHttpServer/3rdparty/http-parser.pri)
#include(include/QtHttpServer/headers.pri)
#include(include/QtSslServer/headers.pri)
#INCLUDEPATH     += include/
#INCLUDEPATH     += include/QtHttpServer
#INCLUDEPATH     += include/QtSslServer
#INCLUDEPATH     *= include/QtHttpServer/5.12.0/QtHttpServer
#INCLUDEPATH     *= include/QtHttpServer/5.12.0/QtHttpServer/private
#INCLUDEPATH     *= include/QtSslServer/5.12.0/QtSslServer
#INCLUDEPATH     *= include/QtSslServer/5.12.0/QtSslServer/private
# This is for Travis
#INCLUDEPATH     += "${QT_INSTALL_LIBS}/qt/QtHttpServer"
#INCLUDEPATH     += "${QT_INSTALL_LIBS}/qt/QtSslServer"
#LIBS            += -L"3rdparty/qthttpserver/lib" -lQt5HttpServer
#SUBDIRS         += 3rdparty/qthttpserver/qthttpserver.pro
#SUBDIRS         += 3rdparty/qthttpserver/src/httpserver/httpserver.pro
#SUBDIRS         += 3rdparty/qthttpserver/src/src.pro
#SUBDIRS         += 3rdparty/qthttpserver/src/sslserver/sslserver.pro
#include(3rdparty/qthttpserver/qthttpserver.pro)
#include(3rdparty/qthttpserver/src/3rdparty/http-parser.pri)
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/auto.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/cmake/cmake.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/qabstracthttpserver/qabstracthttpserver.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/qhttpserver/qhttpserver.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/qhttpserverresponder/qhttpserverresponder.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/qhttpserverresponse/qhttpserverresponse.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/auto/qhttpserverrouter/qhttpserverrouter.pro
#SUBDIRS += 3rdparty/qthttpserver/tests/tests.pro
#SUBDIRS += 3rdparty/qthttpserver/examples/examples.pro
#SUBDIRS += 3rdparty/qthttpserver/examples/httpserver/afterrequest/afterrequest.pro
#SUBDIRS += 3rdparty/qthttpserver/examples/httpserver/httpserver.pro
#SUBDIRS += 3rdparty/qthttpserver/examples/httpserver/simple/simple.pro
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qabstracthttpserver_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserver_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserverliterals_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserverrequest_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserverresponder_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserverresponse_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserverrouter_p.h
#HEADERS         += include/QtHttpServer/5.12.0/QtHttpServer/private/qhttpserverrouterrule_p.h
#HEADERS         += include/QtHttpServer/QAbstractHttpServer
#HEADERS         += include/QtHttpServer/QHttpServer
#HEADERS         += include/QtHttpServer/QHttpServerAfterRequestViewTraits
#HEADERS         += include/QtHttpServer/QHttpServerRequest
#HEADERS         += include/QtHttpServer/QHttpServerResponder
#HEADERS         += include/QtHttpServer/QHttpServerResponse
#HEADERS         += include/QtHttpServer/QHttpServerRouter
#HEADERS         += include/QtHttpServer/QHttpServerRouterRule
#HEADERS         += include/QtHttpServer/QHttpServerRouterViewTraits
#HEADERS         += include/QtHttpServer/QtHttpServer
#HEADERS         += include/QtHttpServer/QtHttpServerDepends
#HEADERS         += include/QtHttpServer/QtHttpServerVersion
#HEADERS         += include/QtHttpServer/qabstracthttpserver.h
#HEADERS         += include/QtHttpServer/qhttpserver.h
#HEADERS         += include/QtHttpServer/qhttpserverrequest.h
#HEADERS         += include/QtHttpServer/qhttpserverresponder.h
#HEADERS         += include/QtHttpServer/qhttpserverresponse.h
#HEADERS         += include/QtHttpServer/qhttpserverrouter.h
#HEADERS         += include/QtHttpServer/qhttpserverrouterrule.h
#HEADERS         += include/QtHttpServer/qhttpserverrouterviewtraits.h
#HEADERS         += include/QtHttpServer/qhttpserverviewtraits.h
#HEADERS         += include/QtHttpServer/qhttpserverviewtraits_impl.h
#HEADERS         += include/QtHttpServer/qthttpserverglobal.h
#HEADERS         += include/QtHttpServer/qthttpserverversion.h
#HEADERS         += include/QtSslServer/5.12.0/QtSslServer/private/qsslserver_p.h
#HEADERS         += include/QtSslServer/QSslServer
#HEADERS         += include/QtSslServer/QtSslServer
#HEADERS         += include/QtSslServer/QtSslServerDepends
#HEADERS         += include/QtSslServer/QtSslServerVersion
#HEADERS         += include/QtSslServer/qsslserver.h
#HEADERS         += include/QtSslServer/qtsslserverglobal.h
#HEADERS         += include/QtSslServer/qtsslserverversion.h
#HEADERS         += src/QHttpServer/sslserver/qtsslserverglobal.h
#INCLUDEPATH     += 3rdparty/qthttpserver/src/3rdparty
#INCLUDEPATH     += 3rdparty/qthttpserver/src/3rdparty/http-parser
#INCLUDEPATH     += 3rdparty/qthttpserver/src/httpserver
#INCLUDEPATH     += 3rdparty/qthttpserver/src/sslserver
#INCLUDEPATH     += 3rdparty/qthttpserver/include
#INCLUDEPATH     += 3rdparty/qthttpserver/include/QtHttpServer
#INCLUDEPATH     += 3rdparty/qthttpserver/include/QtSslServer
#DEPENDPATH      += 3rdparty/qthttpserver/src/3rdparty
#DEPENDPATH      += 3rdparty/qthttpserver/src/3rdparty/http-parser
#DEPENDPATH      += 3rdparty/qthttpserver/src/httpserver
#DEPENDPATH      += 3rdparty/qthttpserver/src/sslserver
#DEPENDPATH      += 3rdparty/qthttpserver/include
#DEPENDPATH      += 3rdparty/qthttpserver/include/QtHttpServer
#DEPENDPATH      += 3rdparty/qthttpserver/include/QtSslServer
###############################################################################
# Forked: https://github.com/qtproject/qt-solutions                           #
# https://github.com/Light-Wizzard/qt-solutions/tree/master/qtservice         #
###############################################################################
INCLUDEPATH     += src/QtService
DEPENDPATH      += src/QtService
HEADERS         += src/QtService/qtservice.h
HEADERS         += src/QtService/qtservice_p.h
# Source Code
win32:SOURCES   += src/QtService/qtservice_win.cpp
unix:HEADERS    += src/QtService/qtunixsocket.h   src/QtService/qtunixserversocket.h
unix:SOURCES    += src/QtService/qtunixsocket.cpp src/QtService/qtunixserversocket.cpp
unix:SOURCES    += src/QtService/qtservice_unix.cpp

win32 {
    qtservice-buildlib:shared:DEFINES += QT_QTSERVICE_EXPORT
    else:qtservice-uselib:DEFINES     += QT_QTSERVICE_IMPORT
}
###############################################################################
# Fork: https://github.com/francescmm/QLogger                                 #
# https://github.com/Light-Wizzard/QLogger                                    #
###############################################################################
HEADERS     *= src/QLogger/QLoggerLevel.h
HEADERS     *= src/QLogger/QLoggerConstants.h
# QLoggerManager
HEADERS     *= src/QLogger/QLoggerManager.h
SOURCES     *= src/QLogger/QLoggerManager.cpp
# QLoggerWriter
HEADERS     *= src/QLogger/QLoggerWriter.h
SOURCES     *= src/QLogger/QLoggerWriter.cpp
# QLoggerCommon
HEADERS     *= src/QLogger/QLoggerCommon.h
SOURCES     *= src/QLogger/QLoggerCommon.cpp
# QLoggerCrypto
HEADERS     *= src/QLogger/QLoggerCrypto.h
SOURCES     *= src/QLogger/QLoggerCrypto.cpp
###############################################################################
# Fork: https://github.com/bricke/Qt-AES                                      #
# https://github.com/Light-Wizzard/Qt-AES                                     #
###############################################################################
INCLUDEPATH *= src/QtAES
DEPENDSPATH *= src/QtAES
#
SOURCES     *= src/QtAES/QAESEncryption.cpp
HEADERS     *= src/QtAES/QAESEncryption.h
# Test
#QT          += testlib
#HEADERS     *= src/QtAES/unit_test/aestest.h
#SOURCES     *= src/QtAES/maintest.cpp
#SOURCES     *= src/QtAES/unit_test/aestest.cpp
##
#DISTFILES   *= src/QtAES/unit_test/longText.txt
#RESOURCES   *= src/QtAES/res.qrc
###############################################################################

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS
#
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#
macos:QMAKE_INFO_PLIST = macos/Info.plist
ios:QMAKE_INFO_PLIST = ios/Info.plist
#

release: DESTDIR = "$${OUT_PWD}/build/release"
debug:   DESTDIR = "$${OUT_PWD}/build/debug"

OBJECTS_DIR = "$${DESTDIR}/obj"
MOC_DIR     = "$${DESTDIR}/moc"
RCC_DIR     = "$${DESTDIR}/qrc"
UI_DIR      = "$${DESTDIR}/ui"

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    target.path         = $${PREFIX}/bin
    shortcutfiles.files = usr/share/applications/$${TARGET}.desktop
    shortcutfiles.path  = usr/share/applications
    data.files         += usr/share/icons/hicolor/48x48/apps/$${TARGET}.png
    data.path           = usr/share/pixmaps
    INSTALLS           += shortcutfiles
    INSTALLS           += data
    INSTALLS           += target
    macx {
        RC_FILE = macos/Icon.icns
    }
}
################################ End of File ##################################
