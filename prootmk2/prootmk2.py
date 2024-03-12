import ExpressionVectors as EXV

import PathCalculator as PathC
import time
import numpy as np
import cv2
from Options import *

try:
    from rgbmatrix import RGBMatrix, RGBMatrixOptions
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
IsBlink = False


TRUEBLANK = np.zeros([32, 64], dtype='uint8')
EmptyCanvas = np.zeros([32, 64], dtype='uint8')

def Display(Matrix, preprocessed_image):
    image = Image.fromarray(cv2.cvtColor(preprocessed_image, cv2.COLOR_BGR2RGB))
    image.thumbnail((Matrix.width, Matrix.height), Image.LANCZOS)
    Matrix.SetImage(image)

def settings(pixel_mapper_config, Brightness):
    if (pixel_mapper_config == True):
            options = RGBMatrixOptions()
            options.drop_privileges= Drop_Privs
            options.brightness = Brightness
            options.rows = 32
            options.hardware_mapping='adafruit-hat-pwm'
            options.cols = 64
            options.scan_mode = 1
            options.gpio_slowdown = 3
            options.chain_length = 2
            options.pwm_lsb_nanoseconds = Global_Nano_Seconds
            options.limit_refresh_rate_hz = Refresh_Hertz_Cap
            options.parallel = 1
            return RGBMatrix(options=options)
    else:
            options = RGBMatrixOptions()
            options.drop_privileges= Drop_Privs
            options.brightness = Brightness
            options.rows = 32
            options.hardware_mapping='adafruit-hat-pwm'
            options.cols = 64
            options.scan_mode = 1
            options.gpio_slowdown = 3
            options.chain_length = 2
            options.pwm_lsb_nanoseconds = Global_Nano_Seconds
            options.limit_refresh_rate_hz = Refresh_Hertz_Cap
            options.parallel = 1
            options.pixel_mapper_config = "Mirror:H"
            return RGBMatrix(options=options)
def BlinkUpdate(UndecodedArray,Index):
    DecodedArray = PathC.Make_Geometric_Figure(Index, UndecodedArray)
    return DecodedArray

EyePathArray = []
while True:
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
    cycles = 50

#Eyes Time Check
    if IsBlink:
        if ElapsedTimeBS >= BlinkSpeed:  # skips the wait to continue the animation
            if Index < cycles*2-2 and BlinkState:
                Index += 1
            elif Index == cycles*2-2 and BlinkState:
                EyePathArray = PathC.Organic_Curve_Generate(EXV.EyeClosedHappyArray,EXV.EyeHappyArray, cycles)
                Index=0
                BlinkState = False
            elif Index < cycles*2-2 and not BlinkState:
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
    elif ElapsedTime >= BlinkEvery and not IsBlink:
        Index = 0
        EyePathArray = PathC.Organic_Curve_Generate(EXV.EyeHappyArray, EXV.EyeClosedHappyArray, cycles)
        EyeChords = BlinkUpdate(EyePathArray, Index)
        EyeVector = cv2.fillPoly(EmptyCanvas, [EyeChords], 255, 0, 0)  # updates sprite for RightMatrix
        EmptyCanvas = np.zeros_like(TRUEBLANK)
        changed = True
        BlinkState = True
        IsBlink = True
#Mouth Time Check
    ElapsedTimeM = time.time() - StartTimeM
    if ElapsedTimeM >= MouthUpdateSpeed:
        MouthVector = cv2.fillPoly(EmptyCanvas,[EXV.MouthHappyArray], 255,0,0)
        EmptyCanvas = np.zeros_like(TRUEBLANK)
        StartTimeM = time.time()
        changed = True
#Nose Time Check
    ElapsedTimeN = time.time() - StartTimeN
    if ElapsedTimeN >= NoseUpdateSpeed:
        NoseVector = cv2.fillConvexPoly(EmptyCanvas, EXV.NoseHappyArray, 255)
        EmptyCanvas = np.zeros_like(TRUEBLANK)
        StartTimeN = time.time()
        changed = True

    #Update screens
    if changed and not DebugMode:
        LeftMatrix = settings(True, Brightness)
        RightMatrix = settings(False, Brightness)

        Display(RightMatrix, masked)
        Display(LeftMatrix, masked)

    elif changed and DebugMode:
        mask = EyeVector + MouthVector + NoseVector
        masked = cv2.bitwise_and(frame, frame, mask=mask)

        # Update screen
        masked = cv2.resize(masked, (1200, 600))
        cv2.imshow("image", masked)
        cv2.waitKey(1)
        changed = False
        if len(EyeChords) != 9:
            print("EyeChords is not 9")
