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

# -p, --sign-command=sign-command
#  When dpkg-buildpackage needs to execute GPG to sign a source
#  control (.dsc) file or a .changes file it will run sign-command
#  (searching the PATH if necessary) instead of gpg (long option since
#  dpkg 1.18.8).  sign-command will get all the arguments that gpg
#  would have gotten. sign-command should not contain spaces or any
#  other shell metacharacters.

# -k, --sign-key=key-id
#  Specify a key-ID to use when signing packages (long option since
#  dpkg 1.18.8).

# -us, --unsigned-source
#  Do not sign the source package (long option since dpkg 1.18.8).

# -ui, --unsigned-buildinfo
#  Do not sign the .buildinfo file (since dpkg 1.18.19).

# -uc, --unsigned-changes
#  Do not sign the .buildinfo and .changes files (long option since
#  dpkg 1.18.8).

# -b  Equivalent to --build=binary or --build=any,all.
# -S  Equivalent to --build=source
# -d, --no-check-builddeps    do not check build dependencies and conflicts.
#      --ignore-builtin-builddeps
#                              do not check builtin build dependencies.

#The -us -uc tell it there is no need to GPG sign the package. the -b is build binary
dpkg-buildpackage -us -uc -b -d

#The -us -uc tell it there is no need to GPG sign the package. the -S is build source package
#dpkg-buildpackage -us -uc -S

#dpkg-buildpackage -S

# build source and binary package
#dpkg-buildpackage -us -uc -d

#dpkg-buildpackage -d
