PRINT "Testing CyberBasic GUI System"

INITGUI
PRINT "GUI initialized"

button_id = CREATEBUTTON("test_button", 50, 50, 100, 30)
SETCONTROLTEXT(button_id, "Click Me!")
PRINT "Button created with ID: " + STR(button_id)

label_id = CREATELABEL("test_label", 50, 100, 200, 20)
SETCONTROLTEXT(label_id, "This is a test label")
PRINT "Label created with ID: " + STR(label_id)

PRINT "GUI test completed successfully!"

SHUTDOWNGUI
PRINT "GUI shutdown"
