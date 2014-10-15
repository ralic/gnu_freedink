#!/bin/bash -ex
# MS Woe release, built with MXE

# Copyright (C) 2008, 2009, 2010, 2012, 2013, 2014  Sylvain Beucler

# This file is part of GNU FreeDink

# GNU FreeDink is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

# GNU FreeDink is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see
# <http://www.gnu.org/licenses/>.

# cd /usr/src/mxe/  # master branch
## Disable MP3 support: src/sdl_mixer.mk: --enable-music-mp3 -> --disable-music-mp3
## Use MinGW-w64 in /usr/src/mxe/settings.mk: MXE_TARGETS := x86_64-w64-mingw32.static i686-w64-mingw32.static
## Use SDL_gfx 2.0.25: git checkout origin/master src/sdl_gfx.mk
# make -j2 JOBS=$(nproc) sdl2 sdl2_gfx sdl2_image sdl2_mixer sdl2_ttf libzip gettext check nsis

PACKAGE=freedink
VERSION=$(make version | tail -1)

make dist
rm -rf $PACKAGE-$VERSION/
tar xzf $PACKAGE-$VERSION.tar.gz
pushd $PACKAGE-$VERSION/
mkdir zip/

# documentation
for i in TRANSLATIONS.txt; do
    cp $i zip/freedink-$i
done
for i in AUTHORS COPYING NEWS README THANKS TROUBLESHOOTING; do
    cp $i zip/freedink-$i.txt
done
cat <<EOF > zip/freedink-SOURCE.txt
The FreeDink source code is available at:
  http://ftp.gnu.org/gnu/freedink/

The source code is the "recipe" of FreeDink, that let you study it,
modify it, and redistribute your changes.  The GNU GPL license
explicitely allows you to do so (see freedink-COPYING.txt).

If you upload a FreeDink .exe on your website, you must also offer the
corresponding source code for download.
EOF

# Include documentation with MS-DOS newlines (if not already)
sed -i -e 's/\(^\|[^\r]\)$/\1\r/' zip/freedink-*.txt


##
# Compile!
##
# full-static
PATH=/usr/src/mxe/usr/bin:$PATH

rm -rf cross-woe-32/
mkdir cross-woe-32/
pushd cross-woe-32/
../configure --host=i686-w64-mingw32.static \
  --enable-static --enable-upx
make -j $(nproc)
make install-strip DESTDIR=$(pwd)/destdir
# move .exe but avoid symlinks
find destdir/usr/local/bin/ -type f -name "*.exe" | while read file; do
  mv $file ../zip/
done
# Resources
cp -a destdir/usr/local/share/freedink zip/
cp -a destdir/usr/local/share/locale zip/freedink/
popd
#rm -rf cross-woe-32/

# 64-bit support:
# - upx-ucl >= 3.91 for w64 support (in Debian >= 8 "Jessie")
# - use a recent mingw-w64, issues with wchar.h on mingw-w64 v2.0 (GCC 4.6)
# - NSIS universal installer should be possible:
#   http://bojan-komazec.blogspot.fr/2011/10/nsis-installer-for-64-bit-windows.html
rm -rf cross-woe-64/
mkdir cross-woe-64/
pushd cross-woe-64/
../configure --host=x86_64-w64-mingw32.static \
  --enable-static --enable-upx
make -j$(nproc) install-strip DESTDIR=`pwd`/destdir
# move .exe but avoid symlinks
find destdir/usr/local/bin/ -type f -name "*.exe" | while read file; do
  mv $file ../zip/$(basename ${file%.exe}64.exe)
done
popd
#rm -rf cross-woe-64/

(cd zip/ && zip -r ../../$PACKAGE-$VERSION-bin.zip .)


popd
#rm -rf $PACKAGE-$VERSION/
