#!/bin/bash

# Variables
QT_464=/usr/bin/qmake-4.6
QT_474=/Applications/QtSDK/Desktop/Qt/474/gcc/bin/qmake
QT_481=/Applications/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
QMAKE_PAR="-spec macx-g++ flame_editor.pro"

test_func () {
	
	QT=${2}
	echo ${1}
	echo "qmake ..."
	$QT $QMAKE_PAR
	# Run make and redirect stderr to file
	echo "make ..."
	make > /dev/null
	echo "run ..."
	#open FLAME\ Editor.app/ --args -test
	FLAME\ Editor.app/Contents/MacOS/FLAME\ Editor -test
	#echo rv: $?
	if [ $? -ne 0 ]
	then
		echo "Error: Cannot open FLAME Editor app."
	#else
		#echo "FLAME Editor app run successfully."
	fi
	#|| {
	#   echo "Error: Cannot open FLAME Editor app."
	#}
	echo "clean up ..."
	make distclean > /dev/null
	
	return 0
}

# Change to project directory
cd ..

#test_func "Test Qt 4.6.4" $QT_464
test_func "Test Qt 4.7.4" $QT_474
test_func "Test Qt 4.8.1" $QT_481

# Change to tests directory
cd tests