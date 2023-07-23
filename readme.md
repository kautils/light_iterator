
### Platform
* mingw 

### Outline
* assumed to be used if not wanted to expose the definition of std::container outside source file.
* assumed to be used inside same module (or between modules that are same compiler version).   
* type-check is not perfect because it is done by the size of object, but prenty of avoiding type-miss like between std::map and std::unordered_map. 
* throw exception at setup process when different types are passed. 
