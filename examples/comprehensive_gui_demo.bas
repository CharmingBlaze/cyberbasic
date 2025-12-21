REM Comprehensive GUI Demo - CyberBasic
REM Demonstrates all available GUI drawing commands and interactive elements

INITWINDOW(1200, 800, "CyberBasic Comprehensive GUI Demo")
SETTARGETFPS(60)

REM GUI State Variables
VAR button1_pressed = 0
VAR button2_pressed = 0
VAR button3_pressed = 0
VAR slider_value = 50
VAR checkbox1_checked = 0
VAR checkbox2_checked = 1
VAR checkbox3_checked = 0
VAR textbox_text$ = "Type here..."
VAR textbox_active = 0
VAR progress_value = 0.0
VAR scroll_position = 0.0
VAR selected_tab = 0
VAR color_r = 100
VAR color_g = 150
VAR color_b = 200
VAR mouse_x = 0
VAR mouse_y = 0
VAR mouse_button_down = 0

REM Helper function to check if mouse is over rectangle
FUNCTION mouseover(x, y, w, h) AS INTEGER
    IF mouse_x >= x AND mouse_x <= x + w AND mouse_y >= y AND mouse_y <= y + h THEN
        RETURN 1
    ELSE
        RETURN 0
    ENDIF
ENDFUNCTION

REM Helper function to draw button
FUNCTION drawbutton(x, y, w, h, text$, pressed) AS INTEGER
    VAR hover = mouseover(x, y, w, h)
    VAR clicked = 0
    
    IF hover = 1 AND mouse_button_down = 1 THEN
        DRAWRECTANGLEROUNDED(x, y, w, h, 0.3, 8, 60, 60, 80)
        clicked = 1
    ELSEIF hover = 1 THEN
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

REM Helper function to draw checkbox
FUNCTION drawcheckbox(x, y, size, label$, checked) AS INTEGER
    VAR hover = mouseover(x, y, size, size)
    VAR clicked = 0
    
    IF hover = 1 AND mouse_button_down = 1 THEN
        clicked = 1
    ENDIF
    
    DRAWRECTANGLE(x, y, size, size, 50, 50, 50)
    DRAWRECTANGLELINES(x, y, size, size, 200, 200, 200)
    
    IF checked = 1 THEN
        DRAWRECTANGLE(x + 2, y + 2, size - 4, size - 4, 100, 200, 100)
        DRAWLINE(x + 2, y + size/2, x + size/2, y + size - 2, 255, 255, 255)
        DRAWLINE(x + size/2, y + size - 2, x + size - 2, y + 2, 255, 255, 255)
    ENDIF
    
    DRAWTEXT(label$, x + size + 10, y + (size - 20) / 2, 18, 255, 255, 255)
    
    RETURN clicked
ENDFUNCTION

REM Helper function to draw slider
FUNCTION drawslider(x, y, w, h, min_val, max_val, value) AS DOUBLE
    VAR hover = mouseover(x, y, w, h)
    VAR new_value = value
    
    DRAWRECTANGLE(x, y, w, h, 40, 40, 40)
    DRAWRECTANGLELINES(x, y, w, h, 150, 150, 150)
    
    VAR slider_pos = (value - min_val) / (max_val - min_val)
    VAR handle_x = x + slider_pos * (w - h)
    VAR handle_y = y
    
    IF hover = 1 AND mouse_button_down = 1 THEN
        VAR mouse_rel = mouse_x - x
        slider_pos = mouse_rel / w
        IF slider_pos < 0 THEN slider_pos = 0
        IF slider_pos > 1 THEN slider_pos = 1
        new_value = min_val + slider_pos * (max_val - min_val)
        handle_x = x + slider_pos * (w - h)
    ENDIF
    
    DRAWRECTANGLEROUNDED(handle_x, handle_y, h, h, 0.5, 8, 100, 150, 200)
    DRAWRECTANGLEROUNDEDLINES(handle_x, handle_y, h, h, 0.5, 8, 255, 255, 255)
    
    VAR value_text$ = STR(INT(value))
    DRAWTEXT(value_text$, x + w + 10, y + (h - 18) / 2, 16, 200, 200, 200)
    
    RETURN new_value
ENDFUNCTION

