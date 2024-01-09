import io 
import time
import sys
sys.path.append("/home/fur3/CairoSVG")
import numpy as np
import cairosvg
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image


# Configuration for the matrix
options = RGBMatrixOptions()
options.cols = 64 
options.rows = 32
options.chain_length = 2
options.gpio_slowdown=4
options.hardware_mapping = 'adafruit-hat'

matrix = RGBMatrix(options=options)

TRUEROWS = options.cols * options.chain_length 

# Convert SVG to bitmap
svg_file = "/home/fur3/Desktop/Code tests/Hblink1.svg"
image_data = cairosvg.svg2png(url=svg_file, write_to=None, output_width=matrix.width / options.chain_length, output_height=matrix.height)

image_stream = io.BytesIO(image_data)

# Load the bitmap image
image = Image.open(image_stream)

# Make image fit our screen.
image.thumbnail((matrix.width, matrix.height), Image.LANCZOS)

matrix.SetImage(image.convert('RGB'), 0, 0)
try:
    print("Press CTRL-C to stop.")
    while True:
        time.sleep(100)
except KeyboardInterrupt:
    sys.exit(0)
