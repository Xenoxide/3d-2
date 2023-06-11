# 3d-2

This project is an experiment in 3D projection. I am learning about C and 3D rendering at the same time. It uses SDL to take input and draw to the screen. I'll just say, I've learned a lot doing this.

For the corresponding presentation, [Xenoxide/3d-2-presentation](https://github.com/Xenoxide/3d-2-presentation).

Compile using:
```sh
gcc main.c t_obj.c t_math.c -lSDL2 -lSDL2main -lm
```
Run with ./a.out OBJECT.obj WIDTH HEIGHT
Compile unit tests using:
```sh
gcc unit_tests.c t_obj.c t_math.c -lm
```
