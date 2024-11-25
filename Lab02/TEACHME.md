# Lab 2: Introduction to 3D Concepts in OpenGL

This lab introduced key 3D graphics concepts using OpenGL. The session focused on creating and transforming 3D objects, with practical tasks to reinforce the learning.

### Topics Covered

#### 3D Object Creation and Transformations
- **3D Object Basics**: We learned how to define 3D objects and apply transformations such as translation, rotation, and scaling.
- **3D Cube**: A cube was created by defining its vertices in 3D space.

#### Vertex Coloring
- Each vertex of the cube was assigned a different color, demonstrating how to interpolate colors across the cube's surfaces.

#### Projection Methods
- **Perspective Projection**: Simulates how objects appear smaller as they get farther from the viewer, creating a realistic depth effect.
- **Orthographic Projection**: Maintains object sizes regardless of distance, often used in technical drawings.

#### Window and Viewport
- **Window**: Represents the application interface.
- **Viewport**: Defines the area of the window where rendering occurs.
- **Differences**: Understanding how the window size and viewport size affect rendering output.

#### Triangle Winding Order
- Importance of vertex order in 3D triangles:
  - **Clockwise (CW)**: Typically indicates back-facing surfaces.
  - **Counter-Clockwise (CCW)**: Indicates front-facing surfaces, often used for rendering.

#### Vertex Buffer Objects (VBO) and Vertex Buffer Descriptors (VBD)
- **VBO**: Used to store vertex data on the GPU for efficient rendering.
- **VBD**: Configures how vertex data is read and processed.

### Task: Drawing a Computer Setup
As part of the lab, we applied the learned concepts to create a 3D model of a computer setup, consisting of:
1. **Monitor**
2. **Keyboard**
3. **Mouse**
4. **Multiple Soundbox**
5. **Base**

Each component was modeled using basic geometric shapes, demonstrating:
- Proper vertex placement in 3D space.
- Applying colors and transformations to individual objects.

### Outcome
This lab provided a strong foundation in 3D graphics programming with OpenGL. By the end of the session, we were able to create complex scenes and understand the principles of 3D rendering.

---

