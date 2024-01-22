import time
import copy
from io import BytesIO
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
import cv2
import numpy as np

#cool changable variables
GradientScrollSpeed = 0.05 #seconds per frame
BlinkDisplayDuration = 6 #seconds
BlinkSpeed = 0.03       #seconds

#Do not change just global declaration ill clean up later
StartTime=0
StartTimeF=0
StartTimeBS= time.time() + BlinkDisplayDuration # lol two birds and one stone with this one
IsBlink=False
BlinkState=False #True for Closed False for Open
Y=0
I=0
SucessfullRunthroughs = 0

#         Screen settings DO NOT CHANGE UNLESS ABSOLUTLY NESSISARY
def settings(pixel_mapper_config):
    options = RGBMatrixOptions()
    options.drop_privileges=True
    options.brightness = 100
    options.rows = 32
    options.hardware_mapping='adafruit-hat-pwm'
    options.cols = 64
    options.scan_mode = 1
    options.show_refresh_rate = True
    options.drop_privileges = 0
    options.gpio_slowdown = 3
    options.chain_length = 2
    options.pwm_lsb_nanoseconds = 50
    options.limit_refresh_rate_hz = 200
    options.parallel = 1
    options.pixel_mapper_config = pixel_mapper_config
    return RGBMatrix(options=options)


#Screens
RightMatrix = settings("None")
LeftMatrix = settings("Mirror:H")

def Display(Matrix, image_path):
    image = Image.open(image_path)
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
        # Capture frame-by-frame
        ret, frame = MovingColorMap.read()
        StartTimeF = time.time()
        if MovingColorMap.get(cv2.CAP_PROP_FRAME_COUNT) == MovingColorMap.get(cv2.CAP_PROP_POS_FRAMES):
            MovingColorMap.set(cv2.CAP_PROP_POS_FRAMES, 0)

    ElapsedTime = time.time() - StartTime
    ElapsedTimeBS = time.time() - StartTimeBS
    if IsBlink == True and ElapsedTimeBS >= BlinkSpeed:  # skips the wait to continue the animation
        BlinkUpdate()
        RightMatrixVector = cv2.fillConvexPoly(blank, RightMatrixVertices, 255, 0, 0)  # updates sprite for RightMatrix
        StartTimeBS = time.time()
    elif ElapsedTime >= BlinkDisplayDuration and IsBlink == False:
        BlinkState = True
        IsBlink = True
        BlinkUpdate()
    
    blank = np.zeros_like(blank)
    MouthVector = cv2.fillPoly(blank, [MouthVertices], 255)  # used but not important
    NoseVector = cv2.fillConvexPoly(blank, NoseVertices, 255)
    mask = RightMatrixVector + MouthVector + NoseVector
    masked = cv2.bitwise_and(frame, frame, mask=mask)
    #Save to memory not the drive which was weirdly complicated
    pil_image = Image.fromarray(cv2.cvtColor(masked,cv2.COLOR_BGR2RGB))
    memory_image = BytesIO()
    pil_image.save(memory_image, format='png')
    
    #Update screen
    Display(RightMatrix, memory_image)
    Display(LeftMatrix, memory_image)

