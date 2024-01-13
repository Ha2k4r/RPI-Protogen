import copy

import cv2
import numpy as np
import time

#cool changable variables
GradientScrollSpeed = 0.3
BlinkDisplayDuration = 6
BlinkSpeed = 0.1




#Do not change just global declaration ill clean up later
StartTime=0
StartTimeF=0
StartTimeBS= time.time() + BlinkDisplayDuration # lol two birds and one stone with this one
IsBlink=False
BlinkState=False #True for Closed False for Open
Y=0
I=0
SucessfullRunthroughs = 0

MovingColorMap = cv2.VideoCapture('WHOH1.gif')

Prootbefore = cv2.imread('Prootbefore.png')

blank = np.zeros([32, 64], dtype='uint8')

#        Width1                   Height1
EyeVertices = np.array([
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

EyeVerticesDefault = copy.deepcopy(EyeVertices)

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
    XY = EyeVertices[I]
    XYD = EyeVerticesDefault[I]

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
    if EyeVertices[3][1] == 3:#  opens the eye
        BlinkState=False
    if np.array_equal(EyeVertices,EyeVerticesDefault) and BlinkState==False: # ends loop
        global IsBlink
        global StartTime
        StartTime = time.time()
        IsBlink = False
        I=0
        return None
    I=I+1
    if I >= 9:
        I = 0
EyeVector = cv2.fillConvexPoly(blank, EyeVertices, 255, 0, 0)
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
    if IsBlink == True and ElapsedTimeBS >= BlinkSpeed: #skips the wait to continue the animation
        BlinkUpdate()
        EyeVector = cv2.fillConvexPoly(blank, EyeVertices, 255, 0, 0) #updates sprite for eye
        StartTimeBS = time.time()
    elif ElapsedTime >= BlinkDisplayDuration and IsBlink == False:
        BlinkState = True
        IsBlink = True
        BlinkUpdate()
    blank = np.zeros_like(blank)
    MouthVector = cv2.fillPoly(blank, [MouthVertices], 255)  # i have yet to do anything with these but they are ver
    NoseVector = cv2.fillConvexPoly(blank, NoseVertices, 255)
    mask = EyeVector + MouthVector + NoseVector
    masked = cv2.bitwise_and(frame, frame, mask=mask)
    TEST_OUTPUT = cv2.resize( masked, [1600,800])
    cv2.imshow('TEST_OUTPUT', TEST_OUTPUT)
    # Press Q on keyboard to  exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
