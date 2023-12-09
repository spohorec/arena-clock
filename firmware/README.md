# Arena Clock Firmware

This firmware uses [platformio](https://platformio.org/) for build management. It runs on a [ST Nucleo-L412KB](https://www.st.com/en/evaluation-tools/nucleo-l412kb.html) on a custom carrier board (see [pcb](../pcb/)).

The CubeMX project configuration is generated in headless mode during the project build. You must change the value of the `CUBE` varibale in `lib/cube/generate_cube_code.sh` to point to your installation of STM32CubeMX. Note that this script will likely not work on a Windows machine without modification. If developing on Windows, the call to `generate_stm32_cube_sources()` in `extra.py` can be commented out and the required code can be manually generated using the STM32CubeMx GUI and then moved to `lib/cube/Src/` and `lib/cube/Inc`. The patch file adds an include directive to include `main_loop.h` and call `MainLoop()` after initialization is completed in `main()`. This can also be added manually.
