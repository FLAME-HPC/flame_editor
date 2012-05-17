# Only deploy on fresh builds once
echo "Deploying Qt app"
macdeployqt FLAME\ Editor.app/
mkdir FLAME\ Editor
cp -r FLAME\ Editor.app/ FLAME\ Editor/FLAME\ Editor.app
ln -s /Applications/ FLAME\ Editor/Applications
echo "Creating dmg"
hdiutil create -format UDBZ -quiet -srcfolder FLAME\ Editor FLAME\ Editor.dmg
rm -rf FLAME\ Editor
echo "Finished"