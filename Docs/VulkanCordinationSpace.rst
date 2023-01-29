

This software cordinate space:

Camera space:
x: negative left    -> positive right
y: negative bottom  -> positive up
z: negative forward -> positive back.

Screen space:

x: negative left -> positive right   (-1)
y: negative top  -> positive down


normalized device coordinates only contain primitives within the unit cube from 
(-1,−1,0)T to (1,1,1)T. 


ramebuffer space’s x and y coordinates range from 
[0..w) and [0..h), with w referring to the framebuffer’s horizontal resolution, and h referring to the framebuffer’s vertical resolution. 




The z coordinates refer to depth values in [0,1] range.


Vulkan NDC