REM Helper function to draw progress bar
FUNCTION drawprogressbar(x, y, w, h, min_val, max_val, value)
    VAR progress = (value - min_val) / (max_val - min_val)
    IF progress < 0 THEN progress = 0
    IF progress > 1 THEN progress = 1
    
    DRAWRECTANGLE(x, y, w, h, 30, 30, 30)
    DRAWRECTANGLELINES(x, y, w, h, 150, 150, 150)
    
    VAR fill_width = w * progress
    IF fill_width > 0 THEN
        DRAWRECTANGLE(x + 2, y + 2, fill_width - 4, h - 4, 50, 200, 100)
    ENDIF
    
    VAR percent_text$ = STR(INT(progress * 100)) + "%"
    VAR text_width = MEASURETEXT(percent_text$, 16)
    VAR text_x = x + (w - text_width) / 2
    VAR text_y = y + (h - 16) / 2
    DRAWTEXT(percent_text$, text_x, text_y, 16, 255, 255, 255)
ENDFUNCTION

REM Helper function to draw textbox
FUNCTION drawtextbox(x, y, w, h, text$, active) AS INTEGER
    VAR hover = mouseover(x, y, w, h)
    VAR clicked = 0
    
    IF hover = 1 AND mouse_button_down = 1 THEN
        clicked = 1
    ENDIF
    
    IF active = 1 THEN
        DRAWRECTANGLE(x, y, w, h, 60, 60, 80)
        DRAWRECTANGLELINES(x, y, w, h, 100, 150, 255)
    ELSE
        DRAWRECTANGLE(x, y, w, h, 40, 40, 50)
        DRAWRECTANGLELINES(x, y, w, h, 150, 150, 150)
    ENDIF
    
    DRAWTEXT(text$, x + 5, y + (h - 20) / 2, 18, 255, 255, 255)
    
    IF active = 1 THEN
        VAR cursor_x = x + MEASURETEXT(text$, 18) + 5
        VAR cursor_y = y + 5
        VAR cursor_h = h - 10
        VAR time = GETTIME()
        VAR blink = INT(time * 2) MOD 2
        IF blink = 0 THEN
            DRAWLINE(cursor_x, cursor_y, cursor_x, cursor_y + cursor_h, 255, 255, 255)
        ENDIF
    ENDIF
    
    RETURN clicked
ENDFUNCTION

REM Helper function to draw panel
FUNCTION drawpanel(x, y, w, h, title$)
    DRAWRECTANGLE(x, y, w, h, 35, 35, 45)
    DRAWRECTANGLELINES(x, y, w, h, 100, 100, 120)
    
    DRAWRECTANGLE(x, y, w, 30, 50, 50, 60)
    DRAWTEXT(title$, x + 10, y + 5, 20, 255, 255, 255)
    DRAWLINE(x, y + 30, x + w, y + 30, 100, 100, 120)
ENDFUNCTION

REM Helper function to draw tab
FUNCTION drawtab(x, y, w, h, label$, selected) AS INTEGER
    VAR hover = mouseover(x, y, w, h)
    VAR clicked = 0
    
    IF hover = 1 AND mouse_button_down = 1 THEN
        clicked = 1
    ENDIF
    
    IF selected = 1 THEN
        DRAWRECTANGLE(x, y, w, h, 60, 70, 90)
        DRAWRECTANGLELINES(x, y, w, h, 150, 150, 200)
    ELSEIF hover = 1 THEN
        DRAWRECTANGLE(x, y, w, h, 45, 50, 65)
        DRAWRECTANGLELINES(x, y, w, h, 120, 120, 150)
    ELSE
        DRAWRECTANGLE(x, y, w, h, 35, 40, 50)
        DRAWRECTANGLELINES(x, y, w, h, 100, 100, 120)
    ENDIF
    
    VAR text_width = MEASURETEXT(label$, 18)
    VAR text_x = x + (w - text_width) / 2
    VAR text_y = y + (h - 18) / 2
    DRAWTEXT(label$, text_x, text_y, 18, 255, 255, 255)
    
    RETURN clicked
ENDFUNCTION

PRINT "Comprehensive GUI Demo Started!"
PRINT "Click buttons, checkboxes, sliders, and tabs to interact"

