cmake_minimum_required(VERSION 3.16)
project(Obj_Model_Viewer)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(CMAKE_AUTOMOC ON) 

set(SRCS
    main.cc
    MainWindow.cc
    GLWidget.cc
)

add_executable(app.out ${SRCS})

target_link_libraries(app.out
Qt6::Widgets
Qt6::OpenGLWidgets
)