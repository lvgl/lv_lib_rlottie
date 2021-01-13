# lv_lib_rlottie
Allows to use Rlottie animations in LVGL. Taken from [base repository](https://github.com/ValentiWorkLearning/lv_rlottie)

Uses [Samsung/rlottie](https://github.com/Samsung/rlottie) library with C-api header

Compatible with LVGL main branch. Tested with the latest LVGL/master branch

Rlottie build requires C++14-compatible compiler, optionally CMake 3.14.

## Get started

- Download or clone this repository

- Run `git submodule update --init` for fetching dependencies

- For running simulator visit [base repo](https://github.com/ValentiWorkLearning/lv_rlottie)

- Rlottie build configuration supports CMake as well, but a few extra build flags must be passed:

  ```makefile
  set(LOTTIE_MODULE OFF)
  set(LOTTIE_THREAD OFF)
  set(BUILD_SHARED_LIBS OFF)
  ```

- Also, you can generate content for LVGL library for either CMake integration or your preferable build system. See [Simulator CMakeLists](https://github.com/ValentiWorkLearning/lv_rlottie/blob/master/graphics/lvgl_lib/lvgl_library/CMakeLists.txt) with up-to-date for major release 7 CMake-buildsystem. 

- 

## Use Rlottie from external file

```c
lv_obj_t* rlottie_animation = lv_rlottie_create_from_file(parent,width,height, "Path/to/resource/insta_camera.json");
```



## Use Rlottie from raw string data

```c
//Create characters array for the image data
static const char FingerprintRaw[] ="{\"v\":\"5.1.16\",\"fr\":60,\"ip\":0,\"op\":154,\"w\":800,\"h\":600,\"nm\":\"Fingerprint... a lot of JSON items";

// Use it with corresponding API function
lv_obj_t* rlottie_anim_fingerprint_raw = lv_rlottie_create_from_raw(parent, width, height, FingerprintRaw);
```



## JSON escaping

For using raw strings it's necessary to escape an image data. For this purpose the following escape tool can be used:

https://www.freeformatter.com/json-escape.html

