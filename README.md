# Cloth_Simulation
Cloth Simulation



# How to build
conan install .. -s build_type=Debug --build=missing


# Progress (Current Status)
* ObjImporter 0.1 (Can import cube and sphere "Primitive Shapes")


# Current Progress
**Add word cordination**

- **What is needed**
- [x] Seprate Graphics Pipeline for line primitives?
- [x] What is need to Graphics Pipeline to create
- [ ] Cache for Graphics Pipline

- **What to do**
 Understand work corination and math behaind it
 model view project
 cordination system in vulkan

- **Result**





# To-Do
- [ ] Understanding Vertex buffer to add objects in runtime
- [ ] Understand buffers and memory
- [ ] Adding Outliner
- [ ] Change exception handling to result handling
- [ ] Move api to RAII


- [ ] Add download assets script
- [ ] Separate Gui from application (application should, could run without gui for testing purpose)
- [ ] Add python script for generating library documentation




# Compilation To-Do
* Add /LTCG	/GL /Gy /GF



# Validate format style
cmake --build $(tmpDir)/format-validate-build --target run-validate