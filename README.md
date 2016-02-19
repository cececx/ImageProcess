#Content-Aware Image Resizing
Resize an image without distortion for display on cell phones and web browsers.<br>
This project is an implementation of a different image resizing approach called seam carving. This resizing method is presented by Avidan, S and Shamir, A ([Seam Carving for Content-Aware Image Resizing](http://graphics.cs.cmu.edu/courses/15-463/2007_fall/hw/proj2/imret.pdf)).
###Approach
* Assign energy to each pixel
  * Use dual gradient energy function
* Find the seam with minimum energy
  * Process the image as an edge-weighted Directed Acyclic Graph (DAG)
  * Find the shortest path using Dynamic Programming 
* Remove the seam 

###Samples
![image](https://github.com/cecilia-xc/ImageProcess/blob/master/img/1.bmp)
![image](https://github.com/cecilia-xc/ImageProcess/blob/master/img/out1.bmp) <br>
![image](https://github.com/cecilia-xc/ImageProcess/blob/master/img/2.bmp)
![image](https://github.com/cecilia-xc/ImageProcess/blob/master/img/out2.bmp)
