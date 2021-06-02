# Rlottie library for LVGL
Allows to use Rlottie animations in LVGL. Taken from [base repository](https://github.com/ValentiWorkLearning/lv_rlottie)
Uses [Samsung/rlottie](https://github.com/Samsung/rlottie) library with C-api header.

Rlottie build requires C++14-compatible compiler, optionally CMake 3.14.

## Build setup
`lv_lib_rottile` can be built with CMake.

1. Download or clone this repository with `git clone https://github.com/lvgl/lv_lib_rlottie.git --recurse-submodules` next to the `lvgl` folder. If you have downlaoded it run `git submodule update --init` for fetching the dependencies.
2. In you `CMakeFile.txt`
  - add:
```makefile
  set(LOTTIE_MODULE OFF)
  set(LOTTIE_THREAD OFF)
  set(BUILD_SHARED_LIBS OFF)
```
  - include `lv_lib_rlottie`s CMakeFile with `include(${CMAKE_CURRENT_SOURCE_DIR}/lv_lib_rlottie/CMakeLists.txt)`
  - link `lv_lib_rlottie` with your target. E.g. `target_link_libraries(my_target PRIVATE lv_lib_rlottie)`

## Usage

You can use animation from raw data (text) or files. In either case first include `lv_lib_rlottie/lv_rlottie.h`.

The `width` and `height` os the object be set in the *craete* function. The animation will be scaled accordingly.
 
### Use Rlottie from raw string data

`lv_rlottie_example_fingerprint.c` cotains an example animation in raw string format. It can be used like this:

```c
extern const char FingerprintRaw[];
lv_obj_t* r1 = lv_rlottie_create_from_raw(parent, width, height, FingerprintRaw);
```
#### JSON escaping

For using raw strings it's necessary to escape an image data. For this purpose the following escape tool can be used:
https://www.freeformatter.com/json-escape.html


### Use Rlottie from external file

You can use `card.json` to at first:

```c
  lv_obj_t* r2 = lv_rlottie_create_from_file(lv_scr_act(), width, height, "../lv_lib_rlottie/card.json");
```


## Getting aniamtions

Lottie is standard and popular format so you can find many animation files on the web.
For example: https://lottiefiles.com/ 
