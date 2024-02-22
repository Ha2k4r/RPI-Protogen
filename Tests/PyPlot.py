import math
import numpy as np
import matplotlib.pyplot as plt


def increment_logarithmically(current_point, setpoint, cycles):
    # Ensure current_point and setpoint are positive
    current_point = max(0, current_point)
    setpoint = max(0, setpoint)

    # Ensure cycles is a positive integer
    cycles = max(1, int(cycles))

    # Calculate the increment factor based on the number of cycles
    increment_factor = math.log(setpoint / current_point) / cycles

    # Generate current points using a generator expression
    current_points = np.array(
        [setpoint - (setpoint - current_point) * np.exp(-increment_factor * cycle) for cycle in range(cycles + 1)])

    # Round the current points to three decimal places
    current_points = np.round(current_points, 3)

    # Plot the graph
    plt.plot(range(cycles + 1), current_points, marker='o')
    plt.xlabel('Cycle Number')
    plt.ylabel('Current Point')
    plt.title('Decelerating Increment Towards Setpoint')
    plt.grid(True)
    plt.show()


# Example usage:
current_point = 1  # Initial current point
setpoint = 100  # Desired setpoint
cycles = 9  # Number of cycles
increment_logarithmically(current_point, setpoint, cycles)