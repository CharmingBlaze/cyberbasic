# CyberBasic GUI System - Quick Reference

## System Functions

| Function | Description | Parameters |
|----------|-------------|------------|
| `INITGUI()` | Initialize GUI system | None |
| `SHUTDOWNGUI()` | Shutdown GUI system | None |
| `UPDATEGUI()` | Update all controls | None |
| `DRAWGUI()` | Draw all controls | None |

## Control Creation

### Basic Controls

| Function | Description | Parameters |
|----------|-------------|------------|
| `CREATEBUTTON(name, x, y, width, height, text)` | Create button | name, x, y, width, height, text |
| `CREATELABEL(name, x, y, width, height, text)` | Create label | name, x, y, width, height, text |
| `CREATETEXTBOX(name, x, y, width, height, text)` | Create text box | name, x, y, width, height, text |
| `CREATESLIDER(name, x, y, width, height, min, max, current)` | Create slider | name, x, y, width, height, min, max, current |
| `CREATECHECKBOX(name, x, y, width, height, text, checked)` | Create checkbox | name, x, y, width, height, text, checked |
| `CREATECOMBOBOX(name, x, y, width, height)` | Create combo box | name, x, y, width, height |
| `CREATEPROGRESSBAR(name, x, y, width, height, min, max, current)` | Create progress bar | name, x, y, width, height, min, max, current |
| `CREATELISTVIEW(name, x, y, width, height)` | Create list view | name, x, y, width, height |
| `CREATEWINDOW(name, x, y, width, height, title)` | Create window | name, x, y, width, height, title |
| `CREATEPANEL(name, x, y, width, height)` | Create panel | name, x, y, width, height |

## Control Properties

### Text Properties

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETCONTROLTEXT(control_id, text)` | Set control text | control_id, text |
| `GETCONTROLTEXT(control_id)` | Get control text | control_id |

### Value Properties

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETCONTROLVALUE(control_id, value)` | Set control value | control_id, value |
| `GETCONTROLVALUE(control_id)` | Get control value | control_id |

### Position and Size

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETCONTROLPOSITION(control_id, x, y)` | Set control position | control_id, x, y |
| `SETCONTROLSIZE(control_id, width, height)` | Set control size | control_id, width, height |

### Visibility and State

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETCONTROLVISIBLE(control_id, visible)` | Set control visibility | control_id, visible (true/false) |
| `SETCONTROLENABLED(control_id, enabled)` | Set control enabled state | control_id, enabled (true/false) |

## Specialized Control Functions

### Check Box

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETCHECKBOXCHECKED(control_id, checked)` | Set checkbox state | control_id, checked (true/false) |
| `ISCHECKBOXCHECKED(control_id)` | Get checkbox state | control_id |

### Slider

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETSLIDERVALUE(control_id, value)` | Set slider value | control_id, value (number) |
| `GETSLIDERVALUE(control_id)` | Get slider value | control_id |

### Progress Bar

| Function | Description | Parameters |
|----------|-------------|------------|
| `SETPROGRESSBARVALUE(control_id, value)` | Set progress bar value | control_id, value (number) |
| `GETPROGRESSBARVALUE(control_id)` | Get progress bar value | control_id |

### Combo Box

| Function | Description | Parameters |
|----------|-------------|------------|
| `ADDCOMBOBOXITEM(control_id, item)` | Add item to combo box | control_id, item (text) |
| `CLEARCOMBOBOXITEMS(control_id)` | Clear all combo box items | control_id |
| `SETCOMBOBOXSELECTED(control_id, index)` | Set selected item | control_id, index (number) |
| `GETCOMBOBOXSELECTED(control_id)` | Get selected item index | control_id |

### List View

| Function | Description | Parameters |
|----------|-------------|------------|
| `ADDLISTVIEWITEM(control_id, item)` | Add item to list view | control_id, item (text) |
| `CLEARLISTVIEWITEMS(control_id)` | Clear all list view items | control_id |
| `SETLISTVIEWSELECTED(control_id, index)` | Set selected item | control_id, index (number) |
| `GETLISTVIEWSELECTED(control_id)` | Get selected item index | control_id |

