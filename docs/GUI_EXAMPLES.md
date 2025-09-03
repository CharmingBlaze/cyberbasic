# CyberBasic GUI System - Example Programs

## Table of Contents
1. [Basic Examples](#basic-examples)
2. [Interactive Examples](#interactive-examples)
3. [Layout Examples](#layout-examples)
4. [Style Examples](#style-examples)
5. [Application Examples](#application-examples)
6. [Game Interface Examples](#game-interface-examples)

## Basic Examples

### Example 1: Hello World GUI
```basic
REM Simple Hello World GUI
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("main", 200, 200, 300, 200, "Hello World")
VAR label = CREATELABEL("hello", 50, 50, 200, 30, "Hello, World!")
VAR button = CREATEBUTTON("close", 100, 100, 100, 30, "Close")

UPDATEGUI()
DRAWGUI()

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 2: Multiple Controls
```basic
REM Multiple Controls Demo
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("main", 100, 100, 400, 300, "Controls Demo")

VAR label = CREATELABEL("label", 20, 20, 100, 20, "Name:")
VAR textbox = CREATETEXTBOX("textbox", 130, 20, 200, 25, "Enter name")
VAR slider = CREATESLIDER("slider", 20, 60, 200, 20, 0, 100, 50)
VAR checkbox = CREATECHECKBOX("checkbox", 20, 100, 150, 20, "I agree", false)
VAR button = CREATEBUTTON("button", 20, 140, 100, 30, "Submit")

UPDATEGUI()
DRAWGUI()

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Interactive Examples

### Example 3: Real-time Calculator
```basic
REM Real-time Calculator
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("calc", 200, 200, 300, 200, "Calculator")

VAR num1_label = CREATELABEL("num1_label", 20, 20, 50, 20, "Number 1:")
VAR num1_textbox = CREATETEXTBOX("num1_textbox", 80, 20, 100, 25, "0")

VAR num2_label = CREATELABEL("num2_label", 20, 60, 50, 20, "Number 2:")
VAR num2_textbox = CREATETEXTBOX("num2_textbox", 80, 60, 100, 25, "0")

VAR result_label = CREATELABEL("result_label", 20, 100, 50, 20, "Result:")
VAR result_display = CREATELABEL("result_display", 80, 100, 100, 20, "0")

VAR operation_combo = CREATECOMBOBOX("operation_combo", 200, 20, 80, 25)
ADDCOMBOBOXITEM(operation_combo, "+")
ADDCOMBOBOXITEM(operation_combo, "-")
ADDCOMBOBOXITEM(operation_combo, "*")
ADDCOMBOBOXITEM(operation_combo, "/")
SETCOMBOBOXSELECTED(operation_combo, 0)

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    VAR num1 = VAL(GETCONTROLTEXT(num1_textbox))
    VAR num2 = VAL(GETCONTROLTEXT(num2_textbox))
    VAR operation = GETCOMBOBOXSELECTED(operation_combo)
    
    VAR result = 0
    IF operation = 0 THEN result = num1 + num2
    IF operation = 1 THEN result = num1 - num2
    IF operation = 2 THEN result = num1 * num2
    IF operation = 3 AND num2 <> 0 THEN result = num1 / num2
    
    SETCONTROLTEXT(result_display, STR(result))
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 4: Color Mixer
```basic
REM Color Mixer
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("mixer", 100, 100, 400, 300, "Color Mixer")

VAR red_label = CREATELABEL("red_label", 20, 20, 50, 20, "Red:")
VAR red_slider = CREATESLIDER("red_slider", 80, 20, 200, 20, 0, 255, 128)
VAR red_value = CREATELABEL("red_value", 290, 20, 50, 20, "128")

VAR green_label = CREATELABEL("green_label", 20, 60, 50, 20, "Green:")
VAR green_slider = CREATESLIDER("green_slider", 80, 60, 200, 20, 0, 255, 128)
VAR green_value = CREATELABEL("green_value", 290, 60, 50, 20, "128")

VAR blue_label = CREATELABEL("blue_label", 20, 100, 50, 20, "Blue:")
VAR blue_slider = CREATESLIDER("blue_slider", 80, 100, 200, 20, 0, 255, 128)
VAR blue_value = CREATELABEL("blue_value", 290, 100, 50, 20, "128")

VAR color_display = CREATEPANEL("color_display", 20, 140, 200, 100)
VAR hex_label = CREATELABEL("hex_label", 20, 250, 200, 20, "Hex: #808080")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    VAR red = GETSLIDERVALUE(red_slider)
    VAR green = GETSLIDERVALUE(green_slider)
    VAR blue = GETSLIDERVALUE(blue_slider)
    
    SETCONTROLTEXT(red_value, STR(red))
    SETCONTROLTEXT(green_value, STR(green))
    SETCONTROLTEXT(blue_value, STR(blue))
    
    VAR hex_color = "#" + HEX(red) + HEX(green) + HEX(blue)
    SETCONTROLTEXT(hex_label, "Hex: " + hex_color)
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Layout Examples

### Example 5: Button Grid
```basic
REM Button Grid Layout
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("grid", 100, 100, 400, 300, "Button Grid")

REM Create buttons
VAR buttons[9]
VAR i = 0
WHILE i < 9
    buttons[i] = CREATEBUTTON("btn" + STR(i), 0, 0, 60, 40, "Btn " + STR(i + 1))
    i = i + 1
ENDWHILE

REM Create grid layout
VAR grid_layout = CREATEGRIDLAYOUT(50, 50, 300, 200, 3, 3)

REM Add buttons to grid
i = 0
WHILE i < 9
    ADDTOLAYOUT(grid_layout, buttons[i])
    i = i + 1
ENDWHILE

UPDATELAYOUT(grid_layout)

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 6: Form Layout
```basic
REM Form Layout
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("form", 100, 100, 400, 500, "Contact Form")

REM Create form controls
VAR name_label = CREATELABEL("name_label", 0, 0, 100, 20, "Name:")
VAR name_textbox = CREATETEXTBOX("name_textbox", 0, 0, 200, 25, "")

VAR email_label = CREATELABEL("email_label", 0, 0, 100, 20, "Email:")
VAR email_textbox = CREATETEXTBOX("email_textbox", 0, 0, 200, 25, "")

VAR phone_label = CREATELABEL("phone_label", 0, 0, 100, 20, "Phone:")
VAR phone_textbox = CREATETEXTBOX("phone_textbox", 0, 0, 200, 25, "")

VAR message_label = CREATELABEL("message_label", 0, 0, 100, 20, "Message:")
VAR message_textbox = CREATETEXTBOX("message_textbox", 0, 0, 200, 100, "")

VAR submit_button = CREATEBUTTON("submit_button", 0, 0, 100, 30, "Submit")
VAR cancel_button = CREATEBUTTON("cancel_button", 0, 0, 100, 30, "Cancel")

REM Create vertical layout
VAR form_layout = CREATEVERTICALLAYOUT(50, 50, 300, 400)

REM Add controls to layout
ADDTOLAYOUT(form_layout, name_label)
ADDTOLAYOUT(form_layout, name_textbox)
ADDTOLAYOUT(form_layout, email_label)
ADDTOLAYOUT(form_layout, email_textbox)
ADDTOLAYOUT(form_layout, phone_label)
ADDTOLAYOUT(form_layout, phone_textbox)
ADDTOLAYOUT(form_layout, message_label)
ADDTOLAYOUT(form_layout, message_textbox)

REM Create horizontal layout for buttons
VAR button_layout = CREATEHORIZONTALLAYOUT(50, 460, 200, 30)
ADDTOLAYOUT(button_layout, submit_button)
ADDTOLAYOUT(button_layout, cancel_button)

UPDATELAYOUT(form_layout)
UPDATELAYOUT(button_layout)

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Style Examples

### Example 7: Style Comparison
```basic
REM Style Comparison
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("styles", 100, 100, 600, 400, "Style Comparison")

REM Default style
VAR default_label = CREATELABEL("default_label", 20, 20, 150, 30, "Default Style")
VAR default_button = CREATEBUTTON("default_button", 20, 60, 100, 30, "Default")
VAR default_textbox = CREATETEXTBOX("default_textbox", 20, 100, 150, 25, "Default text")

REM Dark style
USEDARKSTYLE()
VAR dark_label = CREATELABEL("dark_label", 200, 20, 150, 30, "Dark Style")
VAR dark_button = CREATEBUTTON("dark_button", 200, 60, 100, 30, "Dark")
VAR dark_textbox = CREATETEXTBOX("dark_textbox", 200, 100, 150, 25, "Dark text")

REM Light style
USELIGHTSTYLE()
VAR light_label = CREATELABEL("light_label", 380, 20, 150, 30, "Light Style")
VAR light_button = CREATEBUTTON("light_button", 380, 60, 100, 30, "Light")
VAR light_textbox = CREATETEXTBOX("light_textbox", 380, 100, 150, 25, "Light text")

REM Game style
USEGAMESTYLE()
VAR game_label = CREATELABEL("game_label", 20, 150, 150, 30, "Game Style")
VAR game_button = CREATEBUTTON("game_button", 20, 190, 100, 30, "Game")
VAR game_textbox = CREATETEXTBOX("game_textbox", 20, 230, 150, 25, "Game text")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Application Examples

### Example 8: File Manager
```basic
REM Simple File Manager
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("filemanager", 100, 100, 600, 500, "File Manager")

REM Toolbar
VAR toolbar = CREATEPANEL("toolbar", 0, 0, 600, 40)
VAR back_button = CREATEBUTTON("back_button", 10, 10, 60, 25, "Back")
VAR forward_button = CREATEBUTTON("forward_button", 80, 10, 60, 25, "Forward")
VAR up_button = CREATEBUTTON("up_button", 150, 10, 60, 25, "Up")
VAR refresh_button = CREATEBUTTON("refresh_button", 220, 10, 60, 25, "Refresh")

REM Address bar
VAR address_label = CREATELABEL("address_label", 300, 15, 40, 20, "Path:")
VAR address_textbox = CREATETEXTBOX("address_textbox", 350, 12, 240, 25, "C:\\")

REM File list
VAR file_list = CREATELISTVIEW("file_list", 10, 50, 400, 350)
ADDLISTVIEWITEM(file_list, "Documents")
ADDLISTVIEWITEM(file_list, "Pictures")
ADDLISTVIEWITEM(file_list, "Music")
ADDLISTVIEWITEM(file_list, "Videos")
ADDLISTVIEWITEM(file_list, "Downloads")
ADDLISTVIEWITEM(file_list, "Desktop")
ADDLISTVIEWITEM(file_list, "Program Files")
ADDLISTVIEWITEM(file_list, "Windows")

REM File info panel
VAR info_panel = CREATEPANEL("info_panel", 420, 50, 170, 200)
VAR filename_label = CREATELABEL("filename_label", 425, 55, 160, 20, "File Name:")
VAR filename_text = CREATELABEL("filename_text", 425, 75, 160, 20, "No file selected")
VAR size_label = CREATELABEL("size_label", 425, 100, 160, 20, "Size:")
VAR size_text = CREATELABEL("size_text", 425, 120, 160, 20, "0 bytes")
VAR type_label = CREATELABEL("type_label", 425, 145, 160, 20, "Type:")
VAR type_text = CREATELABEL("type_text", 425, 165, 160, 20, "Unknown")
VAR date_label = CREATELABEL("date_label", 425, 190, 160, 20, "Modified:")
VAR date_text = CREATELABEL("date_text", 425, 210, 160, 20, "Never")

REM Action buttons
VAR open_button = CREATEBUTTON("open_button", 420, 260, 80, 30, "Open")
VAR copy_button = CREATEBUTTON("copy_button", 510, 260, 80, 30, "Copy")
VAR delete_button = CREATEBUTTON("delete_button", 420, 300, 80, 30, "Delete")
VAR rename_button = CREATEBUTTON("rename_button", 510, 300, 80, 30, "Rename")

REM Status bar
VAR status_bar = CREATELABEL("status_bar", 10, 410, 580, 20, "Ready")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    VAR selected = GETLISTVIEWSELECTED(file_list)
    IF selected >= 0 THEN
        VAR file_names[8] = "Documents", "Pictures", "Music", "Videos", "Downloads", "Desktop", "Program Files", "Windows"
        VAR file_sizes[8] = "2.5 GB", "1.2 GB", "850 MB", "3.1 GB", "120 MB", "45 MB", "15.2 GB", "25.8 GB"
        VAR file_types[8] = "Folder", "Folder", "Folder", "Folder", "Folder", "Folder", "Folder", "Folder"
        
        SETCONTROLTEXT(filename_text, file_names[selected])
        SETCONTROLTEXT(size_text, file_sizes[selected])
        SETCONTROLTEXT(type_text, file_types[selected])
        SETCONTROLTEXT(date_text, "Today")
    ENDIF
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 9: Settings Dialog
```basic
REM Settings Dialog
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("settings", 150, 150, 500, 400, "Game Settings")

REM Graphics settings
VAR graphics_label = CREATELABEL("graphics_label", 20, 30, 200, 25, "Graphics Settings")
VAR resolution_label = CREATELABEL("resolution_label", 20, 60, 100, 20, "Resolution:")
VAR resolution_combo = CREATECOMBOBOX("resolution_combo", 130, 60, 150, 25)
ADDCOMBOBOXITEM(resolution_combo, "800x600")
ADDCOMBOBOXITEM(resolution_combo, "1024x768")
ADDCOMBOBOXITEM(resolution_combo, "1280x720")
ADDCOMBOBOXITEM(resolution_combo, "1920x1080")
SETCOMBOBOXSELECTED(resolution_combo, 2)

VAR fullscreen_check = CREATECHECKBOX("fullscreen_check", 20, 90, 200, 20, "Fullscreen", false)
VAR vsync_check = CREATECHECKBOX("vsync_check", 20, 115, 200, 20, "Vertical Sync", true)
VAR antialiasing_check = CREATECHECKBOX("antialiasing_check", 20, 140, 200, 20, "Anti-aliasing", true)

REM Audio settings
VAR audio_label = CREATELABEL("audio_label", 20, 170, 200, 25, "Audio Settings")
VAR master_volume_label = CREATELABEL("master_volume_label", 20, 200, 100, 20, "Master Volume:")
VAR master_volume_slider = CREATESLIDER("master_volume_slider", 130, 200, 200, 20, 0, 100, 80)
VAR master_volume_value = CREATELABEL("master_volume_value", 340, 200, 50, 20, "80%")

VAR music_volume_label = CREATELABEL("music_volume_label", 20, 230, 100, 20, "Music Volume:")
VAR music_volume_slider = CREATESLIDER("music_volume_slider", 130, 230, 200, 20, 0, 100, 70)
VAR music_volume_value = CREATELABEL("music_volume_value", 340, 230, 50, 20, "70%")

VAR sound_volume_label = CREATELABEL("sound_volume_label", 20, 260, 100, 20, "Sound Volume:")
VAR sound_volume_slider = CREATESLIDER("sound_volume_slider", 130, 260, 200, 20, 0, 100, 90)
VAR sound_volume_value = CREATELABEL("sound_volume_value", 340, 260, 50, 20, "90%")

REM Game settings
VAR game_label = CREATELABEL("game_label", 20, 290, 200, 25, "Game Settings")
VAR difficulty_label = CREATELABEL("difficulty_label", 20, 320, 100, 20, "Difficulty:")
VAR difficulty_combo = CREATECOMBOBOX("difficulty_combo", 130, 320, 150, 25)
ADDCOMBOBOXITEM(difficulty_combo, "Easy")
ADDCOMBOBOXITEM(difficulty_combo, "Normal")
ADDCOMBOBOXITEM(difficulty_combo, "Hard")
SETCOMBOBOXSELECTED(difficulty_combo, 1)

REM Buttons
VAR apply_button = CREATEBUTTON("apply_button", 300, 350, 80, 30, "Apply")
VAR cancel_button = CREATEBUTTON("cancel_button", 390, 350, 80, 30, "Cancel")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    VAR master_vol = GETSLIDERVALUE(master_volume_slider)
    VAR music_vol = GETSLIDERVALUE(music_volume_slider)
    VAR sound_vol = GETSLIDERVALUE(sound_volume_slider)
    
    SETCONTROLTEXT(master_volume_value, STR(master_vol) + "%")
    SETCONTROLTEXT(music_volume_value, STR(music_vol) + "%")
    SETCONTROLTEXT(sound_volume_value, STR(sound_vol) + "%")
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Game Interface Examples

### Example 10: Game Menu
```basic
REM Game Main Menu
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("menu", 200, 200, 400, 500, "My Game")

REM Title
VAR title = CREATELABEL("title", 50, 50, 300, 50, "MY AWESOME GAME")

REM Menu buttons
VAR new_game_button = CREATEBUTTON("new_game", 100, 120, 200, 40, "New Game")
VAR load_game_button = CREATEBUTTON("load_game", 100, 170, 200, 40, "Load Game")
VAR settings_button = CREATEBUTTON("settings", 100, 220, 200, 40, "Settings")
VAR multiplayer_button = CREATEBUTTON("multiplayer", 100, 270, 200, 40, "Multiplayer")
VAR credits_button = CREATEBUTTON("credits", 100, 320, 200, 40, "Credits")
VAR exit_button = CREATEBUTTON("exit", 100, 370, 200, 40, "Exit Game")

REM Version info
VAR version_label = CREATELABEL("version", 20, 450, 100, 20, "Version 1.0")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 11: Game HUD
```basic
REM Game HUD Interface
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("hud", 0, 0, 800, 600, "Game HUD")

REM Health bar
VAR health_label = CREATELABEL("health_label", 20, 20, 60, 20, "Health:")
VAR health_bar = CREATEPROGRESSBAR("health_bar", 90, 20, 200, 20, 0, 100, 85)
VAR health_value = CREATELABEL("health_value", 300, 20, 50, 20, "85/100")

REM Mana bar
VAR mana_label = CREATELABEL("mana_label", 20, 50, 60, 20, "Mana:")
VAR mana_bar = CREATEPROGRESSBAR("mana_bar", 90, 50, 200, 20, 0, 100, 60)
VAR mana_value = CREATELABEL("mana_value", 300, 50, 50, 20, "60/100")

REM Experience bar
VAR exp_label = CREATELABEL("exp_label", 20, 80, 60, 20, "EXP:")
VAR exp_bar = CREATEPROGRESSBAR("exp_bar", 90, 80, 200, 20, 0, 100, 45)
VAR exp_value = CREATELABEL("exp_value", 300, 80, 80, 20, "450/1000")

REM Inventory
VAR inventory_panel = CREATEPANEL("inventory", 20, 120, 200, 150)
VAR inventory_label = CREATELABEL("inventory_label", 25, 125, 100, 20, "Inventory")
VAR item1 = CREATEPANEL("item1", 30, 150, 40, 40)
VAR item2 = CREATEPANEL("item2", 80, 150, 40, 40)
VAR item3 = CREATEPANEL("item3", 130, 150, 40, 40)
VAR item4 = CREATEPANEL("item4", 180, 150, 40, 40)

REM Mini-map
VAR minimap_panel = CREATEPANEL("minimap", 580, 20, 180, 180)
VAR minimap_label = CREATELABEL("minimap_label", 585, 25, 100, 20, "Mini-map")

REM Chat
VAR chat_panel = CREATEPANEL("chat", 20, 280, 400, 200)
VAR chat_label = CREATELABEL("chat_label", 25, 285, 100, 20, "Chat")
VAR chat_list = CREATELISTVIEW("chat_list", 25, 310, 390, 120)
ADDLISTVIEWITEM(chat_list, "Player1: Hello everyone!")
ADDLISTVIEWITEM(chat_list, "Player2: How's the game going?")
ADDLISTVIEWITEM(chat_list, "Player3: Great! Just reached level 10")
ADDLISTVIEWITEM(chat_list, "Player1: Nice! I'm still on level 5")
VAR chat_input = CREATETEXTBOX("chat_input", 25, 440, 320, 25, "Type your message...")
VAR send_button = CREATEBUTTON("send_button", 350, 440, 60, 25, "Send")

REM Status
VAR status_label = CREATELABEL("status", 20, 490, 400, 20, "Ready | Level: 5 | Gold: 1250 | Location: Forest")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 12: Character Creation
```basic
REM Character Creation Screen
INITGAMESYSTEMS()
INITGUI()

VAR window = CREATEWINDOW("char_creation", 100, 100, 600, 500, "Character Creation")

REM Character name
VAR name_label = CREATELABEL("name_label", 20, 20, 100, 20, "Character Name:")
VAR name_textbox = CREATETEXTBOX("name_textbox", 130, 20, 200, 25, "Enter name")

REM Character class
VAR class_label = CREATELABEL("class_label", 20, 60, 100, 20, "Class:")
VAR class_combo = CREATECOMBOBOX("class_combo", 130, 60, 150, 25)
ADDCOMBOBOXITEM(class_combo, "Warrior")
ADDCOMBOBOXITEM(class_combo, "Mage")
ADDCOMBOBOXITEM(class_combo, "Rogue")
ADDCOMBOBOXITEM(class_combo, "Cleric")
SETCOMBOBOXSELECTED(class_combo, 0)

REM Character appearance
VAR appearance_label = CREATELABEL("appearance_label", 20, 100, 200, 25, "Character Appearance")

VAR hair_color_label = CREATELABEL("hair_color_label", 20, 130, 100, 20, "Hair Color:")
VAR hair_color_slider = CREATESLIDER("hair_color_slider", 130, 130, 200, 20, 0, 10, 5)

VAR skin_color_label = CREATELABEL("skin_color_label", 20, 160, 100, 20, "Skin Color:")
VAR skin_color_slider = CREATESLIDER("skin_color_slider", 130, 160, 200, 20, 0, 10, 3)

VAR eye_color_label = CREATELABEL("eye_color_label", 20, 190, 100, 20, "Eye Color:")
VAR eye_color_slider = CREATESLIDER("eye_color_slider", 130, 190, 200, 20, 0, 10, 2)

REM Character preview
VAR preview_panel = CREATEPANEL("preview", 350, 20, 200, 300)
VAR preview_label = CREATELABEL("preview_label", 355, 25, 100, 20, "Preview")

REM Character stats
VAR stats_label = CREATELABEL("stats_label", 20, 230, 200, 25, "Character Stats")

VAR strength_label = CREATELABEL("strength_label", 20, 260, 100, 20, "Strength:")
VAR strength_slider = CREATESLIDER("strength_slider", 130, 260, 200, 20, 1, 20, 10)
VAR strength_value = CREATELABEL("strength_value", 340, 260, 30, 20, "10")

VAR intelligence_label = CREATELABEL("intelligence_label", 20, 290, 100, 20, "Intelligence:")
VAR intelligence_slider = CREATESLIDER("intelligence_slider", 130, 290, 200, 20, 1, 20, 10)
VAR intelligence_value = CREATELABEL("intelligence_value", 340, 290, 30, 20, "10")

VAR dexterity_label = CREATELABEL("dexterity_label", 20, 320, 100, 20, "Dexterity:")
VAR dexterity_slider = CREATESLIDER("dexterity_slider", 130, 320, 200, 20, 1, 20, 10)
VAR dexterity_value = CREATELABEL("dexterity_value", 340, 320, 30, 20, "10")

VAR constitution_label = CREATELABEL("constitution_label", 20, 350, 100, 20, "Constitution:")
VAR constitution_slider = CREATESLIDER("constitution_slider", 130, 350, 200, 20, 1, 20, 10)
VAR constitution_value = CREATELABEL("constitution_value", 340, 350, 30, 20, "10")

REM Points remaining
VAR points_label = CREATELABEL("points_label", 20, 380, 200, 20, "Points Remaining: 0")

REM Buttons
VAR create_button = CREATEBUTTON("create_button", 20, 420, 100, 30, "Create")
VAR random_button = CREATEBUTTON("random_button", 130, 420, 100, 30, "Random")
VAR cancel_button = CREATEBUTTON("cancel_button", 240, 420, 100, 30, "Cancel")

WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    VAR strength = GETSLIDERVALUE(strength_slider)
    VAR intelligence = GETSLIDERVALUE(intelligence_slider)
    VAR dexterity = GETSLIDERVALUE(dexterity_slider)
    VAR constitution = GETSLIDERVALUE(constitution_slider)
    
    SETCONTROLTEXT(strength_value, STR(strength))
    SETCONTROLTEXT(intelligence_value, STR(intelligence))
    SETCONTROLTEXT(dexterity_value, STR(dexterity))
    SETCONTROLTEXT(constitution_value, STR(constitution))
    
    VAR total_points = strength + intelligence + dexterity + constitution
    VAR points_remaining = 40 - total_points
    SETCONTROLTEXT(points_label, "Points Remaining: " + STR(points_remaining))
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Tips for Using These Examples

### Getting Started
1. Copy any example code
2. Save it as a `.bas` file
3. Run it with CyberBasic
4. Modify the code to experiment

### Customization
- Change window sizes and positions
- Modify control properties
- Add new controls
- Experiment with different styles

### Learning Path
1. Start with basic examples
2. Move to interactive examples
3. Try layout examples
4. Explore application examples
5. Build your own creations

### Common Modifications
- Change colors and styles
- Add more controls
- Implement actual functionality
- Create your own layouts
- Add event handling

These examples provide a solid foundation for understanding the CyberBasic GUI system. Use them as starting points for your own projects and applications.
