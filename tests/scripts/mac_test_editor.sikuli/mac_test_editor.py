from sikuli.Sikuli import *
import unittest
import sys
import os
import platform
# Add HTMLTestRunner to PATH and import
sys.path.append("/Users/stc/workspace/flame/flame_editor/branches/stc/tests/scripts/HTMLTestRunner")
sys.path.append(str(getBundlePath()) + "/../" + "HTMLTestRunner")
import HTMLTestRunner

myOS = str(Env.getOS())
myVer = str(Env.getOSVersion())

if myOS == "MAC" and myVer.startswith("10.7"):
    os = "mac_lion"
elif myOS == "LINUX":
    p = os.popen("uname -a")
    uname = p.readline()
    p.close()
    if 'Ubuntu' in uname:
        p = os.popen("lsb_release -r")
        ver = p.readline()
        p.close()
        if '11.04' in ver:
            os = "ubuntu_11.04"
        else:
            print "Error: no script for Ubuntu ver " + ver
            exit(1)
    else:
        print "Error: no script for Linux dist " + uname
        exit(1)
else:
    print "Error: no script for OS " + myOS + " " + myVer
    exit(1)

print "Using script for OS " + os
# Add image path for current OS
addImagePath(str(getBundlePath()) + "/" + os)

# If run from the test script set parameters from arguments
if len(sys.argv) > 1:
    app = sys.argv[1]
    outfile = open(sys.argv[2], "w")
# If run from Sikuli GUI set parameters internally
else:
    #app = "/home/stc/workspace/flame_editor/stc/flame_editor"
    app = "/Users/stc/workspace/flame/flame_editor/branches/flame_editor-build-desktop/flame_editor.app"
    #app = "/Users/stc/workspace/flame/flame_editor/tags/flame_editor-build-desktop/flame_editor.app"
    #app = "flame_editor"
    outfile = open(getBundlePath() + "/../" + "flame_editor_test_report.html", "w")

# set default wait time out to 10s instead of default 3s
setAutoWaitTimeout(10)

#setShowActions(True)

# Test suite classes
class Create_new_model(unittest.TestCase):
    def setUp(self):
        openApp(app)
        wait("FLAME_Editor_window_title.png")
    
    def tearDown(self):
        closeApp("flame_editor")
        waitVanish("FLAME_Editor_window_title.png")
    
    def _create_new_model(self):
        click("New_button.png")
        wait("text_line_highlighted.png")
        type("testmodel.xml")
        wait("file_dialog_Save_button.png")
        click("file_dialog_Save_button.png")

    def _cancel_new_model(self):
        click("New_button.png")
        wait("Cancel_button.png")
        click("Cancel_button.png")

    def test_cancel_new_model(self):
        self._cancel_new_model()
        self._create_new_model()
        assert exists("one_model_in_models_tree.png")

    def test_create_new_model(self):
        self._create_new_model()
        assert exists("one_model_in_models_tree.png")

class Open_a_model(unittest.TestCase):
    editor = App(app);
    
    def setUp(self):
        self.editor.open()
        openApp(app)
        wait("FLAME_Editor_window_title.png")
    
    def tearDown(self):
        #print "editor.close start"
        #self.editor.close()
        #print "editor.close finish"
        closeApp("flame_editor")
        waitVanish("FLAME_Editor_window_title.png")

    def _open_test_model_folder(self):
        click("Open_button.png")
        wait("Documents.png")
        click("Documents.png")
        click("workspace_folder.png")
        click("a_test_models_folder.png")
        click("conditions.xml_file.png")
        click("file_dialog_Open_button.png")
        wait("FLAME_Editor_window_title.png")
        assert exists("one_model_in_models_tree.png")
            
    def test_open_a_valid_model(self):
        self._open_test_model_folder()
        #click("conditions.xml_file.png")
        #wait("file_dialog_Open_button.png")
        #click("file_dialog_Open_button.png")
        #assert exists("one_model_in_models_tree.png")

# End test suite classes

# Define the root test suite
suite = unittest.TestSuite()
# Add test classes
#suite.addTest(unittest.makeSuite(TestNewModel))
#suite.addTest(unittest.makeSuite(EditorTests))
# Add single test (for development)
suite.addTest(Open_a_model("test_open_a_valid_model"))
# Initialise HTMLTestRunner and execute
runner = HTMLTestRunner.HTMLTestRunner(stream=outfile, title='FLAME Editor Test Report', description='' )
runner.run(suite)
print "Output written to " + outfile.name