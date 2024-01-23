from io import BytesIO
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image
import cv2
import time

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

def Display(Matrix, image_path):
    image = Image.open(image_path)
    image.thumbnail((Matrix.width, Matrix.height), Image.LANCZOS)
    Matrix.SetImage(image)

Screen1 = settings("None")
Screen2 = settings("Mirror:H")


# Create a VideoCapture object and read from input file
# If the input is the camera, pass 0 instead of the video file name
cap = cv2.VideoCapture('APPLE.mp4')

# Check if camera opened successfully
if (cap.isOpened() == False):
    print("Error opening video stream or file")

# Read until video is completed
while (cap.isOpened()):
    # Capture frame-by-frame
    ret, frame = cap.read()
    if ret == True:



        width = int(Screen1.width/2)
        height = int(Screen1.height)
        dim = (width, height)

        # resize image
        resized = cv2.resize(frame, dim, interpolation=cv2.INTER_CUBIC)
        pil_image = Image.fromarray(cv2.cvtColor(resized, cv2.COLOR_BGR2RGB))
        memory_image = BytesIO()
        pil_image.save(memory_image, format='png')
        
        Display(Screen1, memory_image)
        Display(Screen2, memory_image)
        
        time.sleep(0.0333333)
        # Press Q on keyboard to  exit
        

    # Break the loop
    else:
        break

# When everything done, release the video capture object
cap.release()

# Closes all the frames
cv2.destroyAllWindows()
