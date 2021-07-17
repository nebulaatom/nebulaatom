#!/bin/bash

cd ../../

# Creating directories
if [ -e build ]; then
	cd build
else
	mkdir build && cd build
fi
if [ -e AppImage ]; then
	cd AppImage
else
	mkdir AppImage && cd AppImage
fi
if [ ! -e AppDir ]; then
	mkdir AppDir && cp ../../packaging/AppImage/AppRun AppDir/
else
	cp ../../packaging/AppImage/AppRun AppDir/
fi
chmod 775 AppDir/AppRun

cmake ../../ \
	-Dproject-name_CONAN=ON \
	-Dproject-name_APPIMAGE=ON \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=AppDir/usr
cmake --build . --target install

if [ -e linuxdeploy-x86_64.AppImage ]; then
	linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage && echo "Your AppImage is on ${PWD}"
else
	wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
	chmod 775 linuxdeploy-x86_64.AppImage
	./linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage && echo "Your AppImage is on ${PWD}"
fi
