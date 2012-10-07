Our implementation of free currently causes segmentation faults, so it has been commented out to allow the program to function without error. Our design for free was for it to do the following:
1) Load the size of the block
2) Create a header struct in the block
3) Locate the adjacent free blocks
4) Update links to add the new block
