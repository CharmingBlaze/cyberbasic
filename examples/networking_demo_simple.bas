REM Simple Networking Demo
REM Demonstrates basic networking functionality

PRINT "Initializing Networking System..."
INITNETWORKING()

PRINT "Creating network server..."
LET server_id = CREATENETWORKSERVER("GameServer", 8080, 16)

PRINT "Creating network client..."
LET client_id = CREATENETWORKCLIENT("Player1", "127.0.0.1", 8080)

PRINT "Networking demo setup complete!"
PRINT "Server ID: " + STR(server_id)
PRINT "Client ID: " + STR(client_id)

PRINT "Networking demo completed successfully!"
