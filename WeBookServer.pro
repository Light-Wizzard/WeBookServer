# WeBookServer.pro
TEMPLATE = app

QT += service
QT -= gui

CONFIG += console
CONFIG -= app_bundle

TARGET = WeBookServer

HEADERS += src/WeBookServer.h
HEADERS += src/WeBookSettings.h
HEADERS += src/WeBookCrypto.h

SOURCES += src/WeBookServer.cpp src/main.cpp
SOURCES += src/WeBookSettings.cpp
SOURCES += src/WeBookCrypto.cpp

QMAKE_SUBSTITUTES += WeBookServer.service.in scinstall.bat.in WeBookServer.plist.in

DISTFILES += $$QMAKE_SUBSTITUTES WeBookServer.socket

unix:DISTFILES  *= ../tools/haproxy.cfg ../tools/monit.conf ../tools/webookserver.ini ../tools/webookserver.sh ../tools/webooksetup.sh
DISTFILES       *= README.md
win32:VERSION    = 0.1.0.0 # major.minor.patch.build
else:VERSION     = 0.1.0   # major.minor.patch
DEFINES          = APP_VERSION=\\\"$${VERSION}\\\"
win32:LIBS      *= -luser32
###############################################################################
# Forked: https://github.com/qt-labs/qthttpserver                             #
# https://github.com/Light-Wizzard/qthttpserver                               #
###############################################################################
# Local install
exists($$[QT_INSTALL_HEADERS]/qt) {
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/qt
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/qt/QtHttpServer
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/qt/QtSslServer
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/qt/QtHttpServer/5.12.0/QtHttpServer/private
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/qt/QtSslServer/5.12.0/QtSslServer/private
} else {
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/QtHttpServer
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/QtSslServer
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/QtHttpServer/5.12.0/QtHttpServer/private
    INCLUDEPATH     *= $$[QT_INSTALL_HEADERS]/QtSslServer/5.12.0/QtSslServer/private
    INCLUDEPATH     *= "include/QtHttpServer"
    INCLUDEPATH     *= "include/QtSslServer"
}
LIBS *= -L"$$[QT_INSTALL_LIBS]" -lQt5HttpServer
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
#QT          *= testlib
#HEADERS     *= src/QtAES/unit_test/aestest.h
#SOURCES     *= src/QtAES/maintest.cpp
#SOURCES     *= src/QtAES/unit_test/aestest.cpp
##
#DISTFILES   *= src/QtAES/unit_test/longText.txt
#RESOURCES   *= src/QtAES/res.qrc
###############################################################################
# Fork: https://github.com/francescmm/QLogger                                 #
# https://github.com/Light-Wizzard/QLogger                                    #
###############################################################################
#HEADERS     *= src/QLogger/QLoggerLevel.h
#HEADERS     *= src/QLogger/QLoggerConstants.h
# QLoggerManager
#HEADERS     *= src/QLogger/QLoggerManager.h
#SOURCES     *= src/QLogger/QLoggerManager.cpp
# QLoggerWriter
#HEADERS     *= src/QLogger/QLoggerWriter.h
#SOURCES     *= src/QLogger/QLoggerWriter.cpp
# QLoggerCommon
#HEADERS     *= src/QLogger/QLoggerCommon.h
#SOURCES     *= src/QLogger/QLoggerCommon.cpp
###############################################################################


target.path = $$[QT_INSTALL_EXAMPLES]/service/WeBookServer
!install_ok: INSTALLS += target

win32: install_svcconf.files += $$shadowed(scinstall.bat)
else:macos: install_svcconf.files += $$shadowed(WeBookServer.plist)
else:linux:!android: install_svcconf.files += $$shadowed(WeBookServer.service) WeBookServer.socket
install_svcconf.CONFIG += no_check_exist
install_svcconf.path = $$[QT_INSTALL_EXAMPLES]/service/WeBookServer
!install_ok: INSTALLS += install_svcconf
