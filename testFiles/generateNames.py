import random

street_roots = ["Maple", "Oak", "Pine", "Cedar", "Elm", "Birch", "Willow", "Aspen", "Hickory", "Spruce",
                "Main", "High", "Broad", "Market", "Center", "Park", "Lake", "Hill", "River", "Sunset",
                "Meadow", "Valley", "Spring", "Autumn", "Winter", "Summer", "Forest", "Glen", "Harbor", "Bay",
                "Ocean", "Mountain", "Creek", "Ridge", "Stone", "Golden", "Silver", "Bronze", "Cherry", "Dogwood"]

suffixes = ["Street", "Avenue", "Boulevard", "Lane", "Road", "Drive", "Terrace", "Place", "Court", "Way",
            "Circle", "Trail", "Parkway", "Crossing", "View", "Heights", "Gardens", "Square", "Loop", "Hollow"]

street_names = []

for suffix in suffixes:
    for name in street_roots:
        street_names.append(name + " " + suffix)

with open("names.txt", "w") as file:
    file.write("\n".join(street_names))

print("File 'names.txt' has been")