from math import log, exp
import numpy as np

# how many frames before it should reach its target vertices

# call this function and it will return an organic logarithmic path to the target cordinate
def Organic_Curve_Generate(Array, Target_Array, cycles):
    if len(Array) == len(Target_Array):
        Trajectory_Numbers = []
        for (x1, y1), (x2, y2) in zip(Array, Target_Array):
            # Calculate increment factors based on the difference between current and target points

            if y1 == 0:
                y1=0.01
            if y2 == 0:
                y2=0.01
            if x1 == 0:
                x1=0.01
            if x2 == 0:
                x2=0.01
            increment_factor_x = log(x2 / x1) / cycles if x2 != x1 else 0
            increment_factor_y = log(y2 / y1) / cycles if y2 != y1 else 0

            # Iterate through cycles
            for num in range(cycles):
                if y2 != 0 :
                    yDerivitive = (y2 - y1) / y2
                else:
                    yDerivitive = 1
                if x2 != 0:
                    xDerivitive = (x2 - x1) / x2
                else:
                    xDerivitive = 1


                current_point_x = round(x1 + (x2 - x1) * (1 - exp(-increment_factor_x *(num*xDerivitive))))
                current_point_y = round(y1 + (y2 - y1) * (1 - exp(-increment_factor_y * (num*yDerivitive))))

                if num == 0:
                    Trajectory_Numbers.append([current_point_x, current_point_y])
                elif num == cycles - 1:
                    Trajectory_Numbers[-1].extend([x2, y2])
                else:
                    Trajectory_Numbers[-1].extend([current_point_x, current_point_y])
    else:
        raise UnboundLocalError("Arrays Passed into PathCalculator have different lengths.")
    return Trajectory_Numbers


# this is a decoder used to decrypt the very cryptic output of the plotter above
def Make_Geometric_Figure(Index, Trajectory_Numbers):
    if Index <= len(Trajectory_Numbers[0]):
        Trajectory_Number_length = len(Trajectory_Numbers)
        Geometric_Vertices = np.zeros((Trajectory_Number_length, 2), dtype=int)
        for i in range(Trajectory_Number_length):
            if (Index % 2) != 0:
                Index += 1
            x = Trajectory_Numbers[i][0 + Index]
            y = Trajectory_Numbers[i][1 + Index]
            Geometric_Vertices[i] = [x, y]
        return Geometric_Vertices

    else:
        raise IndexError("Index passed into Make_Geometric_Figure function was larger than the max index of the Trajectory numbers :/")
