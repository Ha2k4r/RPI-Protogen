from cv2 import imread,VideoCapture

#Output settings
Global_Nano_Seconds = 50
Refresh_Hertz_Cap=100

Brightness = 100

# Changeable variables

DebugMode = True

Index = 0

GradientScrollSpeed = 0.042
BlinkEvery = 4
BlinkSpeed = 0.0001
MouthUpdateSpeed = 6
NoseUpdateSpeed = 6

Drop_Privs = False
Show_Refresh = False
Refresh_Hertz_Cap = 200
Global_Nano_Seconds = 50

#Images and gifs

TransColorMap = imread("Traaaaaans.png")
MovingColorMap = VideoCapture('WHOH1.gif')
