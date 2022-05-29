#Create staging directory
PWD=`pwd`
STAGING=$PWD/vorbis-tools
mkdir -p $STAGING

#Sources
SRC=$STAGING/src
mkdir -p $SRC

#Get the libogg
wget downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.xz -qO-|tar -C $SRC -xJ

#Get the "libvorbis"
wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.xz -qO-|tar -C $SRC -xJ

DEVELOPER=`xcode-select -print-path`
ARCHS="armv7"

setenv_clean()
{
	unset CFLAGS
	unset PLATFORM
	unset SDK
	unset CC
}

setenv_x86_mac()
{
	export CFLAGS="-arch x86_64 -no-cpp-precomp"
}

setenv_arm_mac()
{
	export CFLAGS="-arch arm64 -no-cpp-precomp"
}

setenv_armv7()
{
	PLATFORM="iPhoneOS"
	SDK="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/iPhoneOS.sdk"
	#export CFLAGS="-arch armv7 -no-cpp-precomp"
	export CC="clang -arch armv7 -isysroot ${SDK}"
}

setenv_all()
{
	export CXXFLAGS="$CFLAGS"
    export LDFLAGS="$CFLAGS"
    export LD=$CC
}

#
#Build artifacts for macos-x86
OUT=$STAGING/build-macos-x86
mkdir -p $OUT

#Build a static version of "libogg" for mac os x86
pushd $SRC/libogg*/
setenv_clean
setenv_x86_mac
setenv_all
make clean
./configure --prefix=$OUT --disable-shared --host=arm-apple-darwin
make install
popd

#Build a static version of "libvorbis" for mac os x86
pushd $SRC/libvorbis*/
setenv_clean
setenv_x86_mac
setenv_all
make clean
./configure --prefix=$OUT LDFLAGS="-L$OUT/lib" CPPFLAGS="-I$OUT/include" --host=arm-apple-darwin
make install
popd

#
#Build artifacts for macos-arm
OUT=$STAGING/build-macos-arm
mkdir -p $OUT

#Build a static version of "libogg" for macos-arm
pushd $SRC/libogg*/
setenv_clean
setenv_arm_mac
setenv_all
make clean
./configure --prefix=$OUT --disable-shared --host=arm-apple-darwin
make install
popd

#Build a static version of "libvorbis" for macos-arm
pushd $SRC/libvorbis*/
make clean
setenv_clean
setenv_arm_mac
setenv_all
./configure --prefix=$OUT LDFLAGS="-L$OUT/lib" CPPFLAGS="-I$OUT/include" --host=arm-apple-darwin
make install
popd

#
#Build artifacts for macos-arm
OUT=$STAGING/build-ios-armv7
mkdir -p $OUT

#Build a static version of "libogg" for ios-armv7
pushd $SRC/libogg*/
setenv_clean
setenv_armv7
setenv_all
make clean
./configure --prefix=$OUT --disable-shared --host=arm-apple-darwin
make install
popd

#Build a static version of "libvorbis" for macos-arm
pushd $SRC/libvorbis*/
make clean
setenv_clean
setenv_armv7
setenv_all
./configure --prefix=$OUT LDFLAGS="-L$OUT/lib" CPPFLAGS="-I$OUT/include" --host=arm-apple-darwin
make install
popd


#Make the far macos library
OUT=$STAGING/apple
mkdir -p $OUT
cp -r $STAGING/build-macos-arm/include $OUT
mkdir -p $OUT/lib
lipo -create $STAGING/build-macos-arm/lib/libogg.a $STAGING/build-macos-x86/lib/libogg.a $STAGING/build-ios-armv7/lib/libogg.a -output $OUT/lib/libogg.a
lipo -create $STAGING/build-macos-arm/lib/libvorbis.a $STAGING/build-macos-x86/lib/libvorbis.a $STAGING/build-ios-armv7/lib/libvorbis.a -output $OUT/lib/libvorbis.a
lipo -create $STAGING/build-macos-arm/lib/libvorbisenc.a $STAGING/build-macos-x86/lib/libvorbisenc.a $STAGING/build-ios-armv7/lib/libvorbisenc.a -output $OUT/lib/libvorbisenc.a
lipo -create $STAGING/build-macos-arm/lib/libvorbisfile.a $STAGING/build-macos-x86/lib/libvorbisfile.a $STAGING/build-ios-armv7/lib/libvorbisfile.a -output $OUT/lib/libvorbisfile.a
ID=`uuidgen`
mv $STAGING/build-macos-arm ~/.Trash/build-macos-arm-$ID
mv $STAGING/build-macos-x86 ~/.Trash/build-macos-x86-$ID
mv $STAGING/build-ios-armv7 ~/.Trash/build-ios-armv7-$ID
