#!/bin/bash

function help()
{
    echo "Usage: $0 QT_ROOT ThirdLibs_DIR RabbitCommon_DIR ENABLE_DOWNLOAD_MODULE"
    echo "Error message: $1"
    exit -1
}

if [ -n "$1" -a -z "$QT_ROOT" ]; then
    QT_ROOT=$1
fi

if [ -n "$2" ]; then
    export ThirdLibs_DIR=$2
fi

if [ -n "${ThirdLibs_DIR}" ]; then
    export ThirdLibs_DIR=${ThirdLibs_DIR}
    export PKG_CONFIG_PATH=${ThirdLibs_DIR}/lib/pkgconfig
fi

if [ -n "$3" -a -z "$RabbitCommon_DIR" ]; then
    RabbitCommon_DIR=$3
fi

if [ -z "$RabbitCommon_DIR" ]; then
    RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
    help "RabbitCommon_DIR don't exist: $RabbitCommon_DIR"
fi

if [ -n "$4" ]; then
    export ENABLE_DOWNLOAD_MODULE=$4
fi
if [ -z "$ENABLE_DOWNLOAD_MODULE" ]; then
    export ENABLE_DOWNLOAD_MODULE=OFF
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
#fakeroot debian/rules binary
#dpkg-buildpackage -us -uc -b  #The -us -uc tell it there is no need to GPG sign the package. the -b is build binary
dpkg-buildpackage -us -uc   #The -us -uc tell it there is no need to GPG sign the package

