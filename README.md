# WeBookServer

QT Multithreaded Server to daemonized server in Qt, and handles connections asychro via the QThreads.

This application is a Linux Daemon.


## Travis Build

If you look at .travis.yml, you will notice I am using Berineri Qt 5.14.1:
https://launchpad.net/~beineri/+archive/ubuntu/opt-qt-5.14.1-xenial

Note how this is defined: QTV="qt514"

Note: meta-full should be everything, so I might cut it down to just that later,
for now, I am trying to resolve network.

```
sudo apt-get install --yes -q "${QTV}meta-full" "${QTV}base" "${QTV}tools" "${QTV}translations" "${QTV}x11extras";
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

## What Example should do

The Server sets up a localhost TCP Socket on port 6633,
it only allows one instanstance of the Server to run on this port.

I am running on Ubuntu Xenial Qt 5.13.2,


See the build-with-qmake.sh

## Status

Not Working.

## Travis

https://travis-ci.org/github/Light-Wizzard/WeBookServer

## AppVeyor

https://ci.appveyor.com/project/Light-Wizzard/WeBookServer

## More Information

More Information can be found in the Wiki https://github.com/Light-Wizzard/WeBookServer/wiki/System.d

### End of File
