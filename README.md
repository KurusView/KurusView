<!--
[![Build Status](https://travis-ci.com/KurusView/2020_GROUP_21.svg?branch=master)](https://travis-ci.com/KurusView/2020_GROUP_21)
-->

<div align="left">
&nbsp;&nbsp;

  <!-- Build Status -->
  <a href="https://travis-ci.com/KurusView/2020_GROUP_21">
    <img src="https://travis-ci.com/KurusView/2020_GROUP_21.svg?branch=master"
      alt="Build Status" />
  </a>
  <!-- SemVer Version -->
  <a href="https://github.com/KurusView/2020_GROUP_21">
    <img src="https://img.shields.io/badge/Version-0.1.0-blue.svg"
      alt="SemVer Version" />
  </a>

</div>

# KurusView

KurusView is a cross-platform visualisation tool for three-dimensional models. 

![main_sidePanel](https://user-images.githubusercontent.com/38017709/118720858-6e15ed00-b82a-11eb-801c-28a2963f58e3.png)


### Iinteraction with the models is supported through the following features:

* Open multiple models in the same or different windows.
* Move models freely around every axis.
* Modify and edit multiple models at once.
* Apply shrink and/or clip filters.
* Measure model dimensions along predefined or custom axis.
* Change model, background, and back-face colour independently.
* Change the structure of the model to a wireframe.
* View the individual points of the model.
* Apply 3D gridlines encapsulating any model.
* Calculation of model statistics such as:
    -	Volume
    -	Weight
    -	Centre of Gravity
    -	Number of Cells
    -	Density
* Adjust lighting, model opacity and specularity.
* Save per-model view transformations (colours, position, etc) to proprietary .kurus files.

> Light and dark modes are available and on Windows switching is automatic matching Windows app settings.


&nbsp;

---

## Requirements
- CMake >= 3.3
- Qt 5
- VTK 8.2.0

These requirements are only relevant if compiling from source. [Installers](#Install) are available.

## Install
`zip` and `exe` installers are available for Windows, and `zip`, `tgz` and `deb` for Linux. 


## Compile from source
``` bash
$ cmake -DENABLE_TESTS=OFF build/
$ cmake --build build/ --target KurusView
```

Or without the GUI:
``` bash
$ cmake -DENABLE_TESTS=OFF build/
$ cmake --build build/ --target KurusView_LIB
```

## Repo tree
```
.
├── include/                # Project Headers
|   ├── Cells/                  # MCell Children Headers
|   ├── ModelRenderer/	        # Semester 2 GUI Headers
|   └── Utils/                  # Internally Used Utility Headers
├── lib/                    # Library Source Files
|   ├── Cells/                  # MCell Children Source Files
|   ├── ModelRenderer/	        # Semester 2 GUI Source Files
|   ├── Utils/                  # Internally Used Utilities
|   └── KurusViewLib.cmake      # Library CMake
├── models/                 # Example Models (.kurus, .mod and .stl)
├── packaging/              # Packaging CMake Dependencies
|   └── CMakeLists.txt          # Packaging CMake
├── Resources/              # Icons, Images and Stylesheets
├── src/                    # Main Function Source File
|   └── KurusView.cmake         # Main Function CMake
├── tests/                  # Google Test Tests
|   └── CMakeLists.txt          # Tests CMake
├── googletest/             # Git Submodule: Google Test
├── doxygen-awesome-css/    # Git Submodule: Doxygen Awesome
└── CMakeLists.txt          # Main CMakeLists
```

## Documentation
Doxygen formatted comments are used in all header files and docs are hosted [here](https://kurusview.github.io/2020_GROUP_21/). The [theme](https://github.com/jothepro/doxygen-awesome-css) used is by [@jothepro](https://github.com/jothepro) :rocket:.

![docs](https://user-images.githubusercontent.com/38017709/118726672-ef24b280-b831-11eb-957b-c3ce2fc8e03e.png)


## Testing and Continuous Integration
This repository uses [Google Test](https://github.com/google/googletest) and [Travis CI](https://travis-ci.org/). To run tests yourself:
``` bash
$ cmake -DENABLE_TESTS=ON build/
$ make test
```


## Other views

Welcome window             |  Settings
:-------------------------:|:-------------------------:
![](https://user-images.githubusercontent.com/38017709/118721895-b71a7100-b82b-11eb-84a5-9ae32dd6eb9e.png)  |  ![](https://user-images.githubusercontent.com/38017709/118722592-9bfc3100-b82c-11eb-9960-e9ea6c7f897d.png)

<!--
<p float="left">
  <img src="https://user-images.githubusercontent.com/38017709/118721895-b71a7100-b82b-11eb-84a5-9ae32dd6eb9e.png" width="500"/>
  <img src="https://user-images.githubusercontent.com/38017709/118722592-9bfc3100-b82c-11eb-9960-e9ea6c7f897d.png" width="500"/> 
</p>
-->




<!--
![main_sidePanel](https://user-images.githubusercontent.com/38017709/118720858-6e15ed00-b82a-11eb-801c-28a2963f58e3.png)
![main](https://user-images.githubusercontent.com/38017709/118718910-18404580-b828-11eb-9b42-9970cd08682a.png)
![menu](https://user-images.githubusercontent.com/38017709/118718917-1aa29f80-b828-11eb-9e6e-6eb4df1fabf7.png)
![welcome_2](https://user-images.githubusercontent.com/38017709/118721895-b71a7100-b82b-11eb-84a5-9ae32dd6eb9e.png)
![settings_2](https://user-images.githubusercontent.com/38017709/118722592-9bfc3100-b82c-11eb-9960-e9ea6c7f897d.png)
-->
