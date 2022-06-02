#!/bin/bash

#Create staging directory
PWD=`pwd`
STAGING=$PWD/vorbis-tools
mkdir -p $STAGING

#Sources
SRC=$STAGING/src
mkdir -p $SRC

# Get the libogg
wget downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.xz -qO-|tar -C $SRC -xJ

#Get the "libvorbis"
wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.xz -qO-|tar -C $SRC -xJ

DEVELOPER=`xcode-select -print-path`

#List of possible build types
declare -a platforms
declare -a sdk
declare -a flags
declare -a folders

platforms=()
sdk=()
flags=()

INDEX=0
platforms[$INDEX]='iPhoneOS'
sdk[$INDEX]="${DEVELOPER}/Platforms/${platforms[$INDEX]}.platform/Developer/SDKs/iPhoneOS.sdk"
flags[$INDEX]="clang -isysroot ${sdk[$INDEX]} -target arm64-apple-ios15.4"
folder[$INDEX]="build-ios-device-armv7"

INDEX=1
platforms[$INDEX]='iPhoneSimulator'
sdk[$INDEX]="${DEVELOPER}/Platforms/${platforms[$INDEX]}.platform/Developer/SDKs/iPhoneSimulator.sdk"
flags[$INDEX]="clang -arch arm64 -target arm64-apple-ios15.4-simulator -isysroot ${sdk[$INDEX]}"
folder[$INDEX]="build-ios-sim-arm64"

INDEX=2
platforms[$INDEX]='iPhoneSimulator'
sdk[$INDEX]="${DEVELOPER}/Platforms/${platforms[$INDEX]}.platform/Developer/SDKs/iPhoneSimulator.sdk"
flags[$INDEX]="clang -arch x86_64 -target x86_64-apple-ios15.4-simulator -isysroot ${sdk[$INDEX]}"
folder[$INDEX]="build-ios-sim-x86_64"

INDEX=3
platforms[$INDEX]='MacOSX'
sdk[$INDEX]="${DEVELOPER}/Platforms/${platforms[$INDEX]}.platform/Developer/SDKs/MacOSX.sdk"
flags[$INDEX]="clang -target x86_64-apple-macos12.3 -isysroot ${sdk[$INDEX]}"
folder[$INDEX]="build-mac-x86_64"

INDEX=4
platforms[$INDEX]='MacOSX'
sdk[$INDEX]="${DEVELOPER}/Platforms/${platforms[$INDEX]}.platform/Developer/SDKs/MacOSX.sdk"
flags[$INDEX]="clang -target arm64-apple-macos12.3 -isysroot ${sdk[$INDEX]}"
folder[$INDEX]="build-mac-arm64"


for((i=0;i<${#flags[@]};i++));

do

	FOLDER=${folder[i]}
	export CC=${flags[i]}

	echo "Building in $FOLDER"
	OUT=$STAGING/$FOLDER
	
	mkdir -p $OUT
	echo "FOLDER = $OUT"
	
	pushd $SRC/libogg*/
	make clean
	./configure --prefix=$OUT --disable-shared --host=arm-apple-darwin
	make install
	popd
	
	pushd $SRC/libvorbis*/
	make clean
	./configure --prefix=$OUT LDFLAGS="-L$OUT/lib" CPPFLAGS="-I$OUT/include" --host=arm-apple-darwin
	make install
	popd

	libtool -static -o $OUT/lib/vorbis.a $OUT/lib/libogg.a $OUT/lib/libvorbis.a $OUT/lib/libvorbisenc.a  $OUT/lib/libvorbisfile.a
done

pushd $STAGING
lipo -create build-ios-sim-arm64/lib/vorbis.a build-ios-sim-x86_64/lib/vorbis.a -output vorbis-ios-sim.a
lipo -create build-ios-device-armv7/lib/vorbis.a -output vorbis-ios-device.a
lipo -create build-mac-x86_64/lib/vorbis.a build-mac-arm64/lib/vorbis.a -output vorbis-macos.a
xcodebuild -create-xcframework -library vorbis-ios-sim.a -library vorbis-ios-device.a -library vorbis-macos.a -output vorbis.xcframework
popd
