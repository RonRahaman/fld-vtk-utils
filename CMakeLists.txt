cmake_minimum_required(VERSION 3.16)
project(fld-vtk-utils CXX)
set(CMAKE_CXX_STANDARD 14)

#add_subdirectory(contrib/vtk)

find_package(VTK REQUIRED)
vtk_module_config(VTK
        vtkCommonCore
        vtkFiltersSources
        vtkInteractionStyle
        vtkInteractionWidgets
        vtkRenderingOpenGL2
        vtkRenderingLOD
        vtkRenderingLabel
        vtkIOParallel
        )
include(${VTK_USE_FILE})

#add_executable(my_cylinder my_cylinder.cpp)
#target_link_libraries(my_cylinder ${VTK_LIBRARIES})

add_executable(reader_demo demos/reader_demo.cpp src/make_lagrange_hex.cpp)
target_link_libraries(reader_demo PUBLIC ${VTK_LIBRARIES})
target_include_directories(reader_demo PUBLIC include)

add_executable(main src/main.cpp src/make_lagrange_hex.cpp)
target_link_libraries(main PUBLIC ${VTK_LIBRARIES})
target_include_directories(main PUBLIC include)

add_executable(lagrange_tet_demo demos/lagrange_tet_demo.cpp)
target_link_libraries(lagrange_tet_demo PUBLIC ${VTK_LIBRARIES})
target_include_directories(lagrange_tet_demo PUBLIC include)

add_executable(lagrange_hex_demo demos/lagrange_hex_demo.cpp)
target_link_libraries(lagrange_hex_demo PUBLIC ${VTK_LIBRARIES})
target_include_directories(lagrange_hex_demo PUBLIC include)

add_executable(lagrange_hex_many_demo demos/lagrange_hex_many_demo.cpp)
target_link_libraries(lagrange_hex_many_demo PUBLIC ${VTK_LIBRARIES})
target_include_directories(lagrange_hex_many_demo PUBLIC include)

add_executable(ugrid demos/ugrid.cpp)
target_link_libraries(ugrid PUBLIC ${VTK_LIBRARIES})
target_include_directories(ugrid PUBLIC include)
