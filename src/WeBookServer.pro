# WeBookServer.pro
# Specifies the name of the template to use when generating the project.
# The allowed values are: app, lib, subdirs, aux, vcapp or vclib
TEMPLATE     = "app"
# Specifies the name of the target file. Contains the base name of the project file by default.
# Do not use Spaces in Target Name
# This pro(ject) file is based on a pattern
TARGET       = "WeBookServer"
#
#INCLUDEPATH     += $$clean_path($$absolute_path("src/"))
#INCLUDEPATH     += $$clean_path($$absolute_path("src/QtService"))

#INCLUDEPATH     += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/3rdparty"))
#INCLUDEPATH     += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/3rdparty/http-parser"))
#INCLUDEPATH     += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/httpserver"))
#INCLUDEPATH     += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/sslserver"))

#DEPENDPATH      += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/3rdparty"))
#DEPENDPATH      += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/3rdparty/http-parser"))
#DEPENDPATH      += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/httpserver"))
#DEPENDPATH      += $$clean_path($$absolute_path("3rdparty/qthttpserver/src/sslserver"))

#DEPENDPATH      += $$clean_path($$absolute_path("src/"))
#DEPENDPATH      += $$clean_path($$absolute_path("src/QtService"))
INCLUDEPATH     += .
INCLUDEPATH     += src/
INCLUDEPATH     += src/QtService
#INCLUDEPATH     += 3rdparty/qthttpserver/src/3rdparty
#INCLUDEPATH     += 3rdparty/qthttpserver/src/3rdparty/http-parser
#INCLUDEPATH     += 3rdparty/qthttpserver/src/httpserver
#INCLUDEPATH     += 3rdparty/qthttpserver/src/sslserver
INCLUDEPATH     += 3rdparty/qthttpserver/include
INCLUDEPATH     += 3rdparty/qthttpserver/include/QtHttpServer
INCLUDEPATH     += 3rdparty/qthttpserver/include/QtSslServer
#
DEPENDPATH      += src/
DEPENDPATH      += src/QtService
#DEPENDPATH      += 3rdparty/qthttpserver/src/3rdparty
#DEPENDPATH      += 3rdparty/qthttpserver/src/3rdparty/http-parser
#DEPENDPATH      += 3rdparty/qthttpserver/src/httpserver
#DEPENDPATH      += 3rdparty/qthttpserver/src/sslserver
DEPENDPATH      += 3rdparty/qthttpserver/include
DEPENDPATH      += 3rdparty/qthttpserver/include/QtHttpServer
DEPENDPATH      += 3rdparty/qthttpserver/include/QtSslServer
#
QT              -= gui
QT              *= core
QT              *= core-private
!win32:QT       *= network

CONFIG          *= qt
CONFIG          *= console
CONFIG          *= cmdline
CONFIG          -= app_bundle
CONFIG          *= "c++11"
#CONFIG          *= "c++14"
#CONFIG         *= "c++1z"
#CONFIG         *= "c++17"
#CONFIG         *= "c++2a"
#CONFIG         *= "c++latest"
# Headers
HEADERS         += src/webookserver.h   src/QtService/qtservice.h   src/QtService/qtservice_p.h
# Source Code
SOURCES         += src/webookserver.cpp src/QtService/qtservice.cpp src/main.cpp
win32:SOURCES   += src/QtService/qtservice_win.cpp
unix:HEADERS    += src/QtService/qtunixsocket.h src/QtService/qtunixserversocket.h
unix:SOURCES    += src/QtService/qtservice_unix.cpp src/QtService/qtunixsocket.cpp src/QtService/qtunixserversocket.cpp
unix:DISTFILES  += tools/haproxy.cfg tools/monit.conf tools/webookserver.ini tools/webookserver.sh tools/webooksetup.sh
DISTFILES       += README.md usr/bin/README.md usr/share/applications/$${TARGET}.desktop
TRANSLATIONS    += src/WeBookServer_en_US.ts

win32:VERSION = 0.1.0.0 # major.minor.patch.build
else:VERSION  = 0.1.0   # major.minor.patch


SUBDIRS         += 3rdparty/qthttpserver/qthttpserver.pro

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

win32:LIBS      += -luser32
LIBS            += -L"3rdparty/qthttpserver/lib" -lQt5HttpServer

win32 {
    qtservice-buildlib:shared:DEFINES += QT_QTSERVICE_EXPORT
    else:qtservice-uselib:DEFINES     += QT_QTSERVICE_IMPORT
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
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


#unix {
#    isEmpty(PREFIX) {
#        PREFIX = /usr
#    }
#    message("PREFIX=$${PREFIX}")
#    target.path = $${PREFIX}/bin
#    DISTDIR    += $$clean_path($$absolute_path("$${PREFIX}/bin"))
#    message("DISTDIR=$$DISTDIR")
#    shortcutfiles.files = $$clean_path($$absolute_path("usr/share/applications/$${TARGET}.desktop"))
#    shortcutfiles.path  = $$clean_path($$absolute_path("usr/share/applications/"))
#    data.files += $$clean_path($$absolute_path("resources/$${TARGET}.png"))
#    data.path   = $$clean_path($$absolute_path("usr/share/pixmaps/"))
#    INSTALLS   += shortcutfiles
#    INSTALLS   += data
#    INSTALLS   += target
#    macx {
#        RC_FILE = macos/Icon.icns
#    }
#}
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



