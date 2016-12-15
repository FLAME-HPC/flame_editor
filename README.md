FLAME Editor
============

This branch, v2, was created in the orignal svn repository. It appears that it was an `svn copy` of the original trunk and so there should be a common history with the git master branch. It is expected, therefore that futures merges etc are in theory manageable. 

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
