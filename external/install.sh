setenv_x86_mac()
{
	export CFLAGS="-arch x86_64 -no-cpp-precomp"
}

setenv_arm_mac()
{
	export CFLAGS="-arch arm64 -no-cpp-precomp"
}

#Create staging directory
STAGING=~/Desktop/vorbis-tools
mkdir -p $STAGING

#Sources
SRC=$STAGING/src
mkdir -p $SRC

#Get the libogg
wget downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.xz -qO-|tar -C $SRC -xJ

#Get the "libvorbis"
wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.xz -qO-|tar -C $SRC -xJ

#Build artifacts for x86
OUT=$STAGING/build-macos-x86
mkdir -p $OUT

#Build a static version of "libogg" for mac os x86
pushd $SRC/libogg*/
setenv_x86_mac
make clean
./configure --prefix=$OUT --disable-shared --build=x86_64-apple-darwin --host=arm-apple-darwin
make install
popd

#Build a static version of "libvorbis" for mac os x86
pushd $SRC/libvorbis*/
make clean
setenv_x86_mac
./configure --prefix=$OUT LDFLAGS="-L$OUT/lib" CPPFLAGS="-I$OUT/include" --build=x86_64-apple-darwin --host=arm-apple-darwin
make install
popd

#Build artifacts for x86
OUT=$STAGING/build-macos-arm
mkdir -p $OUT

#Build a static version of "libogg" for mac os x86
pushd $SRC/libogg*/
setenv_arm_mac
make clean
./configure --prefix=$OUT --disable-shared --build=apple --build=arm-apple-darwin --host=arm-apple-darwin
make install
popd

#Build a static version of "libvorbis" for mac os x86
pushd $SRC/libvorbis*/
make clean
setenv_arm_mac
./configure --prefix=$OUT LDFLAGS="-L$OUT/lib" CPPFLAGS="-I$OUT/include" --build=arm-apple-darwin --host=arm-apple-darwin
make install
popd

#Make the far macos library
OUT=$STAGING/build-macos
mkdir -p $OUT
cp -r $STAGING/build-macos-arm/include $OUT
mkdir -p $OUT/lib
lipo -create $STAGING/build-macos-arm/lib/libogg.a $STAGING/build-macos-x86/lib/libogg.a -output $OUT/lib/libogg.a
lipo -create $STAGING/build-macos-arm/lib/libvorbis.a $STAGING/build-macos-x86/lib/libvorbis.a -output $OUT/lib/libvorbis.a
lipo -create $STAGING/build-macos-arm/lib/libvorbisenc.a $STAGING/build-macos-x86/lib/libvorbisenc.a -output $OUT/lib/libvorbisenc.a
lipo -create $STAGING/build-macos-arm/lib/libvorbisfile.a $STAGING/build-macos-x86/lib/libvorbisfile.a -output $OUT/lib/libvorbisfile.a
ID=`uuidgen`
mv $STAGING/build-macos-arm ~/.Trash/build-macos-arm-$ID
mv $STAGING/build-macos-x86 ~/.Trash/build-macos-x86-$ID

