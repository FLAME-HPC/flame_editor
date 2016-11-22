#!/bin/sh
# Get the full directory name of the current script regardless of where it is called from
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#TEST1=$DIR"/mac_test_editor.sikuli"
#/Applications/Sikuli-IDE.app/sikuli-ide.sh -r $TEST1

#JYTHONDIR=/usr/local/Cellar/jython/2.5.2/libexec/jython.jar
# Location of sikuli jar file
SIKULI=/Applications/Sikuli-IDE.app/Contents/Resources/Java/sikuli-script.jar
# Name of sikuli script
SCRIPT=mac_test_editor

SCRIPTDIRECTORY=$SCRIPT.sikuli
SCRIPTFILE=$SCRIPT.py
#APP=/Users/stc/workspace/flame/flame_editor/branches/flame_editor-build-desktop/flame_editor.app
APP=/Users/stc/workspace/flame/flame_editor/tags/flame_editor-build-desktop/flame_editor.app
OUTPUT=../flame_editor_test_report.html
# Change directory to run script in same directory as its associated images
cd $SCRIPTDIRECTORY
RUN="java -cp "$SIKULI" org.python.util.jython "$SCRIPTFILE" "$APP" "$OUTPUT""
#echo $RUN
$RUN
cd ..
