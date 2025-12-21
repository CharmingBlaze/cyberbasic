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

## GUI Drawing Commands

CyberBasic provides comprehensive drawing commands for building custom GUI elements. These are the core functions used in `comprehensive_gui_demo.bas`:

### Basic Shapes

| Function | Description | Parameters |
|----------|-------------|------------|
| `DRAWRECTANGLE(x, y, width, height, r, g, b)` | Draw filled rectangle | x, y, width, height, red, green, blue |
| `DRAWRECTANGLELINES(x, y, width, height, r, g, b)` | Draw rectangle outline | x, y, width, height, red, green, blue |
| `DRAWRECTANGLEROUNDED(x, y, width, height, roundness, segments, r, g, b)` | Draw rounded rectangle | x, y, width, height, roundness (0.0-1.0), segments, red, green, blue |
| `DRAWRECTANGLEROUNDEDLINES(x, y, width, height, roundness, segments, r, g, b)` | Draw rounded rectangle outline | x, y, width, height, roundness, segments, red, green, blue |
| `DRAWCIRCLE(x, y, radius, r, g, b)` | Draw filled circle | x, y, radius, red, green, blue |
| `DRAWCIRCLELINES(x, y, radius, r, g, b)` | Draw circle outline | x, y, radius, red, green, blue |
| `DRAWTRIANGLE(x1, y1, x2, y2, x3, y3, r, g, b)` | Draw filled triangle | x1, y1, x2, y2, x3, y3, red, green, blue |
| `DRAWTRIANGLELINES(x1, y1, x2, y2, x3, y3, r, g, b)` | Draw triangle outline | x1, y1, x2, y2, x3, y3, red, green, blue |
| `DRAWLINE(x1, y1, x2, y2, r, g, b)` | Draw line | x1, y1, x2, y2, red, green, blue |
| `DRAWELLIPSE(x, y, radiusX, radiusY, r, g, b)` | Draw filled ellipse | x, y, radiusX, radiusY, red, green, blue |
| `DRAWELLIPSELINES(x, y, radiusX, radiusY, r, g, b)` | Draw ellipse outline | x, y, radiusX, radiusY, red, green, blue |

### Text Rendering

| Function | Description | Parameters |
|----------|-------------|------------|
| `DRAWTEXT(text, x, y, fontSize, r, g, b)` | Draw text | text string, x, y, fontSize, red, green, blue |
| `MEASURETEXT(text, fontSize)` | Measure text width | text string, fontSize |

### Input Functions

| Function | Description | Parameters |
|----------|-------------|------------|
| `GETMOUSEX()` | Get mouse X position | None |
| `GETMOUSEY()` | Get mouse Y position | None |
| `ISMOUSEBUTTONDOWN(button)` | Check if mouse button is down | button (1=left, 2=right, 3=middle) |
| `ISMOUSEBUTTONPRESSED(button)` | Check if mouse button was pressed | button (1=left, 2=right, 3=middle) |
| `GETKEYPRESSED()` | Get pressed key code | None |
| `GETCHARPRESSED()` | Get pressed character | None |

### Example: Custom Button

```basic
FUNCTION drawbutton(x, y, w, h, text$, hover) AS INTEGER
    VAR clicked = 0
    IF hover = 1 THEN
        DRAWRECTANGLEROUNDED(x, y, w, h, 0.3, 8, 100, 120, 150)
    ELSE
        DRAWRECTANGLEROUNDED(x, y, w, h, 0.3, 8, 80, 100, 120)
    ENDIF
    DRAWRECTANGLEROUNDEDLINES(x, y, w, h, 0.3, 8, 200, 200, 200)
    
    VAR text_width = MEASURETEXT(text$, 20)
    VAR text_x = x + (w - text_width) / 2
    VAR text_y = y + (h - 20) / 2
    DRAWTEXT(text$, text_x, text_y, 20, 255, 255, 255)
    
    RETURN clicked
ENDFUNCTION
```

### Example: Custom Slider

```basic
FUNCTION drawslider(x, y, w, h, min_val, max_val, value) AS DOUBLE
    DRAWRECTANGLE(x, y, w, h, 40, 40, 40)
    DRAWRECTANGLELINES(x, y, w, h, 150, 150, 150)
    
    VAR slider_pos = (value - min_val) / (max_val - min_val)
    VAR handle_x = x + slider_pos * (w - h)
    
    DRAWRECTANGLEROUNDED(handle_x, y, h, h, 0.5, 8, 100, 150, 200)
    
    RETURN value
ENDFUNCTION
```

### Example: Custom Checkbox

```basic
FUNCTION drawcheckbox(x, y, size, label$, checked) AS INTEGER
    DRAWRECTANGLE(x, y, size, size, 50, 50, 50)
    DRAWRECTANGLELINES(x, y, size, size, 200, 200, 200)
    
    IF checked = 1 THEN
        DRAWRECTANGLE(x + 2, y + 2, size - 4, size - 4, 100, 200, 100)
        DRAWLINE(x + 2, y + size/2, x + size/2, y + size - 2, 255, 255, 255)
        DRAWLINE(x + size/2, y + size - 2, x + size - 2, y + 2, 255, 255, 255)
    ENDIF
    
    DRAWTEXT(label$, x + size + 10, y + (size - 20) / 2, 18, 255, 255, 255)
    
    RETURN 0
ENDFUNCTION
```

See `examples/comprehensive_gui_demo.bas` for a complete working example with all GUI elements.