#!/bin/bash

set -e

#获取运行的程序名
PRONAME=`basename $0`
#获取文件运行的当前目录
#current_dir=$(cd "$(dirname "$0")"; pwd)

cd "$(dirname "$0")"/..
INSTALL_PATH=`pwd`
echo "INSTALL_PATH:$INSTALL_PATH"
export LD_LIBRARY_PATH=$INSTALL_PATH/bin:$INSTALL_PATH/lib:$INSTALL_PATH/lib/`uname -m`-linux-gnu:$LD_LIBRARY_PATH
echo "LD_LIBRARY_PATH:$LD_LIBRARY_PATH"
$INSTALL_PATH/bin/FaceRecognizerApp $*
