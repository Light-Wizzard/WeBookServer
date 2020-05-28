# WeBookServer.pro qmake file
requires(qtHaveModule(network))
# I will define all the variables you need to change for you project at the top
# Target is the Name of this Application, you can change it to fit your needs
TARGET               = WeBookServer
DEFINES             += APP_TARGET=\\\"$${TARGET}\\\"
# I will define my TARGET folder here:
TARGET_FOLDER        = WeBookServer
#
TARGET_DEPLOY_FOLDER = "deploy"
#
PACKAGE_FOLDER_NAME = "com.github.Light-Wizzard.WeBookServer"
#
#BUILD_ON_TRAVIS    = "Comment to build locally"
#
DESTDIR              = $$clean_path("$${_PRO_FILE_PWD_}/$${TARGET_DEPLOY_FOLDER}")
# The value of the does not matter, it only checks for isEmpty
MY_SILENT_MESSAGE = "COMMENT-TO-SILENCE-MESSAGES"
# VERSION Section
# Specifies the version number of the application if the app template is specified or the version number of the library if the lib template is specified.
win32:VERSION = 0.1.0.0 # major.minor.patch.build
else:VERSION  = 0.1.0   # major.minor.patch
DEFINES             += APP_VERSION=\\\"$${VERSION}\\\"
# we use app as our template for an exec
TEMPLATE             = app
# we set QT to service, which is a path to QtServer
# Project ERROR: Unknown module(s) in QT: service
QT                  *= core network
# we remove the GUI
QT                  -= gui
# configure as a concole
CONFIG              *= cmdline
CONFIG              *= console
CONFIG              *= warn_on utf8_source executable c++17 thread
# and do not bundle it: cmdline
CONFIG              -= app_bundle
unix {
    CONFIG          *= x11
}
# These are the files we need
HEADERS     += mylogger.h   mythread.h   myserver.h
SOURCES     += mylogger.cpp mythread.cpp myserver.cpp main.cpp
# now set them as Distrobution files
DISTFILES           += $$clean_path($$absolute_path("$${TARGET}.service"))
DISTFILES           += $$clean_path($$absolute_path("$${TARGET}.plist"))
DISTFILES           += $$clean_path($$absolute_path("scinstall.bat"))
DISTFILES           += $$clean_path($$absolute_path("$${TARGET}.socket"))
DISTFILES           += $$clean_path($$absolute_path("LICENSE"))
DISTFILES           += $$clean_path($$absolute_path("README.md"))
#DISTFILES           += $$clean_path($$absolute_path("CODE_OF_CONDUCT.md"))
#DISTFILES           += $$clean_path($$absolute_path("config/config.xml"))
#DISTFILES           += $$clean_path($$absolute_path("packages/$${PACKAGE_FOLDER_NAME}/data/README.md"))
#DISTFILES           += $$clean_path($$absolute_path("packages/$${PACKAGE_FOLDER_NAME}/meta/installscript.qs"))
#DISTFILES           += $$clean_path($$absolute_path("packages/$${PACKAGE_FOLDER_NAME}/meta/package.xml"))
#DISTFILES           += $$clean_path($$absolute_path("tools/build-with-qmake.sh"))
#DISTFILES           += $$clean_path($$absolute_path("tools/qtinstallerframework.7z"))
#DISTFILES           += $$clean_path($$absolute_path("tools/upload.sh"))
#
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES     *= QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
# disables all the APIs deprecated before Qt 6.0.0
DEFINES     *= QT_DISABLE_DEPRECATED_BEFORE=0x060000
# undefined symbol: _ZdlPvm, version Qt_5
QMAKE_CXXFLAGS += "-fno-sized-deallocation"
### function messenger("message") #############################################
###### This wraps all message calls so they can be silenced with one comment
defineTest(messenger) {
    !isEmpty(MY_SILENT_MESSAGE) {
        message($$1)
    }
}
# Show Qt Info
defineTest(showQtInfo) {
    messenger("***-> ************* Show Qt Info Begin ************* <-***")
    messenger(Qt version: $$[QT_VERSION])
    messenger(Qt is installed in $$[QT_INSTALL_PREFIX])
    messenger(Qt resources can be found in the following locations:)
    messenger(Documentation: $$[QT_INSTALL_DOCS])
    messenger(Header files: $$[QT_INSTALL_HEADERS])
    messenger(Libraries: $$[QT_INSTALL_LIBS])
    messenger(Binary files (executables): $$[QT_INSTALL_BINS])
    messenger(Plugins: $$[QT_INSTALL_PLUGINS])
    messenger(Data files: $$[QT_INSTALL_DATA])
    messenger(Translation files: $$[QT_INSTALL_TRANSLATIONS])
    messenger(Settings: $$[QT_INSTALL_SETTINGS])
    messenger(Examples: $$[QT_INSTALL_EXAMPLES])
    messenger(Demonstrations: $$[QT_INSTALL_DEMOS])
    messenger("***<- ************* Show Qt Info End ************* ->***")
}
showQtInfo("")
### function checkAllFilesExist(file_list) ####################################
defineTest(checkAllFilesExist) {
    files = $$1
    for(FILE, files) {
        !exists($$FILE) {
            error("File does Not Exist -> $$FILE")
        }
    }
}
checkAllFilesExist($$HEADERS)
checkAllFilesExist($$SOURCES)
checkAllFilesExist($$DISTFILES)
#checkAllFilesExist(/usr/include/qt/QtService/service.h)
#checkAllFilesExist($$MY_QTSERVICE_LIB)
# Create folders if they do not exist
#
# Windows Users:
# If you can not run mkdir in a terminal see the below link
# https://www.windowscentral.com/install-windows-subsystem-linux-windows-10
defineTest(ifNotExistFolderCreate) {
    theFolder = $$1
    exists($$theFolder) {
            messenger("Folder Exists -> $$theFolder")
        } else {
                QMAKE_POST_LINK += $$quote(mkdir $${theFolder} $$escape_expand(\n\t))
                export(QMAKE_POST_LINK)
                messenger("Created Folder -> $$theFolder")
    }
}
# call the function above
ifNotExistFolderCreate("$${DESTDIR}")
# Copies the given files to the destination directory
defineTest(copyToDestdir) {
    files = $$1
    messenger("files=$$files")
    for(FILE, files) {
        messenger("FILE=$$FILE")
        DDIR = $$DESTDIR
        messenger("DDIR=$$DDIR")
        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g
        # create return link
        messenger("copyToDestdir($$FILE, $$DDIR)")
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}
# call the function above
copyToDestdir($$DISTFILES) # a variable containing multiple paths
#
# Now the target.path can get confusing
# the qmake variable $$[QT_INSTALL_EXAMPLES], will look for example.pro
# service is a path that has the examples in it,
# and where this one is from, as such,
# check where its built
    # if running outside it
!isEmpty(BUILD_ON_TRAVIS) {
    unix {
        isEmpty(PREFIX) {
            PREFIX = "/$${DESTDIR}/lib/systemd/system/$${TARGET_FOLDER}"
        }
    }
} else {
    isEmpty(PREFIX) {
        PREFIX = "/$${DESTDIR}"
    }
}
target.path             = "$${PREFIX}"
install_svcconf.path    = "$${PREFIX}"
#
# if running outside it, do this, where $${TARGET_FOLDER} is the variabl for the folder name,
# Do not cargo-cult this! basically means to keep in lean and mean,
# it will ignore stuff that is not set in this target correcly
# not to add all the default extras, if true, it means debug is added
!install_ok: INSTALLS  += target
# This is where the Magic takes place for specific OS
# Windows uses bat files
win32: install_svcconf.files                += $$shadowed($$clean_path($$absolute_path("scinstall.bat")))
# Mac uses plist
else:macos: install_svcconf.files           += $$shadowed($$clean_path($$absolute_path("$${TARGET}.plist")))
# If Linux but not Andriod
else:linux:!android: install_svcconf.files  += $$shadowed($$clean_path($$absolute_path("$${TARGET}.service"))) $$clean_path($$absolute_path("$${TARGET}.socket"))
install_svcconf.CONFIG                      += no_check_exist
# append to INSTALLS
!install_ok: INSTALLS                       += install_svcconf
messenger("INSTALLS=$$INSTALLS")
messenger("target.path=$$target.path")
################################ End of File ##################################
