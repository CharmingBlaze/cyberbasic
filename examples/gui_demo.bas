REM Comprehensive GUI Demo - Shows how easy it is to create full GUI applications
INITGAMESYSTEMS()
INITGUI()

PRINT "=== CyberBasic GUI System Demo ==="

REM Create a main window
VAR main_window = CREATEWINDOW("main", 50, 50, 600, 400, "CyberBasic GUI Demo")

REM Create controls
VAR title_label = CREATELABEL("title", 20, 20, 200, 30, "Welcome to CyberBasic GUI!")
VAR name_label = CREATELABEL("name_label", 20, 60, 100, 20, "Name:")
VAR name_textbox = CREATETEXTBOX("name_textbox", 130, 60, 200, 25, "Enter your name")

VAR age_label = CREATELABEL("age_label", 20, 100, 100, 20, "Age:")
VAR age_slider = CREATESLIDER("age_slider", 130, 100, 200, 20, 0, 100, 25)

VAR options_label = CREATELABEL("options_label", 20, 140, 100, 20, "Options:")
VAR music_checkbox = CREATECHECKBOX("music_checkbox", 130, 140, 150, 20, "Enable Music", true)
VAR sound_checkbox = CREATECHECKBOX("sound_checkbox", 130, 170, 150, 20, "Enable Sound", true)

VAR difficulty_label = CREATELABEL("difficulty_label", 20, 210, 100, 20, "Difficulty:")
VAR difficulty_combo = CREATECOMBOBOX("difficulty_combo", 130, 210, 150, 25)
ADDCOMBOBOXITEM(difficulty_combo, "Easy")
ADDCOMBOBOXITEM(difficulty_combo, "Medium")
ADDCOMBOBOXITEM(difficulty_combo, "Hard")
SETCOMBOBOXSELECTED(difficulty_combo, 1)

VAR progress_label = CREATELABEL("progress_label", 20, 250, 100, 20, "Progress:")
VAR progress_bar = CREATEPROGRESSBAR("progress_bar", 130, 250, 200, 20, 0, 100, 0)

VAR items_label = CREATELABEL("items_label", 20, 290, 100, 20, "Items:")
VAR items_list = CREATELISTVIEW("items_list", 130, 290, 200, 80)
ADDLISTVIEWITEM(items_list, "Sword")
ADDLISTVIEWITEM(items_list, "Shield")
ADDLISTVIEWITEM(items_list, "Potion")
ADDLISTVIEWITEM(items_list, "Key")

REM Create buttons
VAR save_button = CREATEBUTTON("save_button", 400, 60, 100, 30, "Save")
VAR load_button = CREATEBUTTON("load_button", 400, 100, 100, 30, "Load")
VAR reset_button = CREATEBUTTON("reset_button", 400, 140, 100, 30, "Reset")
VAR exit_button = CREATEBUTTON("exit_button", 400, 180, 100, 30, "Exit")

REM Create a status panel
VAR status_panel = CREATEPANEL("status_panel", 20, 380, 560, 20)
VAR status_label = CREATELABEL("status_label", 25, 385, 550, 15, "Ready")

PRINT "GUI controls created successfully!"

REM Demo different styles
PRINT "Testing different GUI styles..."
USEDARKSTYLE()
PRINT "Applied dark style"

USELIGHTSTYLE()
PRINT "Applied light style"

USEGAMESTYLE()
PRINT "Applied game style"

USEDEFAULTSTYLE()
PRINT "Applied default style"

REM Demo control manipulation
PRINT "Testing control manipulation..."
SETCONTROLTEXT(title_label, "Updated Title!")
SETCONTROLVALUE(name_textbox, "John Doe")
SETSLIDERVALUE(age_slider, 30)
SETCHECKBOXCHECKED(music_checkbox, false)
SETPROGRESSBARVALUE(progress_bar, 75)

PRINT "Control values updated!"

REM Demo layout system
PRINT "Testing layout system..."
VAR layout = CREATEHORIZONTALLAYOUT(350, 250, 200, 50)
ADDTOLAYOUT(layout, save_button)
ADDTOLAYOUT(layout, load_button)
UPDATELAYOUT(layout)

PRINT "Layout created and updated!"

REM Update GUI and draw
UPDATEGUI()
DRAWGUI()

PRINT "GUI updated and drawn!"

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()

PRINT "=== GUI Demo Completed Successfully! ==="
