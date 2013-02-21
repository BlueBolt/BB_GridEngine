#!/bin/bash
#
#
#	basic compiler script for building Grid Engine
#
#

./aimk -no-secure -spool-classic -no-dump -no-gui-inst -no-qmake -only-depend
./aimk -no-secure -spool-classic -no-dump -no-gui-inst -no-qmake depend
./aimk -no-secure -spool-classic -no-dump -no-gui-inst -no-qmake

echo
echo "#-------------- Build Complete ---------------#"
echo "#---------Starting Package Creation ----------#"
echo

export SGE_ROOT=$PWD/DIST/BGE_2011.11p1b1
mkdir -p $SGE_ROOT
scripts/distinst -all -local -noexit

echo "#------------- Package finished --------------#"

