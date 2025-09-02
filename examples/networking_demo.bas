REM Networking System Demo
REM Demonstrates client-server networking, message handling, and multiplayer features

PRINT "Initializing Networking System..."
INITNETWORKING

PRINT "Creating network server..."
server_id = CREATENETWORKSERVER("GameServer", 8080, 16)
PRINT "Server created with ID: "; server_id

PRINT "Creating network clients..."
client1_id = CREATENETWORKCLIENT("Player1", "127.0.0.1", 8080)
client2_id = CREATENETWORKCLIENT("Player2", "127.0.0.1", 8080)
PRINT "Client 1 created with ID: "; client1_id
PRINT "Client 2 created with ID: "; client2_id

PRINT "Starting server..."
server_started = STARTNETWORKSERVER(server_id)
IF server_started THEN
    PRINT "Server started successfully!"
ELSE
    PRINT "Failed to start server!"
END IF

PRINT "Connecting clients..."
client1_connected = CONNECTNETWORKCLIENT(client1_id)
client2_connected = CONNECTNETWORKCLIENT(client2_id)

IF client1_connected THEN
    PRINT "Client 1 connected successfully!"
ELSE
    PRINT "Client 1 failed to connect!"
END IF

IF client2_connected THEN
    PRINT "Client 2 connected successfully!"
ELSE
    PRINT "Client 2 failed to connect!"
END IF

PRINT "Networking system initialized!"
PRINT "Servers: "; GETNETWORKSERVERCOUNT
PRINT "Clients: "; GETNETWORKCLIENTCOUNT

REM Main networking loop simulation
PRINT "Running networking simulation..."
FOR i = 1 TO 50
    REM Update networking system
    UPDATENETWORKINGSYSTEM
    
    REM Send chat messages
    IF i = 10 THEN
        SENDCHATMESSAGE(client1_id, "Hello from Player 1!")
        PRINT "Sent chat message from Player 1"
    END IF
    
    IF i = 20 THEN
        SENDCHATMESSAGE(client2_id, "Hello from Player 2!")
        PRINT "Sent chat message from Player 2"
    END IF
    
    REM Check for incoming messages
    IF NETWORKHASMESSAGES THEN
        message = NETWORKGETNEXTMESSAGE
        PRINT "Received message: "; message
    END IF
    
    REM Simulate game state updates
    IF i = 30 THEN
        PRINT "Simulating game state update..."
    END IF
    
    REM Simulate player actions
    IF i = 40 THEN
        PRINT "Simulating player actions..."
    END IF
NEXT i

PRINT "Networking demo completed!"
PRINT "Final server count: "; GETNETWORKSERVERCOUNT
PRINT "Final client count: "; GETNETWORKCLIENTCOUNT
