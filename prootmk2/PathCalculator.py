from math import log, exp
import numpy as np
#this calculates from a current location to a setpoint and outputs a non linear path to the setpoint
#it outputs a cryptic array that needs to be decoded using the UnpackBezierArray function below
#eventually i would like to add a scalor to dynamically choose how much overshoot and the rate of climb but less inputs the better
#though its definitally a consideration
def BezierCurveCalculation(Array, Target_Array, num_points):
    if len(Array) != len(Target_Array):
        raise ValueError("Arrays passed into BezierCurveCalculation have different lengths.")

    Trajectory_Numbers = []

    def bezier_point(t, p0, p1, p2, p3):
        return round((1 - t) ** 3 * p0 + 3 * (1 - t) ** 2 * t * p1 + 3 * (1 - t) * t ** 2 * p2 + t ** 3 * p3)

    for (x1, y1), (x2, y2) in zip(Array, Target_Array):
        #change these to change the amount of overshoot and the rate of climb
        #eventually these should be dynamic but who really cares right now 
        # 10 minute fix who cares
        xP1 = x1 + (x2 - x1) * 0.5
        xP2 = x1 + (x2 - x1) * 1.8
        yP1 = y1 + (y2 - y1) * 0.5
        yP2 = y1 + (y2 - y1) * 1.8

        points = []
        for i in range(num_points):
            t = i / (num_points - 1)  # Normalize t to range [0, 1]
            current_point_x = bezier_point(t, x1, xP1, xP2, x2)
            current_point_y = bezier_point(t, y1, yP1, yP2, y2)
            points.append([current_point_x, current_point_y])

        Trajectory_Numbers.append(points)

    # Flatten the list of points
    flattened_trajectory = [item for sublist in Trajectory_Numbers for item in sublist]

    return flattened_trajectory
# this is designed to pick from the array given by the bezier curve algoritm above
#in a way to make a geometric figure according to each index go twords the setpoint where
#the higher the index value the closer you are to the target value
def UnpackBezierArray(Index, Trajectory_Numbers, num_points):
    maxindex = int(len(Trajectory_Numbers)/num_points)
    if maxindex < len(Trajectory_Numbers):
        Geometric_Vertices = np.zeros((maxindex, 2), dtype=int)
        for i in range(maxindex):
            target = (num_points*i)+Index
            Geometric_Vertices[i] = Trajectory_Numbers[target]

        return Geometric_Vertices

    else:
        raise IndexError("Index passed into UnpackBezierArray function was larger than the max index of the Trajectory numbers :/")

#not implemented in the main branch yet but this is to blend color maps together
#for expression changes
def Blend(Image_1,Image_2, Degree):
    FinalImage = cv2.addWeighted(Image_1, Degree, Image_2, 1-Degree, 0)
    return FinalImage






# This was a experimental and now depreciated pathmaker and unpacker that no longer works
# possibly worth bringing back depending on future needs but a proportional one
# would be faster and more needed than a logaritmic one
# # call this function and it will return an organic logarithmic path to the target cordinate
# def Organic_Curve_Generate(Array, Target_Array, cycles):
#     if len(Array) == len(Target_Array):
#         Trajectory_Numbers = []
#         for (x1, y1), (x2, y2) in zip(Array, Target_Array):
#             # Calculate increment factors based on the difference between current and target points
#
#             if y1 == 0:
#                 y1=0.01
#             if y2 == 0:
#                 y2=0.01
#             if x1 == 0:
#                 x1=0.01
#             if x2 == 0:
#                 x2=0.01
#             increment_factor_x = log(x2 / x1) / cycles if x2 != x1 else 0
#             increment_factor_y = log(y2 / y1) / cycles if y2 != y1 else 0
#
#             # Iterate through cycles
#             for num in range(cycles):
#                 if y2 != 0 :
#                     yDerivitive = (y2 - y1) / y2
#                 else:
#                     yDerivitive = 1
#                 if x2 != 0:
#                     xDerivitive = (x2 - x1) / x2
#                 else:
#                     xDerivitive = 1
#
#
#                 current_point_x = round(x1 + (x2 - x1) * (1 - exp(-increment_factor_x *(num*xDerivitive))))
#                 current_point_y = round(y1 + (y2 - y1) * (1 - exp(-increment_factor_y * (num*yDerivitive))))
#
#                 if num == 0:
#                     Trajectory_Numbers.append([current_point_x, current_point_y])
#                 elif num == cycles - 1:
#                     Trajectory_Numbers[-1].extend([x2, y2])
#                 else:
#                     Trajectory_Numbers[-1].extend([current_point_x, current_point_y])
#     else:
#         raise UnboundLocalError("Arrays Passed into PathCalculator have different lengths.")
#     return Trajectory_Numbers
# def Make_Geometric_Figure(Index, Trajectory_Numbers):
#     if Index <= len(Trajectory_Numbers[0]):
#         Trajectory_Number_length = len(Trajectory_Numbers)
#         Geometric_Vertices = np.zeros((Trajectory_Number_length, 2), dtype=int)
#         for i in range(Trajectory_Number_length):
#             if (Index % 2) != 0:
#                 Index += 1
#             x = Trajectory_Numbers[i][0 + Index]
#             y = Trajectory_Numbers[i][1 + Index]
#             Geometric_Vertices[i] = [x, y]
#         return Geometric_Vertices
#
#     else:
#         raise IndexError("Index passed into Make_Geometric_Figure function was larger than the max index of the Trajectory numbers :/")

