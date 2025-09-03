REM Inventory System Demo - Shows how to build a complete game inventory GUI
INITGAMESYSTEMS()
INITGUI()

PRINT "=== CyberBasic Inventory System Demo ==="

REM Create main inventory window
VAR inventory_window = CREATEWINDOW("inventory", 100, 100, 500, 400, "Player Inventory")

REM Create inventory grid (3x4 grid of items)
VAR item_slots[12]
VAR i = 0
WHILE i < 12
    VAR slot_x = 20 + (i % 3) * 80
    VAR slot_y = 50 + (i / 3) * 80
    item_slots[i] = CREATEPANEL("slot" + STR(i), slot_x, slot_y, 70, 70)
    i = i + 1
ENDWHILE

REM Create item list
VAR item_list = CREATELISTVIEW("item_list", 300, 50, 180, 200)
ADDLISTVIEWITEM(item_list, "Health Potion")
ADDLISTVIEWITEM(item_list, "Mana Potion")
ADDLISTVIEWITEM(item_list, "Iron Sword")
ADDLISTVIEWITEM(item_list, "Wooden Shield")
ADDLISTVIEWITEM(item_list, "Magic Ring")
ADDLISTVIEWITEM(item_list, "Gold Coin")
ADDLISTVIEWITEM(item_list, "Rope")
ADDLISTVIEWITEM(item_list, "Torch")

REM Create item info panel
VAR info_panel = CREATEPANEL("info_panel", 300, 260, 180, 100)
VAR item_name_label = CREATELABEL("item_name", 305, 265, 170, 20, "Select an item")
VAR item_desc_label = CREATELABEL("item_desc", 305, 285, 170, 40, "No item selected")
VAR item_value_label = CREATELABEL("item_value", 305, 325, 170, 20, "Value: 0 gold")

REM Create action buttons
VAR use_button = CREATEBUTTON("use_button", 300, 370, 80, 25, "Use")
VAR drop_button = CREATEBUTTON("drop_button", 390, 370, 80, 25, "Drop")
VAR close_button = CREATEBUTTON("close_button", 420, 20, 60, 25, "Close")

REM Create status bar
VAR status_bar = CREATELABEL("status_bar", 20, 380, 460, 15, "Inventory loaded - 8 items")

PRINT "Inventory GUI created successfully!"

REM Demo item selection
PRINT "Testing item selection..."
SETLISTVIEWSELECTED(item_list, 2)
SETCONTROLTEXT(item_name_label, "Iron Sword")
SETCONTROLTEXT(item_desc_label, "A sturdy iron sword. Damage: 15")
SETCONTROLTEXT(item_value_label, "Value: 50 gold")

PRINT "Item selection working!"

REM Demo inventory management
PRINT "Testing inventory management..."
ADDLISTVIEWITEM(item_list, "Diamond")
ADDLISTVIEWITEM(item_list, "Scroll of Fire")
SETCONTROLTEXT(status_bar, "Inventory updated - 10 items")

PRINT "Inventory management working!"

REM Update and draw
UPDATEGUI()
DRAWGUI()

PRINT "Inventory GUI updated and drawn!"

REM Clean up
DESTROYALLCONTROLS()
SHUTDOWNGUI()
SHUTDOWNGAMESYSTEMS()

PRINT "=== Inventory Demo Completed Successfully! ==="
