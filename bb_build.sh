#!/bin/bash
#
#
#	basic compiler script for building Grid Engine
#
#
dir=`dirname $0`

# change this if the 3rdparty dependices are in a different location
# if you do change it make sure you update the junit class path in 
# source/build.properties as well.
export GE_DEPENDECIES_PATH=$(readlink -f ../BB_GridEngine_Dependencies) 

export JAVA_HOME=${GE_DEPENDECIES_PATH}/jdk1.6.0_39
export ANT_HOME=${GE_DEPENDECIES_PATH}/apache-ant-1.8.4
export CLASSPATH=${GE_DEPENDECIES_PATH}/junit-4.10.jar:${ANT_HOME}/lib/ant.jar
export JUNIT_JAR=${GE_DEPENDECIES_PATH}/junit-4.10.jar
export PATH=$PATH:$JAVA_HOME/bin:$ANT_HOME/bin

cd "$dir/source"
./aimk -no-gui-inst -only-depend 
./scripts/zerodepend

./aimk -no-gui-inst depend
./aimk -no-gui-inst
# comment the above two lines and uncoment the bleow lines to compile as shared libs
# ./aimk -no-gui-inst -shared-libs depend
# ./aimk -no-gui-inst -shared-libs

echo
echo "#-------------- Build Complete ---------------#"
echo "#--------- Starting Package Creation ----------#"
echo

version=`cat libs/gdi/version.c |grep "char GDI_VERSION"|awk -F "[\",]" '{print $2}'`

export SGE_ROOT=$PWD/../DIST/$version
mkdir -p $SGE_ROOT
scripts/distinst -allall -local -noexit
cp -rf ../docs $SGE_ROOT/
scripts/mk_dist -vdir $SGE_ROOT -version BBGE_$version -basedir $PWD/../DIST/ -common -bin linux-x64

echo "#------------- Package Finished --------------#"

