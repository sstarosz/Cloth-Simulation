import math

# initial x position of the object
x = 0

# amplitude of the movement (how far the object will move to the right and left)
amplitude = 2

# angular frequency of the movement (how fast the object will move)
frequency = 0.1

# time variable, starts at 0
t = 0



# update loop
while True:
    # calculate the new x position of the object using the sin function
    x = amplitude * math.sin(frequency * t)

    print(x)

    # increment the time variable
    t += 0.1
