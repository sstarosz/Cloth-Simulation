# Cloth_Simulation
Cloth Simulation



# How to build
Download C++ compiler used in your operating system
Download Qt (Make sure that path to qt in CMakePresets.json is corresponding to your qt installation)
Download vcpkg and put it folder above this project or put it in another folder and change CMakePresets.json toolchain path to your localization)




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



# Idea
- [ ] Add Compute state that will postprocess the result image. Separate command buffer and computation pipeline is needed for that

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




## Visual studio and clang-tidy analysis

CMakePresets sets visual studio and clang-tidy of analysis, but to really turning on clang tidy it is necessary to set clang ```CMAKE_CXX_CLANG_TIDY``` in CMakeLists.txt

ClangTidy in CmakePresets.json works only when clang-cl is used as compiler