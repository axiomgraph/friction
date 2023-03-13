# enve2d

This is a community fork of [enve](https://github.com/MaurycyLiebner/enve). A 2d animation application.

A lot of changes are currently happening under the hood, so don't expect anything usable until our first [release](https://github.com/enve2d/enve2d/milestone/1).

Features currently present will be checked, if they are broken or not fully implemented a review will be done:

* If they are broken check if a fix is feasible, else remove or disable
* If they are not fully implemented check if this is a feature worth implementing, else remove or disable

The goal is to end up with a [v1.0.0](https://github.com/enve2d/enve2d/milestone/7) release in the not too distant future.

See our issue [tracker](https://github.com/enve2d/enve2d/issues) for more information.

## License

Copyright &copy; 2023 enve2d developers

Copyright &copy; 2016-2022 Maurycy Liebner

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

## Build

We are moving to CMake and currently only Linux is supported, see open [issue](https://github.com/enve2d/enve2d/issues/1) for progress on other platforms.

### Linux

General instructions for building enve2d on Linux *(tested on Ubuntu Jammy)*.

### Requirements

* automake
* autoconf
* python2
* ninja
* cmake
* Qt5
    * Gui
    * Concurrent
    * Widgets
    * OpenGL
    * Multimedia
    * Qml
    * Xml
    * Svg
    * WebEngineWidgets
* libmypaint
* quazip
* qscintilla
* libavformat
* libavcodec
* libavutil
* libswscale
* libswresample
* libunwind

### Ubuntu

```
sudo apt install -y \
build-essential \
autoconf \
automake \
cmake \
python2 \
ninja-build \
libfontconfig1-dev \
libfreetype-dev \
libavcodec-dev \
libavformat-dev \
libavutil-dev \
libmypaint-dev \
libqscintilla2-qt5-dev \
libqt5opengl5-dev \
libqt5svg5-dev \
libquazip5-dev \
libswresample-dev \
libswscale-dev \
libunwind-dev \
qt5-image-formats-plugins \
libqt5multimedia5-plugins \
qtbase5-dev-tools \
qtbase5-dev \
qtdeclarative5-dev-tools \
qtdeclarative5-dev \
qtmultimedia5-dev \
qttools5-dev-tools \
qtwebengine5-dev \
libqt5xmlpatterns5-dev
```

Clone the repository, this might take a while.

```
git clone --recurse-submodules https://github.com/enve2d/enve2d
```

Build a custom version of ``libtcmalloc``:

```
cd enve2d/src/gperftools
./autogen.sh
./configure --disable-shared
make -j4
```

Build ``skia``:

Note that python2 is required. ``/usr/bin/python`` should be a symlink to the python2 binary on your system.

On Ubuntu run:

```
sudo ln -sf /usr/bin/python2 /usr/bin/python
```

Now build ``skia`` (might take a while):

```
cd enve2d/src/skia
python tools/git-sync-deps
bin/gn gen out/build --args='is_official_build=true is_debug=false extra_cflags=["-Wno-error"] target_os="linux" target_cpu="x64" skia_use_system_expat=false skia_use_system_freetype2=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_icu=false skia_use_system_harfbuzz=false'
ninja -C out/build -j4 skia
```

Now you are ready to build ``enve2d``:

```
cd enve2d
mkdir build
cd build
```

Note that if you are not using Ubuntu you will need to set paths for ``qscintilla``:

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DQSCINTILLA_LIBARIES="path/to/libraries" -DQSCINTILLA_INCLUDES="path/to/includes" ..
```

else just use:

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
```

Now build:

```
make -j4
```

Run:

```
cp src/app/enve2d .
cp src/core/libenve2dcore.so.0 .
./enve2d
```

Install (or prepare for package):

```
make DESTDIR=`pwd`/enve2d install 
```

```
enve2d
└── usr
    ├── bin
    │   └── enve2d
    ├── lib
    │   └── x86_64-linux-gnu
    │       ├── libenve2dcore.so -> libenve2dcore.so.0
    │       ├── libenve2dcore.so.0 -> libenve2dcore.so.0.9.0
    │       └── libenve2dcore.so.0.9.0
    └── share
        ├── applications
        │   └── enve2d.desktop
        ├── doc
        │   └── enve2d-0.9.0
        │       ├── LICENSE-gperftools.md
        │       ├── LICENSE.md
        │       ├── LICENSE-skia.md
        │       └── README.md
        ├── icons
        │   └── hicolor
        │       ├── 128x128
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 16x16
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 192x192
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 22x22
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 256x256
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 32x32
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 48x48
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 64x64
        │       │   └── apps
        │       │       └── enve2d.png
        │       ├── 96x96
        │       │   └── apps
        │       │       └── enve2d.png
        │       └── scalable
        │           └── apps
        │               └── enve2d.svg
        └── mime
            └── packages
                └── enve2d.xml
```

or make a DEB package:

```
make package
```
