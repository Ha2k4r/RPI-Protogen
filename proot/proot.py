import time
import sys
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
from Datastruc import *


# Testing and whatnot
DISABLE_LOADING_SCREEN = False

LoadingscreenFrames=10
BlinkFrames = 10
MouthFrames = 0
NoseFrames = 0
LoadingscreenNum = 0
BlinkNum = 0
MouthNum = 0
NoseNum=0

# Settings
def settings(pixel_mapper_config):
    options = RGBMatrixOptions()
    options.brightness = 100
    options.disable_hardware_pulsing=False
    options.rows = 32
    options.hardware_mapping='adafruit-hat'
    options.cols = 64
    options.drop_privileges = 0
    options.gpio_slowdown = 3
    options.chain_length = 2
    options.parallel = 1
    options.pixel_mapper_config = pixel_mapper_config
    return RGBMatrix(options=options)

# Make image yk, an image.
def Display(canvas, image_path):
    image = Image.open(image_path)
    image.thumbnail((canvas.width, canvas.height), Image.LANCZOS)
    canvas.SetImage(image.convert('RGB'))

try:
    EyeR = settings("None")
    EyeL = settings("Mirror:H")
    MouthR = settings("None")
    MouthL = settings("Mirror:H")
    NoseR = settings("None")
    NoseL = settings("Mirror:H")
    Loadingscreen = settings("None")  

    # Initialize canvas buffers
    canvas1 = EyeR.CreateFrameCanvas()
    canvas2 = EyeL.CreateFrameCanvas()
    canvas3 = Loadingscreen.CreateFrameCanvas()

    # Initialize start times
    start_time1 = time.time()
    start_time2 = time.time()
    start_time3 = time.time()

    # Display durations
    display_duration1 = 0
    display_duration2 = 0
    display_duration3 = 0
    # Loading screen logic
    while not DISABLE_LOADING_SCREEN:
        if not DISABLE_LOADING_SCREEN :
            if LoadingscreenNum == LoadingscreenFrames:
                DISABLE_LOADING_SCREEN=True
                Display(canvas3, loading_screen_struc[LoadingscreenNum])
                time.sleep(loading_screen_delay[LoadingscreenNum])
                canvas1.Clear()
                canvas2.Clear()
                canvas3.Clear()
                
                break;
            Loadingscreen_bitmap = loading_screen_struc[LoadingscreenNum]
            Display(canvas3, loading_screen_struc[LoadingscreenNum])
            time.sleep(loading_screen_delay[LoadingscreenNum])
            LoadingscreenNum = LoadingscreenNum + 1
            Loadingscreen.SwapOnVSync(canvas3)
    # Main loop
    while True:
        # Eye animation logic
        elapsed_time1 = time.time() - start_time1
        elapsed_time2 = time.time() - start_time2
        elapsed_time3 = time.time() - start_time3
        if elapsed_time1 >= display_duration1:

            # Display the next frame
            Display(canvas1, eye_struc[BlinkNum])
            Display(canvas2, eye_struc[BlinkNum])

            # Swap the canvases
            EyeR.SwapOnVSync(canvas1)
            EyeL.SwapOnVSync(canvas2)

            display_duration1 = eye_delay[BlinkNum]
            start_time1 = time.time()

            # Increment frame count
            if BlinkNum == BlinkFrames:
                BlinkNum = 0
            else:
                BlinkNum += 1
        elapsed_time1 = time.time() - start_time1
        
        #-----------------------------MOUTH TICK---------------------
        
        if elapsed_time2 >= display_duration2:

            # Display the next frame
            Display(MouthR, mouth_struc[MouthNum])
            Display(MouthL, mouth_struc[MouthNum])

            # Swap the canvases
            EyeR.SwapOnVSync(canvas1)
            EyeL.SwapOnVSync(canvas2)

            display_duration2 = mouth_delay[MouthNum]
            start_time2 = time.time()

            # Increment frame count
            if MouthNum == MouthFrames:
                MouthNum = 0
            else:
                MouthNum += 1
        
        #--------------------------NOSE TICK-----------------------------
        if elapsed_time3 >= display_duration3:

            # Display the next frame
            Display(NoseR, nose_struc[NoseNum])
            Display(NoseL, nose_struc[NoseNum])

            # Swap the canvases
            EyeR.SwapOnVSync(canvas1)
            EyeL.SwapOnVSync(canvas2)

            display_duration3 = nose_delay[NoseNum]
            start_time3 = time.time()

            # Increment frame count
            if NoseNum == NoseFrames:
                NoseNum = 0
            else:
                NoseNum += 1


except KeyboardInterrupt:
    if EyeR:
        EyeR.Clear()
    if EyeL:
        EyeL.Clear()
    sys.exit(0)
