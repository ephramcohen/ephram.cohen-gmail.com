project to scale the tray of the thing https://www.thingiverse.com/thing:3085808 Laundry Detergent Cup Drain by Clarson1.

the threads are already correct on the stl, but the cup holder was too small. I needed to scale the cup up by 8%, but leave the threads alone. I also needed to make the scaling smooth so that the inner channel would remain a closed form, and the outside shape aesthetic would be maintained. 

thread scale factor 100%
cup scale factor 108%
the neck between the threads and the cup would scale along a smooth gradient between 100% and 108%.

I tried using meshmixer to do that but could not figure out the smooth scaling that the neck needed. So, I wrote a program to apply the 
necessary scalings.

This is a quick and dirty program, there is no command line error checking, or any other error checking for that matter.

compiles with gcc.






