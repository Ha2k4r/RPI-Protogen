import numpy as np
import matplotlib.pyplot as plt

def increase_resolution(polygon, target_vertices):
    # Ensure the polygon is closed
    if not np.array_equal(polygon[0], polygon[-1]):
        polygon = np.vstack([polygon, polygon[0]])

    # Calculate total perimeter to determine edge weights
    total_length = sum(np.linalg.norm(polygon[i] - polygon[i-1]) for i in range(1, len(polygon)))

    # Determine vertices to add based on edge length
    new_polygon = []
    accumulated_vertices = 0  # Track the number of vertices added

    for i in range(len(polygon) - 1):
        start = polygon[i]
        end = polygon[i + 1]
        edge_length = np.linalg.norm(end - start)
        edge_weight = edge_length / total_length

        if i < len(polygon) - 2:  # Avoid over-adding on the last edge
            num_points_on_edge = int(np.round(edge_weight * target_vertices))
        else:
            # On the last edge, add enough points to meet the target exactly
            num_points_on_edge = target_vertices - accumulated_vertices

        new_vertices = np.linspace(start, end, num_points_on_edge, endpoint=False, dtype=int)
        new_polygon.extend(new_vertices)
        accumulated_vertices += len(new_vertices)

    new_polygon.append(polygon[-1])  # Ensure the polygon is closed

    return np.array(new_polygon)

# Plotting function
def plot_polygon(polygon, title):
    plt.plot(polygon[:, 0], polygon[:, 1], 'bo-')
    plt.fill(polygon[:, 0], polygon[:, 1], alpha=0.3)
    plt.title(title)
    plt.axis('equal')
    plt.show()

# Example usage
polygon = np.array([
    [0, 4],  # far Left
    [6, 5],
    [11, 5],
    [20, 7],
    [24, 6],
    [24, 4],
    [18, 1],
    [15, 0],
    [12, 0]  # top left on top of the angle
])

target_vertices = int(input("Please type the Target Verticies: "))

# Plot original polygon
plot_polygon(polygon, "Original Polygon")

# Increase resolution
new_polygon = increase_resolution(polygon, target_vertices)

# Plot new polygon
plot_polygon(new_polygon, "Polygon with Increased Resolution")

def check_overlapping_vertices(polygon):
    unique, counts = np.unique(polygon, axis=0, return_counts=True)
    overlapping = counts > 1
    num_overlaps = np.sum(counts[overlapping] - 1)
    if num_overlaps > 0:
        print(f"There are {num_overlaps} overlapping vertices.")
    else:
        print("No overlapping vertices.")
check_overlapping_vertices(new_polygon)
print("Length of the new polygon:", len(new_polygon))