WHILE NOT WINDOWSHOULDCLOSE()
    REM Update mouse position
    mouse_x = GETMOUSEX()
    mouse_y = GETMOUSEY()
    
    REM Check mouse button state
    IF ISMOUSEBUTTONDOWN(1) THEN
        mouse_button_down = 1
    ELSE
        mouse_button_down = 0
    ENDIF
    
    REM Handle keyboard input for textbox
    IF textbox_active = 1 THEN
        VAR key = GETKEYPRESSED()
        IF key > 0 THEN
            IF key = 259 THEN
                REM Backspace - remove last character
                VAR text_len = LEN(textbox_text$)
                IF text_len > 0 THEN
                    textbox_text$ = LEFT(textbox_text$, text_len - 1)
                ENDIF
            ENDIF
        ENDIF
        
        REM Get character input
        VAR char = GETCHARPRESSED()
        IF char > 0 THEN
            VAR char_str$ = STR(char)
            textbox_text$ = textbox_text$ + char_str$
        ENDIF
    ENDIF
    
    REM Update progress bar animation
    progress_value = progress_value + 0.5
    IF progress_value > 100 THEN progress_value = 0
    
    REM Begin drawing
    BEGINDRAWING()
    CLEARBACKGROUND(20, 25, 30)
    
    REM Draw title
    DRAWTEXT("CyberBasic Comprehensive GUI Demo", 20, 10, 32, 255, 255, 100)
    DRAWTEXT("FPS: " + STR(GETFPS()), 1100, 10, 20, 200, 200, 200)
    
    REM Draw tabs
    VAR tab_w = 150
    VAR tab_h = 35
    VAR tab_y = 50
    
    IF drawtab(20, tab_y, tab_w, tab_h, "Controls", selected_tab = 0) = 1 THEN selected_tab = 0
    IF drawtab(180, tab_y, tab_w, tab_h, "Colors", selected_tab = 1) = 1 THEN selected_tab = 1
    IF drawtab(340, tab_y, tab_w, tab_h, "Shapes", selected_tab = 2) = 1 THEN selected_tab = 2
    IF drawtab(500, tab_y, tab_w, tab_h, "Text", selected_tab = 3) = 1 THEN selected_tab = 3
    
    REM Tab 0: Controls
    IF selected_tab = 0 THEN
        REM Panel 1: Buttons
        drawpanel(20, 100, 350, 200, "Buttons")
        
        IF drawbutton(40, 140, 100, 40, "Button 1", button1_pressed) = 1 THEN
            button1_pressed = 1 - button1_pressed
            PRINT "Button 1 clicked!"
        ENDIF
        
        IF drawbutton(160, 140, 100, 40, "Button 2", button2_pressed) = 1 THEN
            button2_pressed = 1 - button2_pressed
            PRINT "Button 2 clicked!"
        ENDIF
        
        IF drawbutton(280, 140, 70, 40, "Reset", button3_pressed) = 1 THEN
            button3_pressed = 1 - button3_pressed
            button1_pressed = 0
            button2_pressed = 0
            PRINT "Reset clicked!"
        ENDIF
        
        DRAWTEXT("Status: Button1=" + STR(button1_pressed) + " Button2=" + STR(button2_pressed), 40, 200, 16, 200, 200, 200)
        
        REM Panel 2: Checkboxes
        drawpanel(400, 100, 350, 200, "Checkboxes")
        
        IF drawcheckbox(420, 140, 20, "Option 1", checkbox1_checked) = 1 THEN
            checkbox1_checked = 1 - checkbox1_checked
        ENDIF
        
        IF drawcheckbox(420, 170, 20, "Option 2", checkbox2_checked) = 1 THEN
            checkbox2_checked = 1 - checkbox2_checked
        ENDIF
        
        IF drawcheckbox(420, 200, 20, "Option 3", checkbox3_checked) = 1 THEN
            checkbox3_checked = 1 - checkbox3_checked
        ENDIF
        
        DRAWTEXT("Selected: " + STR(checkbox1_checked + checkbox2_checked + checkbox3_checked) + " options", 420, 240, 16, 200, 200, 200)
        
        REM Panel 3: Slider
        drawpanel(780, 100, 380, 200, "Slider Control")
        
        slider_value = drawslider(800, 150, 300, 30, 0, 100, slider_value)
        
        DRAWTEXT("Value: " + STR(INT(slider_value)), 800, 200, 18, 255, 255, 255)
        
        REM Panel 4: Progress Bar
        drawpanel(20, 320, 350, 120, "Progress Bar")
        
        drawprogressbar(40, 360, 310, 40, 0, 100, progress_value)
        DRAWTEXT("Animated progress bar", 40, 410, 16, 200, 200, 200)
        
        REM Panel 5: Textbox
        drawpanel(400, 320, 350, 120, "Text Input")
        
        VAR textbox_clicked = drawtextbox(420, 360, 310, 35, textbox_text$, textbox_active)
        IF textbox_clicked = 1 THEN
            textbox_active = 1 - textbox_active
        ENDIF
        
        IF mouse_button_down = 1 AND mouseover(420, 360, 310, 35) = 0 THEN
            textbox_active = 0
        ENDIF
        
        DRAWTEXT("Click to edit, type to input text", 420, 410, 14, 150, 150, 150)
    ENDIF
    
    REM Tab 1: Colors
    IF selected_tab = 1 THEN
        drawpanel(20, 100, 1140, 340, "Color Picker")
        
        DRAWTEXT("Red", 40, 140, 20, 255, 255, 255)
        color_r = drawslider(100, 140, 200, 25, 0, 255, color_r)
        
        DRAWTEXT("Green", 40, 180, 20, 255, 255, 255)
        color_g = drawslider(100, 180, 200, 25, 0, 255, color_g)
        
        DRAWTEXT("Blue", 40, 220, 20, 255, 255, 255)
        color_b = drawslider(100, 220, 200, 25, 0, 255, color_b)
        
        VAR preview_x = 350
        VAR preview_y = 140
        VAR preview_size = 100
        
        DRAWRECTANGLE(preview_x, preview_y, preview_size, preview_size, color_r, color_g, color_b)
        DRAWRECTANGLELINES(preview_x, preview_y, preview_size, preview_size, 255, 255, 255)
        
        DRAWTEXT("RGB(" + STR(color_r) + ", " + STR(color_g) + ", " + STR(color_b) + ")", preview_x, preview_y + 110, 18, 255, 255, 255)
        
        REM Draw color swatches
        VAR swatch_x = 500
        VAR swatch_y = 140
        VAR swatch_size = 50
        
        DRAWRECTANGLE(swatch_x, swatch_y, swatch_size, swatch_size, 255, 0, 0)
        DRAWRECTANGLE(swatch_x + 60, swatch_y, swatch_size, swatch_size, 0, 255, 0)
        DRAWRECTANGLE(swatch_x + 120, swatch_y, swatch_size, swatch_size, 0, 0, 255)
        DRAWRECTANGLE(swatch_x + 180, swatch_y, swatch_size, swatch_size, 255, 255, 0)
        DRAWRECTANGLE(swatch_x + 240, swatch_y, swatch_size, swatch_size, 255, 0, 255)
        DRAWRECTANGLE(swatch_x + 300, swatch_y, swatch_size, swatch_size, 0, 255, 255)
        DRAWRECTANGLE(swatch_x + 360, swatch_y, swatch_size, swatch_size, 255, 255, 255)
        DRAWRECTANGLE(swatch_x + 420, swatch_y, swatch_size, swatch_size, 0, 0, 0)
        
        DRAWTEXT("Color Swatches", swatch_x, swatch_y + 60, 16, 200, 200, 200)
    ENDIF
    
    REM Tab 2: Shapes
    IF selected_tab = 2 THEN
        drawpanel(20, 100, 1140, 340, "Shape Drawing")
        
        VAR shapes_x = 40
        VAR shapes_y = 140
        
        REM Circles
        DRAWCIRCLE(shapes_x + 50, shapes_y + 50, 40, 255, 100, 100)
        DRAWCIRCLELINES(shapes_x + 50, shapes_y + 50, 40, 255, 255, 255)
        DRAWTEXT("Circle", shapes_x, shapes_y + 100, 16, 255, 255, 255)
        
        REM Rectangles
        DRAWRECTANGLE(shapes_x + 200, shapes_y, 80, 80, 100, 255, 100)
        DRAWRECTANGLELINES(shapes_x + 200, shapes_y, 80, 80, 255, 255, 255)
        DRAWTEXT("Rectangle", shapes_x + 200, shapes_y + 90, 16, 255, 255, 255)
        
        REM Rounded Rectangle
        DRAWRECTANGLEROUNDED(shapes_x + 350, shapes_y, 80, 80, 0.3, 8, 100, 100, 255)
        DRAWRECTANGLEROUNDEDLINES(shapes_x + 350, shapes_y, 80, 80, 0.3, 8, 255, 255, 255)
        DRAWTEXT("Rounded", shapes_x + 350, shapes_y + 90, 16, 255, 255, 255)
        
        REM Triangle
        DRAWTRIANGLE(shapes_x + 500, shapes_y + 80, shapes_x + 540, shapes_y, shapes_x + 580, shapes_y + 80, 255, 200, 100)
        DRAWTRIANGLELINES(shapes_x + 500, shapes_y + 80, shapes_x + 540, shapes_y, shapes_x + 580, shapes_y + 80, 255, 255, 255)
        DRAWTEXT("Triangle", shapes_x + 520, shapes_y + 90, 16, 255, 255, 255)
        
        REM Lines
        DRAWLINE(shapes_x + 650, shapes_y, shapes_x + 730, shapes_y + 80, 255, 150, 150)
        DRAWTEXT("Line", shapes_x + 680, shapes_y + 90, 16, 255, 255, 255)
        
        REM Polygon (using triangle)
        DRAWTRIANGLE(shapes_x + 800, shapes_y + 40, shapes_x + 840, shapes_y, shapes_x + 880, shapes_y + 40, 200, 100, 255)
        DRAWTRIANGLE(shapes_x + 800, shapes_y + 40, shapes_x + 820, shapes_y + 80, shapes_x + 880, shapes_y + 40, 200, 100, 255)
        DRAWTEXT("Polygon", shapes_x + 820, shapes_y + 90, 16, 255, 255, 255)
        
        REM Ellipse (using circle with scaling)
        DRAWELLIPSE(shapes_x + 1000, shapes_y + 40, 50, 30, 150, 150, 255)
        DRAWELLIPSELINES(shapes_x + 1000, shapes_y + 40, 50, 30, 255, 255, 255)
        DRAWTEXT("Ellipse", shapes_x + 980, shapes_y + 90, 16, 255, 255, 255)
    ENDIF
    
    REM Tab 3: Text
    IF selected_tab = 3 THEN
        drawpanel(20, 100, 1140, 340, "Text Rendering")
        
        VAR text_x = 40
        VAR text_y = 140
        
        DRAWTEXT("Small Text (16px)", text_x, text_y, 16, 255, 255, 255)
        DRAWTEXT("Medium Text (24px)", text_x, text_y + 30, 24, 200, 255, 200)
        DRAWTEXT("Large Text (32px)", text_x, text_y + 70, 32, 255, 200, 200)
        DRAWTEXT("Extra Large (48px)", text_x, text_y + 120, 48, 200, 200, 255)
        
        DRAWTEXT("Colored Text Examples:", text_x, text_y + 180, 20, 255, 255, 255)
        DRAWTEXT("Red Text", text_x, text_y + 210, 20, 255, 0, 0)
        DRAWTEXT("Green Text", text_x + 150, text_y + 210, 20, 0, 255, 0)
        DRAWTEXT("Blue Text", text_x + 300, text_y + 210, 20, 0, 0, 255)
        DRAWTEXT("Yellow Text", text_x + 450, text_y + 210, 20, 255, 255, 0)
        DRAWTEXT("Cyan Text", text_x + 600, text_y + 210, 20, 0, 255, 255)
        DRAWTEXT("Magenta Text", text_x + 750, text_y + 210, 20, 255, 0, 255)
        
        VAR sample_text$ = "Sample text for measurement: " + STR(MEASURETEXT("Hello World", 20))
        DRAWTEXT(sample_text$, text_x, text_y + 250, 18, 200, 200, 200)
        
        VAR time_text$ = "Time: " + STR(GETTIME())
        DRAWTEXT(time_text$, text_x + 500, text_y + 250, 18, 150, 200, 255)
    ENDIF
    
    REM Draw mouse position indicator
    DRAWCIRCLE(mouse_x, mouse_y, 5, 255, 255, 0)
    DRAWCIRCLELINES(mouse_x, mouse_y, 5, 0, 0, 0)
    
    REM Draw instructions
    DRAWTEXT("Mouse: " + STR(mouse_x) + ", " + STR(mouse_y), 20, 770, 16, 200, 200, 200)
    DRAWTEXT("Click buttons, checkboxes, sliders, and tabs to interact", 300, 770, 16, 200, 200, 200)
    
    ENDDRAWING()
WEND

CLOSEWINDOW()
PRINT "GUI Demo completed!"

