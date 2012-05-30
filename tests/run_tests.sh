#!/bin/bash

# Variables
QT_464=/usr/bin/qmake-4.6
QT_474=/Applications/QtSDK/Desktop/Qt/474/gcc/bin/qmake
QT_481=/Applications/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
QMAKE_PAR="-spec macx-g++ test_flame_editor.pro"

test_func () {
	
	QT=${2}
	echo ${1}
	echo "qmake ..."
	cd qtest
	$QT $QMAKE_PAR
	# Run make and redirect stderr to file
	echo "make ..."
	make > /dev/null
	echo "run ..."
	#open FLAME\ Editor.app/ --args -test
	./test_flame_editor.app/Contents/MacOS/test_flame_editor
	#echo rv: $?
	if [ $? -ne 0 ]
	then
		echo "Error: Cannot open FLAME Editor test."
	#else
		#echo "FLAME Editor test run successfully."
	fi
	#|| {
	#   echo "Error: Cannot open FLAME Editor test."
	#}
	echo "clean up ..."
	make distclean > /dev/null
	cd ..
	
	return 0
}

#test_func "Test Qt 4.6.4" $QT_464
test_func "Test Qt 4.7.4" $QT_474
#test_func "Test Qt 4.8.1" $QT_481
