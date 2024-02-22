import numpy as np
import copy
#This file is for the XY cordinates that form the geomatry of the sprites for the Eyes Nose and mouth
#if you would like different shapes for your facial features this is the file you change, each expression will be differnt shapes

#        Width1                   Height1

EyeHappyArray = np.array([
    [0, 3],  # far Left
    [6, 5],
    [11, 5],
    [20, 7],
    [24, 6],
    [24, 4],
    [18, 1],
    [15, 0],
    [12, 0]  # top left on top of the angle
], np.int32)

EyeClosedHappyArray = np.array([
    [0, 60],  # far Left
    [6, 6],
    [11, 6],
    [20, 6],
    [24, 6],
    [24, 4],
    [18, 1],
    [15, 0],
    [12, 0]  # top left on top of the angle
], np.int32)

EyeHappyDefault = copy.deepcopy(EyeHappyArray)

MouthHappyArray = np.array([
    [14, 19],
    [32, 29],
    [45, 23],
    [53, 23],
    [61, 26],
    [59, 27],
    [52, 25],
    [46, 25],
    [34, 31],
    [29, 31],
    [19, 25],
    [11, 25],
    [7, 21],
    [10, 20]

], np.int32)

NoseHappyArray = np.array([
    # counterclockwise
    [63, 10],  # bottom of nose
    [63, 6],
    [63, 4],
    [61, 2],
    [56, 1],
    [56, 1],
    [53, 2],  # far left of nose end point
    [57, 2],
    [61, 5],
    [63, 10],

], np.int32)