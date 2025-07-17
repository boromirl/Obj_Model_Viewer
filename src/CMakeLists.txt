cmake_minimum_required(VERSION 3.5)

project(3DViewer)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)

# add source files
add_executable(3DViewer
    main.cc
    ui/main_window.cc
    ui/render_widget.cc
    ui/settings_dialog.cc
    ui/shader.cc
    engine/model_engine.cc
    utils/obj_parser.cc
    controller/controller.cc
    S21Matrix/s21_matrix.cc
)

target_link_libraries(3DViewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)

