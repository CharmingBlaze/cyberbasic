REM Minimal audio demo
REM Make sure you have a short WAV/OGG file in the project folder, e.g. beep.wav

PRINT "Loading sound..."
LET sid = LOADSOUND("beep.wav")
IF sid > 0 THEN PRINT "Playing..."
PLAYSOUND(sid)

PRINT "Press ENTER to stop"
INPUT
STOPSOUND(sid)
UNLOADSOUND(sid)
PRINT "Done"
