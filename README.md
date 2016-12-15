FLAME Editor
============
This repository has been migrated from svn to git. 

This branch was created in the orignal svn repository. It has not, therefore, been "branched" from another branch. This means branch merges will not function as expected as there will be no common history.

LINUX
-----
To compile you will need Qt4.
The package for Ubuntu is called: libqt4-dev

To compile type:

	`qmake flame_editor.pro`
	`make`

To run:

	`./flame_editor`

MAC OSX
-------
To compile you will need Qt4.
Download from the Qt web site.
Normally qmake creates xcode projects on mac.
To stop this use the -spec flag below.

To compile type:

	`qmake -spec macx-g++ flame_editor.pro`
	`make`

To run:

	`open flame_editor.app`

If you have any problems try using Qt Creator.

WINDOWS
-----------
To compile you will need Qt4.
Download from the Qt web site.
Use Qt Creator to open the project flame_editor.pro and compile.
