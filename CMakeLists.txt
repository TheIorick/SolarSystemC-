cmake_minimum_required(VERSION 3.16)

project(SolarSystem LANGUAGES CXX) # Убрал VERSION 0.1

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

qt_standard_project_setup()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_add_executable(SolarSystem
    glwidget.cpp glwidget.h
    main.cpp
    mainwindow.cpp mainwindow.h
    celestialbody.h celestialbody.cpp
    planet.h planet.cpp
    sun.h sun.cpp
    resources.qrc
    satellite.h satellite.cpp
    .gitignore

)

# Явное добавление файла ресурсов для CMake < 3.16
if(CMAKE_VERSION VERSION_LESS "3.16")
    set_source_files_properties(resources.qrc PROPERTIES
        "QT_RCC_CPP_CREATE_CPP" ON
        "QT_RCC_CPP_H_FILENAME" qrc_resources.h
        "QT_RCC_CPP_CPP_FILENAME" qrc_resources.cpp
    )
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp
        COMMAND ${RCC_EXECUTABLE} -name resources ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc -o ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
    )
    target_sources(SolarSystem PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp)
endif()

set_target_properties(SolarSystem PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(SolarSystem PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
    opengl32
)

install(TARGETS SolarSystem
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SolarSystem
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
