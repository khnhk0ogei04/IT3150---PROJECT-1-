import matplotlib.pyplot as plt
import re

def parse_file(file_path):
    with open(file_path, 'r') as f:
        data = f.read()
    route_pattern = r"Route \d+: ([\d\s\-\>]+)"
    route_matches = re.findall(route_pattern, data)

    routes = []
    for match in route_matches:
        route = [int(city.strip()) for city in match.split(" -> ")]
        routes.append(route)
    coord_pattern = r"Route \d+: ((?:\([^)]*\) -> )*\([^)]*\))"
    coord_matches = re.findall(coord_pattern, data)

    coords = []
    for match in coord_matches:
        route_coords = []
        points = match.split(" -> ")
        for point in points:
            x, y = map(float, point.strip("()").split(", "))
            route_coords.append((x, y))
        coords.append(route_coords)

    return routes, coords

def plot_routes(routes, coords):
    plt.figure(figsize=(10, 8))
    cmap = plt.cm.get_cmap("tab20")
    colors = [cmap(i % cmap.N) for i in range(len(routes))]
    depot = coords[0][0]  
    depot_x, depot_y = depot

    for idx, (route, coord) in enumerate(zip(routes, coords)):
        x_coords = [point[0] for point in coord]
        y_coords = [point[1] for point in coord]
        plt.plot(x_coords, y_coords, marker='o', label=f"Route {idx + 1}", color=colors[idx % len(colors)])
        for city_id, (x, y) in zip(route, coord):
            plt.text(x, y, f"{city_id}", fontsize=8, ha='right')

    plt.scatter(depot_x, depot_y, color='black', label='Depot (1)', s=100, zorder=5)

    plt.title("Vehicle Routes Visualization")
    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.legend()
    plt.grid()
    plt.show()

file_path = r"C:\Users\Lenovo\Downloads\Vrp-Set-A\clarkewright\clarkewright_A-n32-k5.txt"

# Đọc file và trực quan hóa
routes, coords = parse_file(file_path)
plot_routes(routes, coords)
