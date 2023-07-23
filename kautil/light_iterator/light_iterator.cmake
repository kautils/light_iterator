


set(__t light_iterator)
add_library(${__t} STATIC)
unset(srcs)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
target_sources(${__t} PRIVATE ${srcs})
add_library(kautil::light_iterator ALIAS ${__t})

set(__t tmain_light_iterator)
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_compile_definitions(${__t} PRIVATE TMAIN_LIGHT_ITERATOR)
target_link_libraries(${__t} PRIVATE kautil::light_iterator)





