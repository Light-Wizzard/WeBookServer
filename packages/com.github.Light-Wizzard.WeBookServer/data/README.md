# WeBookServer



## Travis Build

If you look at .travis.yml, you will notice I am using Berineri Qt 5.14.1:
https://launchpad.net/~beineri/+archive/ubuntu/opt-qt-5.14.1-xenial

Note how this is defined: QTV="qt514"


```
sudo apt-get install --yes -q "${QTV}meta-full";
```

For some reason opt-qt-5.14.2-xenial fails to install, I did not troubleshoot yet, fall back to 5.14.1.

## Current Problems with build



```
qmake -query
QT_SYSROOT:
QT_INSTALL_PREFIX:/usr
QT_INSTALL_ARCHDATA:/usr/lib/qt
QT_INSTALL_DATA:/usr/share/qt
QT_INSTALL_DOCS:/usr/share/doc/qt
QT_INSTALL_HEADERS:/usr/include/qt
QT_INSTALL_LIBS:/usr/lib
QT_INSTALL_LIBEXECS:/usr/lib/qt/libexec
QT_INSTALL_BINS:/usr/bin
QT_INSTALL_TESTS:/usr/tests
QT_INSTALL_PLUGINS:/usr/lib/qt/plugins
QT_INSTALL_IMPORTS:/usr/lib/qt/imports
QT_INSTALL_QML:/usr/lib/qt/qml
QT_INSTALL_TRANSLATIONS:/usr/share/qt/translations
QT_INSTALL_CONFIGURATION:/etc/xdg
QT_INSTALL_EXAMPLES:/usr/share/doc/qt/examples
QT_INSTALL_DEMOS:/usr/share/doc/qt/examples
QT_HOST_PREFIX:/usr
QT_HOST_DATA:/usr/lib/qt
QT_HOST_BINS:/usr/bin
QT_HOST_LIBS:/usr/lib
QMAKE_SPEC:linux-g++
QMAKE_XSPEC:linux-g++
QMAKE_VERSION:3.1
QT_VERSION:5.14.2
```

This is on Travis

```
QT_SYSROOT:
QT_INSTALL_PREFIX:/opt/qt514
QT_INSTALL_ARCHDATA:/opt/qt514
QT_INSTALL_DATA:/opt/qt514
QT_INSTALL_DOCS:/opt/qt514/doc
QT_INSTALL_HEADERS:/opt/qt514/include
QT_INSTALL_LIBS:/opt/qt514/lib
QT_INSTALL_LIBEXECS:/opt/qt514/libexec
QT_INSTALL_BINS:/opt/qt514/bin
QT_INSTALL_TESTS:/opt/qt514/tests
QT_INSTALL_PLUGINS:/opt/qt514/plugins
QT_INSTALL_IMPORTS:/opt/qt514/imports
QT_INSTALL_QML:/opt/qt514/qml
QT_INSTALL_TRANSLATIONS:/opt/qt514/translations
QT_INSTALL_CONFIGURATION:/opt/qt514/etc/xdg
QT_INSTALL_EXAMPLES:/opt/qt514/examples
QT_INSTALL_DEMOS:/opt/qt514/examples
QT_HOST_PREFIX:/opt/qt514
QT_HOST_DATA:/opt/qt514
QT_HOST_BINS:/opt/qt514/bin
QT_HOST_LIBS:/opt/qt514/lib
QMAKE_SPEC:linux-g++
QMAKE_XSPEC:linux-g++
QMAKE_VERSION:3.1
QT_VERSION:5.14.1
```

### Travis what not to try

Building Qt from Single will timeout during extraction.

```
mkdir -p Qt-Build;
wget https://download.qt.io/official_releases/qt/5.15/5.15.0/single/qt-everywhere-src-5.15.0.tar.xz;
7z x "qt-everywhere-src-5.15.0.tar.xz" -so | 7z x -aoa -si -ttar -o"./Qt-Build";
cd Qt-Build;
./configure;
make -j4;
sudo make install;
cd ..;
```

## What WeBookServer should do

The concept is that you run an installer bash script on your VPS Server,
it does a git clone and downloads to your server, so look at the code before doing this,
and know what its doing to your VPS server, this App will Control the server,
much like most Web Control Panels, the WeBookClient can add and remove users,
do updates, reboot the server when required, and basically manage every aspect of the server.

The WeBookClient has an Admit feature to control a Domain, or a Domain Controller,
it can manage its own WeBook, which is a concept that a Web Site,
is handled like a Book Manager, so a WeBook, is a Web Site,
and it can manage as many as you want to add,
so it is a Content Management System,


I am running on Ubuntu Xenial Qt 5.14.1 on Travis

See the build-with-qmake.sh

## Status

Unknown

## Travis

I am currently having issues with Travis and Qt 5.14.1 thanks to Beineri

I use Travis to test the installation, as well as to create the installation packages.

I might use Qt installation Framework, as well as App Image, the code is there, just needs to be activated.

https://travis-ci.org/github/Light-Wizzard/WeBookServer

## More Information

More Information can be found in the Wiki https://github.com/Light-Wizzard/WeBookServer/wiki/System.d

## WeBookClient

Currently I do not have a repo for WeBookClient,
and I do not have much at:
https://github.com/Light-Wizzard/WeBook,
but it will eventually point to all the projects in use.

WeBookClient has a lot of work to integrate whatever service I do go with,
but I like to post things that have a chance of working,
so I will not publish this code, till I have a back end for it.

Most of the code works now, its basically a Rich Text Editor, that saves in a book layout.

This makes it easy to make PDF, ePub, RTF, DOCX, or HTML files, you can publish with a push of a button.


### End of File
