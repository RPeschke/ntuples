

add_executable(text1 ${CMAKE_CURRENT_SOURCE_DIR}/test1.cc)

target_link_libraries(text1 ntuples)
set_property(TARGET text1 PROPERTY CXX_STANDARD 17)