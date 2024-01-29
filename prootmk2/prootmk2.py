import time
import copy
from io import BytesIO
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
import cv2
import numpy as np

#Changeable variables
GradientScrollSpeed = 0.05 
BlinkDisplayDuration = 6 
BlinkSpeed = 0.03
MouthUpdateSpeed = 6
NoseUpdateSpeed = 6

Drop_Privs = False
Show_Refresh = False
Refresh_Hertz_Cap = 200
Global_Nano_Seconds = 50

#Declarations ignore
StartTimeN = 0
StartTime=0
StartTimeF=0
StartTimeBS= time.time() + BlinkDisplayDuration
StartTimeM = 0
IsBlink=False
BlinkState=False #True for Closed False for Open
Y=0
I=0
SucessfullRunthroughs = 0

#         Screen settings DO NOT CHANGE UNLESS ABSOLUTLY NESSISARY
def settings(pixel_mapper_config):
    if (pixel_mapper_config == True):
            options = RGBMatrixOptions()
            options.drop_privileges= Drop_Privs
            options.brightness = 100
            options.rows = 32
            options.hardware_mapping='adafruit-hat-pwm'
            options.cols = 64
            options.scan_mode = 1
            options.show_refresh_rate = Show_Refresh
            options.gpio_slowdown = 3
            options.chain_length = 2
            options.pwm_lsb_nanoseconds = Global_Nano_Seconds
            options.limit_refresh_rate_hz = Refresh_Hertz_Cap
            options.parallel = 1
            return RGBMatrix(options=options)
    else:
            options = RGBMatrixOptions()
            options.drop_privileges= Drop_Privs
            options.brightness = 100
            options.rows = 32
            options.hardware_mapping='adafruit-hat-pwm'
            options.cols = 64
            options.scan_mode = 1
            options.show_refresh_rate = Show_Refresh
            options.gpio_slowdown = 3
            options.chain_length = 2
            options.pwm_lsb_nanoseconds = Global_Nano_Seconds
            options.limit_refresh_rate_hz = Refresh_Hertz_Cap
            options.parallel = 1
            options.pixel_mapper_config = "Mirror:H"
            return RGBMatrix(options=options)

#Screens
LeftMatrix = settings(True)
RightMatrix = settings(False)

def Display(Matrix, preprocessed_image):
    image = Image.fromarray(cv2.cvtColor(preprocessed_image, cv2.COLOR_BGR2RGB))
    image.thumbnail((Matrix.width, Matrix.height), Image.LANCZOS)
    Matrix.SetImage(image)

MovingColorMap = cv2.VideoCapture('WHOH1.gif')


blank = np.zeros([32, 64], dtype='uint8')

#        Width1                   Height1
RightMatrixVertices = np.array([
    [0,    3], #far Left
    [6,    5],
    [11,   5],
    [20,   7],
    [24,   6],
    [24,   4],
    [18,   1],
    [15,   0],
    [12,   0] #top left on top of the angle
], np.int32)

RightMatrixVerticesDefault = copy.deepcopy(RightMatrixVertices)

MouthVertices = np.array([
    [14,  19],
    [32,  29],
    [45,  23],
    [53,  23],
    [61,  26],
    [59,  27],
    [52,  25],
    [46,  25],
    [34,  31],
    [29,  31],
    [19,  25],
    [11,  25],
    [7,   21],
    [10,  20]

], np.int32)

NoseVertices = np.array([
    #counterclockwise
    [63,  10],#bottom of nose
    [63,  6],
    [63,  4],
    [61,  2],
    [56,  1],
    [56,  1],
    [53,  2], #far left of nose end point
    [57,  2],
    [61,  5],
    [63, 10],


], np.int32)

def BlinkDown(XY):
    XY[1] -= 1

def BlinkUp(XY):
    XY[1] += 1

def BlinkUpdate():
    global BlinkState
    global I
    XY = RightMatrixVertices[I]
    XYD = RightMatrixVerticesDefault[I]

    if XY[1] <= 3:
        if BlinkState == True:
            BlinkUp(XY)
        else:
            if XY[1] > XYD[1]:
                BlinkDown(XY)
            elif XY[1] < XYD[1]:
                BlinkUp(XY)
    else:
        if BlinkState == True:
            BlinkDown(XY)
        else:
            if XY[1] < XYD[1]:
                BlinkUp(XY)
            elif XY[1] > XYD[1]:
                BlinkDown(XY)
    if RightMatrixVertices[3][1] == 3:#  opens the RightMatrix
        BlinkState=False
    if np.array_equal(RightMatrixVertices,RightMatrixVerticesDefault) and BlinkState==False: # ends loop
        global IsBlink
        global StartTime
        StartTime = time.time()
        IsBlink = False
        I=0
        return None
    I=I+1
    if I >= 9:
        I = 0

RightMatrixVector = cv2.fillConvexPoly(blank, RightMatrixVertices, 255, 0, 0)
MouthVector = cv2.fillPoly(blank, [MouthVertices], 255)  # i have yet to do anything with these but they are ver
NoseVector = cv2.fillConvexPoly(blank, NoseVertices, 255)

while True:
    ElapsedTimeF = time.time() - StartTimeF
    if ElapsedTimeF >= GradientScrollSpeed:
        # Display Gradient
        ret, frame = MovingColorMap.read()
        changed = True
        StartTimeF = time.time()
        if MovingColorMap.get(cv2.CAP_PROP_FRAME_COUNT) == MovingColorMap.get(cv2.CAP_PROP_POS_FRAMES):
            MovingColorMap.set(cv2.CAP_PROP_POS_FRAMES, 0)

    ElapsedTime = time.time() - StartTime
    ElapsedTimeBS = time.time() - StartTimeBS
    if IsBlink == True and ElapsedTimeBS >= BlinkSpeed:  # skips the wait to continue the animation
        BlinkUpdate()
        RightMatrixVector = cv2.fillConvexPoly(blank, RightMatrixVertices, 255, 0, 0)  # updates sprite for RightMatrix
        StartTimeBS = time.time()
        changed = True
    elif ElapsedTime >= BlinkDisplayDuration and IsBlink == False:
        BlinkState = True
        IsBlink = True
        BlinkUpdate()
        changed = True
    
    ElapsedTimeM = time.time() - StartTimeM
    if ElapsedTimeM >= MouthUpdateSpeed:
        MouthVector = cv2.fillPoly(blank, [MouthVertices], 255)
        StartTimeM = time.time()
        changed = True
        
    ElapsedTimeN = time.time() - StartTimeN
    if ElapsedTimeN >= NoseUpdateSpeed:
        NoseVector = cv2.fillConvexPoly(blank, NoseVertices, 255)
        StartTimeN = time.time()
        changed = True
    
    if changed == True:
        mask = RightMatrixVector + MouthVector + NoseVector
        masked = cv2.bitwise_and(frame, frame, mask=mask)
        blank = np.zeros_like(blank) 
        
        #Update screen
        Display(RightMatrix, masked)
        Display(LeftMatrix, masked)
        
        changed = False
