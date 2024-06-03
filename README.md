# Things Betwixt
[README na srpskom](README-sr.md). <br>
An OpenGL project for the course Computer graphics in Faculty of Mathematics, University of Belgrade.

## Borrowed code
The `main.cpp` and most header files in `include` directory (except `stb_image.h`) I wrote myself with help from [JoeyDeVrie's](https://github.com/JoeyDeVries/LearnOpenGL)
repository. Everything else, including the structure of the repository, `CMake` file, `.gitignore` etc. was borrowed from the same repository.

## Where I got my resources
Most models are from [this site](https://poly.pizza/). <br>
Flowers:
- (https://poly.pizza/m/dYQFgjU5Eqx)
- (https://poly.pizza/m/eLVv17bTyB-)
- (https://poly.pizza/m/1baaAXcdzZ)
- (https://poly.pizza/m/fg5U0dl1Sd3)

Trees:
- ()

- [Shrine]()
- [Lantern](https://poly.pizza/m/37EufrdA2UB)
- [Stone path](https://poly.pizza/m/0vAJVcMEFdv)
- [Frogs](https://poly.pizza/m/cwyNyIba6WE)
- [Grass](https://poly.pizza/m/eFUpFgjCf0p)
- [Bush](https://poly.pizza/m/TSbIxkDtxF)

## Progress
| Functionality                | Status  | Progress | Comment                                                    |
|------------------------------|---------|----------|------------------------------------------------------------|
| Shader compiling and linking | Working | Done     | -                                                          |
| Camera movement              | Working | Done     | -                                                          |
| Camera looking               | Working | Done     | -                                                          |
| Model loading                | Working | Done     | -                                                          |
| Point lighting               | Working | Done     | -                                                          |
| Directional lighting         | Working | Done     | -                                                          |
| Spotlight                    | Working | Done     | -                                                          |
| Blinn-Phong's model          | Working | Done     | -                                                          |
| Blending                     | Working | Done     | -                                                          |
| Face culling                 | Working | Done     | May look broken because some vertices were poorly defined. |
| Skybox class                 | Working | Done     | -                                                          |
| Skybox                       | Working | Done     | -                                                          |
| HDR                          | Working | Done     | -                                                          |
| Bloom                        | Working | Done     | -                                                          |
| SimpleShapes class           | Working | Done     | -                                                          |

## To-do
- [ ] finish this README
- [x] main and basic functionalities (12.04.2024.)
- [x] draw a rectangle (12.04.2024.)
- [x] draw a cube (12.04.2024.)
- [x] movement (14.04.2024.)
- [x] shader class, some uniform sets undefined (14.04.2024.)
- [x] camera class (14.04.2024.)
- [x] error handling (16.04.2024.)
- [x] info logs (16.04.2024.)
- [x] some keyboard actions (such as exiting) (14.04.2024.)
- [x] various callback functions (16.04.2024.)
- [x] directional light (15.04.2024.)
- [x] point light (15.04.2024.)
- [x] spotlight direction issue (16.04.2024.)
- [x] spotlight toggle (16.04.2024.)
- [x] more keyboard actions (inc/decr speed, sensitivity, toggle x/y axis inversion...) (16.04.2024.)
- [x] face culling (17.04.2024.)
- [x] blending (17.04.2024.)
- [x] blinn-phong (17.04.2024.)
- [x] toggle effects (blending, face culling, blinn-phong) (17.04.2024.)
- [x] bloom (30.04.2024.)
  - [x] framebuffers (29.04.2024.)
  - [x] hdr (29.04.2024.)
- [x] skybox (30.5.2024.)
- [x] model class (3.6.2024.)
- [x] SimpleShapes class (3.6.2024.)
- [x] water (3.6.2024).
- [x] fireflies (3.6.2024.)
