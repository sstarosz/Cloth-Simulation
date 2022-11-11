# Cloth_Simulation
Cloth Simulation



# How to build
conan install .. -s build_type=Debug --build=missing




# To-Do
- [ ] Add download assets script
- [ ] Separate Gui from application (application should, could run without gui for testing purpose)
- [ ] Add python script for generating library documentation




# Validate format style
cmake --build $(tmpDir)/format-validate-build --target run-validate