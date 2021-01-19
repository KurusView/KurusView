# Group 21 Computing Project - KurusView

![image](https://user-images.githubusercontent.com/38017709/104436085-11a57700-5585-11eb-979e-47145ab32988.png)

---

*Development of a small library capable of loading custom 3D model files as well as STL models, utilizing Qt for the GUI and VTK for 3D modelling, while learning the importance of software engineering and code management through GitHub.*

### Overview

Software on this repository can load, render, transform and save .stl models.

**Implemented Classes** (marked if working):
- [x] Class 1: 3D Vector
- [x] Class 2: Cell
- [x] Class 3: Material 
- [x] Class 4: Model
- [x] Class 5: Matrix  
- [x] Class 6: ModelRenderer

    

### Coding style
Custom model loading library classes start with M, eg:
```cpp
MCell();
MVector();
```

To avoid confusion with similarly named dependency objects.


### Branches

> **master**: Fully working .stl model loading

> **opengl_development**: master + openGL rendering POC.

> **SDL/mouse_control**: opengl_development + mouse controlled rotation, translation, and scaling POC. Zoom on cursor in development.

### Folder structure
    .
    ├── include/                # Library header files
    ├── lib/                    # Library source files
    ├── models/                 # Sample .stl models
    └── src/                    # Main program with testing code

    
### Requirements
* CMAKE > 3.1.0
* Qt5
* SDL2
* GLEW
* GLm


### Documentation
Code comments follow doxygen syntax to automatically generate documentation. 

Commit comments are used as well as issues to document development.
