#!/bin/bash
#
#
#	basic compiler script for building Grid Engine
#
#
dir=`dirname $0`
#JAVA_HOME=/net/pinot/disk1/playpen/dev/OpenGridScheduler/3rdparty/jdk1.6.0_39
cd "$dir/source"
./aimk -no-secure -spool-classic -no-dump -no-gui-inst -only-depend
./scripts/zerodepend
./aimk -no-secure -spool-classic -no-dump -no-gui-inst depend
./aimk -no-secure -spool-classic -no-dump -no-gui-inst 

echo
echo "#-------------- Build Complete ---------------#"
echo "#---------Starting Package Creation ----------#"
echo

version=`cat libs/gdi/version.c |grep "char GDI_VERSION"|awk -F "[\",]" '{print $2}'`

export SGE_ROOT=$PWD/../DIST/$version
mkdir -p $SGE_ROOT
scripts/distinst -all -local -noexit

echo "#------------- Package finished --------------#"

