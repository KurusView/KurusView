

# Any line starting with a '#' is considered to be a comment and ignored

# any line starting with a 'm' is a material definition a unique index for
# the material is given next, followed by its density followed by its name

# e.g. material 0 has a density of 8960 and is called copper 
m 0 8960 b87333 copper

# material 1 has a density of 2700 and is called aluminium
m 1 2700 222222 aluminium 

# Any line starting with a 'v' is a vertex (a vector)
# the 1st, 2nd and 3rd are the x, y and z coordinates

# e.g. vertex 0 is positioned at 0., 0., 0. (the . is just to emphasize floating point numbers)
v 0 0. 0. 0.
v 1 1. 0. 0.
v 2 1. 1. 0.
v 3 0. 1. 0.
v 4 0. 0. 1.
v 5 1. 0. 1.
v 6 1. 1. 1.
v 7 0. 1. 1.

# 8 verteces have now been defined, these can be used as the corners
# of a hexahedral cell

# a cell is defined by a line beginning with a 'c'. The second parameter
# the cell index, the third is a character (either h - hexahedral, p - 
# pyramid, t - tetrahedral). Thie 4th in the index of a material that the
# cell is made of and the remaining  numbers state which node defines 
# each corner.

# e.g. cell 0 is a hexahredal, made from material 0 (copper) which and 
# has nodes 0 - 7 define its verteces
c 0 h 0 0 1 2 3 4 5 6 7



## Second example
# define some more verteces for a second tetrahedral (4 more)
v 8 5. 5. 0.
v 9 6. 5. 0.
v 10 6. 6. 0.
v 11 5.5 5.5 0.5

# cell 1 is a tetrahedral made from material 1
# (aluminium) defined by verteces 8,9,10,11
c 1 t 1 8 9 10 11
