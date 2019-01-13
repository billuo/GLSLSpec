# GLSLSpec

Adapted from [glslViewer] for personal, educational purposes.

[glslViewer]: https://github.com/patriciogonzalezvivo/glslViewer

## Feature

* Support for more modern GLSL (up to #version 430) live editing.
* Detailed Runtime OpenGL shader program introspection.
* Built-in console, like the one [glslViewer] has.

## Dependencies
* Python(3, for now) interpreter to generate glad to load OpenGL.
* GLFW3 headers and libraries (version >= 3.0.0).
* C++17 conforming C++ compiler.

## TODO
* [ ] Finish up  C++ wrapper for OpenGL Object model.
* [ ] Support importing and rendering texture.
* [ ] Multiple viewports side-by-side with difference configuration for better contrast.
* [ ] Way to go...

## Usage
* View help on command line options with '--help'.
* Within built-in console, type 'help' for brief description on supported commands.

The following shaders are customizable:

| Render pass    | vertex   | tess. control | tess. evaluation | geometry | fragment | compute  |
|:---------------|----------|---------------|------------------|----------|----------|----------|
| background     | &#x2612; | &#x2612;      | &#x2612;         | &#x2612; | &#x2611; | &#x2612; |
| forward        | &#x2611; | &#x2611;      | &#x2611;         | &#x2611; | &#x2611; | &#x2611; |
| postprocessing | &#x2612; | &#x2612;      | &#x2612;         | &#x2612; | &#x2611; | &#x2612; |
| debug          | &#x2612; | &#x2612;      | &#x2612;         | &#x2612; | &#x2612; | &#x2612; |

The following uniforms/inputs are commonly supplied in all phases of rendering:
```GLSL
uniform ivec4 u_viewport; // position and size of viewport in window coord.
uniform ivec2 u_fbsize; // size of framebuffer, or resolution.
uniform ivec2 u_mpos; // position of mouse in window coord.
uniform vec3 u_camera; // position of camera in world coord.
uniform vec2 u_clip; // distances to near/far clip planes.
uniform float u_time; // current time in seconds.
uniform mat4 PVM; // xform from world to projection
uniform mat4 PV; // xform from view to projection
uniform mat4 VM; // xform from world to view
uniform mat3 NM; // inverse transpose of mat3(VM) for normal transformation
```

In background rendering, the following uniforms/inputs are additionally supplied:
```GLSL
// TODO
```

In postprocessing, the following uniforms/inputs are additionally supplied:
```GLSL
uniform sampler2D u_scene; // color texture of rendered scene (background+forward)
uniform sampler2D u_depth; // depth texture of rendered scene (background+forward)
```

Both background rendering and postprocessing are given two inputs:
```GLSL
in vec2 v_position; // NDC coord. of this fragment; [-1, 1]x[-1, 1]
in vec2 v_texcoord; // Texture coord. of this fragment when it goes into framebuffer; [0, 1]x[0, 1]
```
