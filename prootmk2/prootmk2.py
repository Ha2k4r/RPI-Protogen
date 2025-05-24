#Thank you for using my code, this is for a protogen of mine but can easily be used as a basis for somone elses protogen's
#code this was made by Ha2k4r contact me on twitter
import ExpressionVectors as EXV
import PathCalculator as PathC
import time
from PIL import Image
import numpy as np
import cv2

from Options import *
try:
    from rgbmatrix import RGBMatrix, RGBMatrixOptions

    EyePathArray = []
    Screens = {
        RIGHT_MAIN_MATRIX: Settings(False),
        LEFT_MAIN_MATRIX: Settings(True)
    }
    Buffer = {
        RIGHT_MAIN_MATRIX: Screens[RIGHT_MAIN_MATRIX].CreateFrameCanvas(),
        LEFT_MAIN_MATRIX: Screens[LEFT_MAIN_MATRIX].CreateFrameCanvas()
    }
except ImportError:
    if not DebugMode:
        exit(ImportError("ImportError: Failed to import RGB Matrix Library"))
    else:
        pass
# Declarations ignore
StartTimeN = 0
StartTime = 0
StartTimeF = 0
StartTimeBS = time.time() + BlinkEvery
StartTimeM = 0
StartTimeCheckExpressions = 0
IsBlink = False
TRUEBLANK = np.zeros([32, 64], dtype='uint8')
EmptyCanvas = np.zeros([32, 64], dtype='uint8')
RIGHT_MAIN_MATRIX = "RightMatrix"
LEFT_MAIN_MATRIX = "LeftMatrix"


def Display(Screen, image):
    image = Image.fromarray(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    image.thumbnail((Screens[Screen].width, Screens[Screen].height), Image.LANCZOS)
    Buffer[Screen].SetImage(image.convert("RGB"))
    Buffer[Screen] = Screens[Screen].SwapOnVSync(Buffer[Screen])


def Settings(Flip):#
    # Configuration for the matrix
    options = RGBMatrixOptions()
    options.cols = 64
    options.chain_length = 2
    options.drop_privileges = False
    options.hardware_mapping = 'adafruit-hat-pwm'  #follow hzellers library for context on any of these as i made none of them
    options.gpio_slowdown = 4#                    he's right here : https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/examples-api-use/README.md
    options.pwm_lsb_nanoseconds = 50
    if Flip:
        options.pixel_mapper_config = "Mirror:H"

    return RGBMatrix(options=options)


def BlinkUpdate(UndecodedArray, Index):
    DecodedArray = PathC.UnpackBezierArray(Index, UndecodedArray, cycles)
    return DecodedArray

while True:
    if CurrentExpression != PreviousExpression:
        if CurrentExpression == "Happy":
            print("Happy")

            EyeArray = EXV.EyeHappyArray
            EyeClosedArray = EXV.EyeClosedHappyArray
            NoseArray = EXV.NoseHappyArray
            MouthArray = EXV.MouthHappyArray
            CurrentExpression = PreviousExpression
        elif CurrentExpression == "Blush":
            print("Blush")
            EyeArray = EXV.BlushEyeVectors
            EyeClosedArray = EXV.EyeClosedHappyArray
            NoseArray = EXV.NoseHappyArray
            MouthArray = EXV.MouthHappyArray
            CurrentExpression = PreviousExpression

    #Color Map Updates
    ElapsedTimeF = time.time() - StartTimeF
    if ElapsedTimeF >= GradientScrollSpeed:
        if MovingColorMap:
            # Get next frame in gradient sequence
            ret, frame = MovingColorMap.read()
            changed = True
            StartTimeF = time.time()
            if MovingColorMap.get(cv2.CAP_PROP_FRAME_COUNT) == MovingColorMap.get(cv2.CAP_PROP_POS_FRAMES):
                MovingColorMap.set(cv2.CAP_PROP_POS_FRAMES, 0)
        else:
            StartTimeF = time.time()
    ElapsedTime = time.time() - StartTime
    ElapsedTimeBS = time.time() - StartTimeBS
    cycles = 100
    # Eyes Time Check
    if IsBlink:
        if ElapsedTimeBS >= BlinkSpeed:  # skips the wait to continue the animation
            if Index < cycles-1 and BlinkState:
                Index += 1
            elif Index <= cycles-1 and BlinkState:
                EyePathArray = PathC.BezierCurveCalculation(EyeClosedArray, EyeArray, cycles)
                Index = 0
                BlinkState=False
            elif Index < cycles-1 and not BlinkState:
                Index += 1
            else:
                IsBlink = False
                StartTime = time.time()
                continue
            EyeChords = BlinkUpdate(EyePathArray, Index)
            EyeVector = cv2.fillPoly(EmptyCanvas, [EyeChords], 255, 0, 0)  # updates sprite for RightMatrix
            EmptyCanvas = np.zeros_like(TRUEBLANK)
            StartTimeBS = time.time()
            changed = True
    elif ElapsedTime >= BlinkEvery:
        Index = 0
        EyePathArray = PathC.BezierCurveCalculation(EyeArray, EyeClosedArray, cycles)
        EyeChords = BlinkUpdate(EyePathArray, Index)
        EyeVector = cv2.fillPoly(EmptyCanvas, [EyeChords], 255, 0, 0)  # updates sprite for RightMatrix
        EmptyCanvas = np.zeros_like(TRUEBLANK)
        changed = True
        BlinkState = True
        IsBlink = True
    # Mouth Time Check
    ElapsedTimeM = time.time() - StartTimeM
    if ElapsedTimeM >= MouthUpdateSpeed:
        MouthVector = cv2.fillPoly(EmptyCanvas, [MouthArray], 255, 0, 0)
        EmptyCanvas = np.zeros_like(TRUEBLANK)
        StartTimeM = time.time()
        changed = True
    # Nose Time Check
    ElapsedTimeN = time.time() - StartTimeN
    if ElapsedTimeN >= NoseUpdateSpeed:
        NoseVector = cv2.fillConvexPoly(EmptyCanvas, NoseArray, 255)
        EmptyCanvas = np.zeros_like(TRUEBLANK)
        StartTimeN = time.time()
        changed = True
    # Update screens
    if changed and not DebugMode:
        mask = EyeVector + MouthVector + NoseVector
        masked = cv2.bitwise_and(frame, frame, mask=mask)
        changed = False
        Display(RIGHT_MAIN_MATRIX, masked)
        Display(LEFT_MAIN_MATRIX, masked)

    elif changed and DebugMode:
        mask = EyeVector + MouthVector + NoseVector
        masked = cv2.bitwise_and(frame, frame, mask=mask)
        # Update screen
        masked = cv2.resize(masked, (1200, 600))
        cv2.imshow("image", masked)
        cv2.waitKey(1)
        changed = False
