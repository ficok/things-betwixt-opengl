# Things Betwixt
[README na srpskom](README-sr.md). <br>
An OpenGL project for the course Computer graphics in Faculty of Mathematics, University of Belgrade.

## Borrowed code
The `main.cpp` and most header files in `include` directory (except `stb_image.h`) I wrote myself with help from [JoeyDeVries'](https://github.com/JoeyDeVries/LearnOpenGL)
repository. Everything else, including the structure of the repository, `CMake` file, `.gitignore` etc. was borrowed from the same repository.

## Keys
- Movement: **W A S D** for forward, left, back, right; **Space** for up, **Shift** for down
- **Escape** to quit
- **F** to toggle flashlight
- **- and =** to decrease/increase movement speed
- **\[ and \]** to decrease/increase mouse sensitivity
- **X** to invert x-axis; **Y** to invert y-axis
- **C** to toggle face culling
- **B** to toggle blending (transparency)
- **P** to switch between Blinn-Phong and Phong lighting model
- **I** to toggle bloom and hdr
- **Q and E** to decrease/increase exposure

## Screenshot
[Demo video](https://youtu.be/peo56Z8w0GY). Set quality to 1080p.
![Things Betwixt](ThingsBetwixtScreenshot.png)

## Where I got my resources
Most models are from [this site](https://poly.pizza/) and [this site](https://sketchfab.com/feed). <br>
Flowers:
- (https://poly.pizza/m/dYQFgjU5Eqx)
- (https://poly.pizza/m/eLVv17bTyB-)
- (https://poly.pizza/m/1baaAXcdzZ)
- (https://poly.pizza/m/fg5U0dl1Sd3)

Trees:
- (https://sketchfab.com/3d-models/low-poly-tree-pack-ea6e844754da494a9c38501b4fff92ad)
- (https://sketchfab.com/3d-models/low-poly-tree-concept-e815f8acd6d34528a82feef38d5af880)
- (https://sketchfab.com/3d-models/tree-stump-14c5d090d54d47cd831c43244f82e242)

Other:
- [Shrine](https://sketchfab.com/3d-models/shrine-61c478f77ea14759b61dd8938e4015a6)
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
- [x] finish this README (03.06.2024.)
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
- [x] skybox (30.05.2024.)
- [x] model class (03.06.2024.)
- [x] SimpleShapes class (03.06.2024.)
- [x] water (03.06.2024).
- [x] fireflies (03.06.2024.)
