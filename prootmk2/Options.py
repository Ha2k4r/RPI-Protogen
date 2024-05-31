from os.path import dirname, realpath, join
from sys import path

current_dir = dirname(realpath(__file__))

from cv2 import imread,VideoCapture

#Output settings
Global_Nano_Seconds = 50
Refresh_Hertz_Cap=100

Brightness = 100

# Changeable variables

DebugMode = True

#only two expressions are currently implemented
#Expression_list = [Happy , Blush]

CurrentExpression = "Happy"
PreviousExpression = "anything other than current Expression"

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
#print(path.append(join(current_dir, "Traaaaaans.png")))
#TransColorMap = imread(join(current_dir, "Color_Maps\\Traaaaaans.png"))
MovingColorMap = VideoCapture(join(current_dir, "Color_Maps\\WHOH1.gif"))
