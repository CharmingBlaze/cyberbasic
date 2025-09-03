# CyberBasic GUI System - Complete Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Basic Controls](#basic-controls)
4. [Control Properties](#control-properties)
5. [Advanced Features](#advanced-features)
6. [Layout System](#layout-system)
7. [Style System](#style-system)
8. [Event Handling](#event-handling)
9. [Complete Examples](#complete-examples)
10. [Best Practices](#best-practices)
11. [Troubleshooting](#troubleshooting)

## Introduction

The CyberBasic GUI System is a powerful, easy-to-use interface for creating graphical user interfaces in your BASIC programs. Whether you're building game menus, desktop applications, or interactive tools, this system provides everything you need.

### Key Features
- **Easy to Learn**: Simple, intuitive function names
- **Complete Control Set**: Buttons, labels, text boxes, sliders, and more
- **Flexible Layouts**: Automatic positioning and sizing
- **Multiple Styles**: Built-in themes and custom styling
- **Event System**: Handle user interactions
- **Modular Design**: Use only what you need

## Getting Started

### Basic Setup

Every GUI program starts with these essential steps:

```basic
REM Initialize the game systems and GUI
INITGAMESYSTEMS()
INITGUI()

REM Your GUI code goes here

REM Clean up when done
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Your First GUI

Here's a simple example to get you started:

```basic
REM Simple GUI Example
INITGAMESYSTEMS()
INITGUI()

REM Create a window
VAR main_window = CREATEWINDOW("main", 100, 100, 400, 300, "My First GUI")

REM Create a label
VAR title = CREATELABEL("title", 20, 20, 200, 30, "Hello, World!")

REM Create a button
VAR button = CREATEBUTTON("button", 20, 60, 100, 30, "Click Me!")

REM Update and draw the GUI
UPDATEGUI()
DRAWGUI()

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Basic Controls

### Creating Controls

All controls follow the same pattern: `CREATE[CONTROLTYPE](name, x, y, width, height, ...)`

#### Button
```basic
VAR button = CREATEBUTTON("button_name", 100, 100, 120, 30, "Button Text")
```

#### Label
```basic
VAR label = CREATELABEL("label_name", 100, 100, 200, 20, "Label Text")
```

#### Text Box
```basic
VAR textbox = CREATETEXTBOX("textbox_name", 100, 100, 200, 25, "Default Text")
```

#### Slider
```basic
VAR slider = CREATESLIDER("slider_name", 100, 100, 200, 20, 0, 100, 50)
REM Parameters: name, x, y, width, height, min_value, max_value, current_value
```

#### Check Box
```basic
VAR checkbox = CREATECHECKBOX("checkbox_name", 100, 100, 150, 20, "Checkbox Text", false)
REM Parameters: name, x, y, width, height, text, checked
```

#### Combo Box
```basic
VAR combobox = CREATECOMBOBOX("combobox_name", 100, 100, 150, 25)
ADDCOMBOBOXITEM(combobox, "Option 1")
ADDCOMBOBOXITEM(combobox, "Option 2")
ADDCOMBOBOXITEM(combobox, "Option 3")
```

#### Progress Bar
```basic
VAR progress = CREATEPROGRESSBAR("progress_name", 100, 100, 200, 20, 0, 100, 0)
REM Parameters: name, x, y, width, height, min_value, max_value, current_value
```

#### List View
```basic
VAR listview = CREATELISTVIEW("listview_name", 100, 100, 200, 150)
ADDLISTVIEWITEM(listview, "Item 1")
ADDLISTVIEWITEM(listview, "Item 2")
ADDLISTVIEWITEM(listview, "Item 3")
```

#### Window
```basic
VAR window = CREATEWINDOW("window_name", 100, 100, 400, 300, "Window Title")
```

#### Panel
```basic
VAR panel = CREATEPANEL("panel_name", 100, 100, 200, 150)
```

## Control Properties

### Getting and Setting Properties

All controls support these basic properties:

#### Text Properties
```basic
REM Set text
SETCONTROLTEXT(control_id, "New Text")

REM Get text
VAR current_text = GETCONTROLTEXT(control_id)
```

#### Value Properties
```basic
REM Set value (for sliders, progress bars, etc.)
SETCONTROLVALUE(control_id, "50")

REM Get value
VAR current_value = GETCONTROLVALUE(control_id)
```

#### Position and Size
```basic
REM Set position
SETCONTROLPOSITION(control_id, 200, 150)

REM Set size
SETCONTROLSIZE(control_id, 300, 50)
```

#### Visibility and State
```basic
REM Show/hide control
SETCONTROLVISIBLE(control_id, true)
SETCONTROLVISIBLE(control_id, false)

REM Enable/disable control
SETCONTROLENABLED(control_id, true)
SETCONTROLENABLED(control_id, false)
```

### Specialized Properties

#### Check Box
```basic
REM Check/uncheck
SETCHECKBOXCHECKED(checkbox_id, true)
SETCHECKBOXCHECKED(checkbox_id, false)

REM Get checked state
VAR is_checked = ISCHECKBOXCHECKED(checkbox_id)
```

#### Slider
```basic
REM Set slider value
SETSLIDERVALUE(slider_id, 75.5)

REM Get slider value
VAR slider_value = GETSLIDERVALUE(slider_id)
```

#### Progress Bar
```basic
REM Set progress value
SETPROGRESSBARVALUE(progress_id, 60.0)

REM Get progress value
VAR progress_value = GETPROGRESSBARVALUE(progress_id)
```

#### Combo Box
```basic
REM Set selected item
SETCOMBOBOXSELECTED(combobox_id, 2)

REM Get selected item index
VAR selected_index = GETCOMBOBOXSELECTED(combobox_id)

REM Clear all items
CLEARCOMBOBOXITEMS(combobox_id)
```

#### List View
```basic
REM Set selected item
SETLISTVIEWSELECTED(listview_id, 1)

REM Get selected item index
VAR selected_index = GETLISTVIEWSELECTED(listview_id)

REM Clear all items
CLEARLISTVIEWITEMS(listview_id)
```

## Advanced Features

### Control Management

#### Destroying Controls
```basic
REM Destroy a specific control
DESTROYCONTROL(control_id)

REM Destroy all controls
DESTROYALLCONTROLS()
```

#### Control Updates
```basic
REM Update all controls (call this in your main loop)
UPDATEGUI()

REM Draw all controls (call this in your main loop)
DRAWGUI()
```

### Working with Multiple Controls

#### Using Arrays
```basic
REM Create multiple buttons
VAR buttons[5]
VAR i = 0
WHILE i < 5
    buttons[i] = CREATEBUTTON("button" + STR(i), 20, 20 + i * 40, 100, 30, "Button " + STR(i))
    i = i + 1
ENDWHILE
```

#### Dynamic Control Creation
```basic
REM Create controls based on data
VAR items[3] = "Apple", "Banana", "Orange"
VAR i = 0
WHILE i < 3
    VAR label = CREATELABEL("item" + STR(i), 20, 20 + i * 25, 100, 20, items[i])
    i = i + 1
ENDWHILE
```

## Layout System

The layout system automatically positions and sizes controls for you.

### Horizontal Layout
```basic
REM Create horizontal layout
VAR layout = CREATEHORIZONTALLAYOUT(100, 100, 300, 50)

REM Add controls to layout
ADDTOLAYOUT(layout, button1)
ADDTOLAYOUT(layout, button2)
ADDTOLAYOUT(layout, button3)

REM Update layout
UPDATELAYOUT(layout)
```

### Vertical Layout
```basic
REM Create vertical layout
VAR layout = CREATEVERTICALLAYOUT(100, 100, 200, 200)

REM Add controls to layout
ADDTOLAYOUT(layout, label1)
ADDTOLAYOUT(layout, textbox1)
ADDTOLAYOUT(layout, button1)

REM Update layout
UPDATELAYOUT(layout)
```

### Grid Layout
```basic
REM Create 3x2 grid layout
VAR layout = CREATEGRIDLAYOUT(100, 100, 300, 200, 3, 2)

REM Add controls to grid
ADDTOLAYOUT(layout, button1)
ADDTOLAYOUT(layout, button2)
ADDTOLAYOUT(layout, button3)
ADDTOLAYOUT(layout, button4)
ADDTOLAYOUT(layout, button5)
ADDTOLAYOUT(layout, button6)

REM Update layout
UPDATELAYOUT(layout)
```

## Style System

Change the appearance of your GUI with built-in styles.

### Available Styles
```basic
REM Use default style
USEDEFAULTSTYLE()

REM Use dark theme
USEDARKSTYLE()

REM Use light theme
USELIGHTSTYLE()

REM Use game theme
USEGAMESTYLE()
```

### Style Examples
```basic
REM Create controls with default style
VAR button1 = CREATEBUTTON("button1", 20, 20, 100, 30, "Default")

REM Switch to dark style
USEDARKSTYLE()
VAR button2 = CREATEBUTTON("button2", 20, 60, 100, 30, "Dark")

REM Switch to light style
USELIGHTSTYLE()
VAR button3 = CREATEBUTTON("button3", 20, 100, 100, 30, "Light")

REM Switch to game style
USEGAMESTYLE()
VAR button4 = CREATEBUTTON("button4", 20, 140, 100, 30, "Game")
```

## Event Handling

While the current system focuses on control creation and management, you can handle user interactions by checking control states and values.

### Basic Event Handling
```basic
REM Main program loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Update GUI
    UPDATEGUI()
    
    REM Check for user interactions
    VAR slider_value = GETSLIDERVALUE(slider_id)
    VAR checkbox_state = ISCHECKBOXCHECKED(checkbox_id)
    VAR selected_item = GETLISTVIEWSELECTED(listview_id)
    
    REM Update other controls based on interactions
    SETPROGRESSBARVALUE(progress_id, slider_value)
    
    REM Draw GUI
    DRAWGUI()
    
    REM Small delay
    SLEEP(16)
ENDWHILE
```

## Complete Examples

### Example 1: Simple Calculator Interface
```basic
REM Simple Calculator GUI
INITGAMESYSTEMS()
INITGUI()

REM Create main window
VAR calc_window = CREATEWINDOW("calculator", 200, 200, 300, 400, "Calculator")

REM Create display
VAR display = CREATETEXTBOX("display", 20, 30, 260, 40, "0")

REM Create number buttons
VAR btn7 = CREATEBUTTON("btn7", 20, 90, 60, 50, "7")
VAR btn8 = CREATEBUTTON("btn8", 90, 90, 60, 50, "8")
VAR btn9 = CREATEBUTTON("btn9", 160, 90, 60, 50, "9")
VAR btn_div = CREATEBUTTON("btn_div", 230, 90, 50, 50, "/")

VAR btn4 = CREATEBUTTON("btn4", 20, 150, 60, 50, "4")
VAR btn5 = CREATEBUTTON("btn5", 90, 150, 60, 50, "5")
VAR btn6 = CREATEBUTTON("btn6", 160, 150, 60, 50, "6")
VAR btn_mul = CREATEBUTTON("btn_mul", 230, 150, 50, 50, "*")

VAR btn1 = CREATEBUTTON("btn1", 20, 210, 60, 50, "1")
VAR btn2 = CREATEBUTTON("btn2", 90, 210, 60, 50, "2")
VAR btn3 = CREATEBUTTON("btn3", 160, 210, 60, 50, "3")
VAR btn_sub = CREATEBUTTON("btn_sub", 230, 210, 50, 50, "-")

VAR btn0 = CREATEBUTTON("btn0", 20, 270, 130, 50, "0")
VAR btn_dot = CREATEBUTTON("btn_dot", 160, 270, 60, 50, ".")
VAR btn_add = CREATEBUTTON("btn_add", 230, 270, 50, 50, "+")

VAR btn_clear = CREATEBUTTON("btn_clear", 20, 330, 130, 50, "Clear")
VAR btn_equals = CREATEBUTTON("btn_equals", 160, 330, 120, 50, "=")

REM Update and draw
UPDATEGUI()
DRAWGUI()

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 2: Settings Dialog
```basic
REM Settings Dialog
INITGAMESYSTEMS()
INITGUI()

REM Create settings window
VAR settings_window = CREATEWINDOW("settings", 150, 150, 500, 400, "Game Settings")

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

REM Audio settings
VAR audio_label = CREATELABEL("audio_label", 20, 150, 200, 25, "Audio Settings")
VAR master_volume_label = CREATELABEL("master_volume_label", 20, 180, 100, 20, "Master Volume:")
VAR master_volume_slider = CREATESLIDER("master_volume_slider", 130, 180, 200, 20, 0, 100, 80)
VAR music_volume_label = CREATELABEL("music_volume_label", 20, 210, 100, 20, "Music Volume:")
VAR music_volume_slider = CREATESLIDER("music_volume_slider", 130, 210, 200, 20, 0, 100, 70)
VAR sound_volume_label = CREATELABEL("sound_volume_label", 20, 240, 100, 20, "Sound Volume:")
VAR sound_volume_slider = CREATESLIDER("sound_volume_slider", 130, 240, 200, 20, 0, 100, 90)

REM Game settings
VAR game_label = CREATELABEL("game_label", 20, 280, 200, 25, "Game Settings")
VAR difficulty_label = CREATELABEL("difficulty_label", 20, 310, 100, 20, "Difficulty:")
VAR difficulty_combo = CREATECOMBOBOX("difficulty_combo", 130, 310, 150, 25)
ADDCOMBOBOXITEM(difficulty_combo, "Easy")
ADDCOMBOBOXITEM(difficulty_combo, "Normal")
ADDCOMBOBOXITEM(difficulty_combo, "Hard")
SETCOMBOBOXSELECTED(difficulty_combo, 1)

REM Buttons
VAR apply_button = CREATEBUTTON("apply_button", 300, 350, 80, 30, "Apply")
VAR cancel_button = CREATEBUTTON("cancel_button", 390, 350, 80, 30, "Cancel")

REM Update and draw
UPDATEGUI()
DRAWGUI()

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Example 3: File Manager Interface
```basic
REM File Manager Interface
INITGAMESYSTEMS()
INITGUI()

REM Create main window
VAR filemanager_window = CREATEWINDOW("filemanager", 100, 100, 600, 500, "File Manager")

REM Create toolbar
VAR toolbar = CREATEPANEL("toolbar", 0, 0, 600, 40)
VAR back_button = CREATEBUTTON("back_button", 10, 10, 60, 25, "Back")
VAR forward_button = CREATEBUTTON("forward_button", 80, 10, 60, 25, "Forward")
VAR up_button = CREATEBUTTON("up_button", 150, 10, 60, 25, "Up")
VAR refresh_button = CREATEBUTTON("refresh_button", 220, 10, 60, 25, "Refresh")

REM Create address bar
VAR address_label = CREATELABEL("address_label", 300, 15, 60, 20, "Path:")
VAR address_textbox = CREATETEXTBOX("address_textbox", 370, 12, 220, 25, "C:\\")

REM Create file list
VAR file_list = CREATELISTVIEW("file_list", 10, 50, 400, 350)
ADDLISTVIEWITEM(file_list, "Documents")
ADDLISTVIEWITEM(file_list, "Pictures")
ADDLISTVIEWITEM(file_list, "Music")
ADDLISTVIEWITEM(file_list, "Videos")
ADDLISTVIEWITEM(file_list, "Downloads")
ADDLISTVIEWITEM(file_list, "Desktop")

REM Create file info panel
VAR info_panel = CREATEPANEL("info_panel", 420, 50, 170, 200)
VAR filename_label = CREATELABEL("filename_label", 425, 55, 160, 20, "File Name:")
VAR filename_text = CREATELABEL("filename_text", 425, 75, 160, 20, "No file selected")
VAR size_label = CREATELABEL("size_label", 425, 100, 160, 20, "Size:")
VAR size_text = CREATELABEL("size_text", 425, 120, 160, 20, "0 bytes")
VAR type_label = CREATELABEL("type_label", 425, 145, 160, 20, "Type:")
VAR type_text = CREATELABEL("type_text", 425, 165, 160, 20, "Unknown")
VAR date_label = CREATELABEL("date_label", 425, 190, 160, 20, "Modified:")
VAR date_text = CREATELABEL("date_text", 425, 210, 160, 20, "Never")

REM Create action buttons
VAR open_button = CREATEBUTTON("open_button", 420, 260, 80, 30, "Open")
VAR copy_button = CREATEBUTTON("copy_button", 510, 260, 80, 30, "Copy")
VAR delete_button = CREATEBUTTON("delete_button", 420, 300, 80, 30, "Delete")
VAR rename_button = CREATEBUTTON("rename_button", 510, 300, 80, 30, "Rename")

REM Create status bar
VAR status_bar = CREATELABEL("status_bar", 10, 410, 580, 20, "Ready")

REM Update and draw
UPDATEGUI()
DRAWGUI()

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Best Practices

### 1. Organization
- Use descriptive names for controls
- Group related controls together
- Use consistent spacing and alignment

### 2. User Experience
- Make buttons large enough to click easily
- Use clear, descriptive labels
- Provide visual feedback for user actions

### 3. Performance
- Only update controls when necessary
- Use layouts for automatic positioning
- Clean up controls when no longer needed

### 4. Code Structure
```basic
REM Good structure example
INITGAMESYSTEMS()
INITGUI()

REM Create main window
VAR main_window = CREATEWINDOW("main", 100, 100, 500, 400, "My Application")

REM Create controls section by section
REM Header section
VAR title = CREATELABEL("title", 20, 20, 200, 30, "Application Title")

REM Input section
VAR input_label = CREATELABEL("input_label", 20, 60, 100, 20, "Input:")
VAR input_textbox = CREATETEXTBOX("input_textbox", 130, 60, 200, 25, "")

REM Button section
VAR submit_button = CREATEBUTTON("submit_button", 20, 100, 100, 30, "Submit")
VAR cancel_button = CREATEBUTTON("cancel_button", 130, 100, 100, 30, "Cancel")

REM Main program loop
WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    REM Handle user interactions here
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

## Troubleshooting

### Common Issues

#### Controls Not Appearing
- Make sure you call `UPDATEGUI()` and `DRAWGUI()` in your main loop
- Check that controls are positioned within the window bounds
- Verify that controls are visible: `SETCONTROLVISIBLE(control_id, true)`

#### Controls Not Responding
- Ensure controls are enabled: `SETCONTROLENABLED(control_id, true)`
- Check that you're calling `UPDATEGUI()` regularly
- Verify control IDs are correct

#### Layout Issues
- Call `UPDATELAYOUT(layout_id)` after adding controls to layouts
- Make sure layout bounds are large enough for all controls
- Check that controls are added to the correct layout

#### Style Problems
- Apply styles before creating controls for best results
- Some styles may not affect all control types
- Try switching between different built-in styles

### Debugging Tips

#### Check Control Properties
```basic
REM Debug control properties
VAR control_text = GETCONTROLTEXT(control_id)
PRINT "Control text: " + control_text

VAR control_value = GETCONTROLVALUE(control_id)
PRINT "Control value: " + control_value
```

#### Verify Control Creation
```basic
REM Check if control was created successfully
IF control_id > 0 THEN
    PRINT "Control created successfully with ID: " + STR(control_id)
ELSE
    PRINT "Failed to create control"
ENDIF
```

#### Test with Simple Examples
Start with the basic examples and gradually add complexity. This helps identify where issues occur.

### Getting Help

If you encounter issues not covered in this guide:

1. Check the example programs in the `examples/` directory
2. Verify your syntax matches the examples exactly
3. Test with minimal code to isolate the problem
4. Make sure all required systems are initialized properly

Remember: The GUI system is designed to be simple and intuitive. Most issues can be resolved by following the examples and best practices outlined in this guide.
