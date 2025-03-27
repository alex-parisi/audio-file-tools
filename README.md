# Audio File Tools

This repository contains a collection of tools that can be used to interact with
audio files. 

---

### Usage:

To integrate this library into your project, it's easiest to use CMake:

```cmake
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        AudioFileTools
        GIT_REPOSITORY https://github.com/alex-parisi/audio-file-tools.git
        GIT_TAG        main
)
FetchContent_MakeAvailable(AudioFileTools)

target_link_libraries(<target_name> PRIVATE AudioFileTools)
```

Then import and use the library in your project:

```cpp

```

---

### Notes: 

Currently, there is support for reading and writing to `.wav` files with the
following configurations:

- Format:
    - `float32`
    - `pcm8`
    - `pcm16`
    - `pcm24`
    - `pcm32`


- Sample Rates:
    - `8000` 
    - `11025`
    - `16000`
    - `22050`
    - `32000`
    - `44100`
    - `48000`
    - `96000`
    - `176400`
    - `192000`
    - `352800`
    - `384000`

The `WavReader` and `WavWriter` classes are used to read and write `.wav` files
respectively. There is support for mono, stereo, and multichannel audio files.