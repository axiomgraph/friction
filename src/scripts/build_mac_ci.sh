#!/bin/bash
#
# Friction - https://friction.graphics
#
# Copyright (c) Ole-André Rodlie and contributors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

set -e -x

CWD=`pwd`

SDK=20240915
URL=https://github.com/friction2d/friction-sdk/releases/download/${SDK}
SDK_TAR=friction-sdk-macOS12.7-v1.tar.bz2

if [ ! -d "${CWD}/sdk" ]; then
    curl -OL ${URL}/${SDK_TAR}
    tar xf ${SDK_TAR}
fi

git submodule update -i --recursive

CUSTOM=CI ./src/scripts/build_mac.sh