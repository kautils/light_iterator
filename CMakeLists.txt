
cmake_minimum_required(VERSION 3.20)
project(Kautil LANGUAGES C CXX VERSION 0.0.0.0)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_CURRENT_LIST_DIR}/kautil/light_iterator/light_iterator.cmake)

# not yet install script
# after install header , source and cmake are separated 
# it is possible to use without CMakeList.txt, in such case, copy and paste the directory kautil. and under then include light_iterator.cmake on some other cmake project