## Layout System

| Function | Description | Parameters |
|----------|-------------|------------|
| `CREATEHORIZONTALLAYOUT(x, y, width, height)` | Create horizontal layout | x, y, width, height |
| `CREATEVERTICALLAYOUT(x, y, width, height)` | Create vertical layout | x, y, width, height |
| `CREATEGRIDLAYOUT(x, y, width, height, columns, rows)` | Create grid layout | x, y, width, height, columns, rows |
| `ADDTOLAYOUT(layout_id, control_id)` | Add control to layout | layout_id, control_id |
| `UPDATELAYOUT(layout_id)` | Update layout | layout_id |

## Style System

| Function | Description | Parameters |
|----------|-------------|------------|
| `USEDEFAULTSTYLE()` | Use default style | None |
| `USEDARKSTYLE()` | Use dark style | None |
| `USELIGHTSTYLE()` | Use light style | None |
| `USEGAMESTYLE()` | Use game style | None |

## Control Management

| Function | Description | Parameters |
|----------|-------------|------------|
| `DESTROYCONTROL(control_id)` | Destroy specific control | control_id |
| `DESTROYALLCONTROLS()` | Destroy all controls | None |

## Common Patterns

### Basic GUI Setup
```basic
INITGAMESYSTEMS()
INITGUI()

REM Your GUI code here

UPDATEGUI()
DRAWGUI()

DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()
```

### Main Loop Pattern
```basic
WHILE NOT WINDOWSHOULDCLOSE()
    UPDATEGUI()
    
    REM Handle user interactions
    VAR slider_value = GETSLIDERVALUE(slider_id)
    VAR checkbox_state = ISCHECKBOXCHECKED(checkbox_id)
    
    DRAWGUI()
    SLEEP(16)
ENDWHILE
```

### Creating Multiple Controls
```basic
VAR buttons[5]
VAR i = 0
WHILE i < 5
    buttons[i] = CREATEBUTTON("button" + STR(i), 20, 20 + i * 40, 100, 30, "Button " + STR(i))
    i = i + 1
ENDWHILE
```

### Using Layouts
```basic
VAR layout = CREATEHORIZONTALLAYOUT(100, 100, 300, 50)
ADDTOLAYOUT(layout, button1)
ADDTOLAYOUT(layout, button2)
ADDTOLAYOUT(layout, button3)
UPDATELAYOUT(layout)
```

### Working with Lists
```basic
VAR listview = CREATELISTVIEW("list", 100, 100, 200, 150)
ADDLISTVIEWITEM(listview, "Item 1")
ADDLISTVIEWITEM(listview, "Item 2")
ADDLISTVIEWITEM(listview, "Item 3")

VAR selected = GETLISTVIEWSELECTED(listview)
IF selected >= 0 THEN
    PRINT "Selected item: " + STR(selected)
ENDIF
```

### Dynamic Control Updates
```basic
VAR progress = CREATEPROGRESSBAR("progress", 100, 100, 200, 20, 0, 100, 0)
VAR slider = CREATESLIDER("slider", 100, 130, 200, 20, 0, 100, 50)

REM In main loop:
VAR slider_value = GETSLIDERVALUE(slider)
SETPROGRESSBARVALUE(progress, slider_value)
```

## Parameter Types

- **name**: String - Unique identifier for the control
- **x, y**: Integer - Position coordinates
- **width, height**: Integer - Size dimensions
- **text**: String - Display text
- **min, max, current**: Number - Numeric values for sliders/progress bars
- **checked**: Boolean - True/false for checkboxes
- **visible, enabled**: Boolean - True/false for control states
- **control_id**: Integer - Return value from control creation functions
- **layout_id**: Integer - Return value from layout creation functions
- **index**: Integer - Zero-based index for list items

## Return Values

- Control creation functions return an integer ID
- Layout creation functions return an integer ID
- Get functions return the appropriate data type (string, number, boolean)
- Set functions return boolean (true for success)
- System functions return boolean (true for success)
