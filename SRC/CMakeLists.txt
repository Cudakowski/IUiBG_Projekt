if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

cmake_minimum_required(VERSION 3.16)
project(ProjektIUiBG CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake zrobiony pod biblioteki statyczne
set(SFML_STATIC_LIBRARIES TRUE)


find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
find_package(OpenGL REQUIRED)


set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui-SFML.cpp
    imgui/ImGuiFileDialog.cpp
)

add_library(imgui_sfml STATIC ${IMGUI_SOURCES})

# udostępnienie imgui/ do includowania
target_include_directories(imgui_sfml PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)




target_compile_definitions(imgui_sfml PUBLIC SFML_STATIC)


target_link_libraries(imgui_sfml PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
    OpenGL::GL
)

#to takie do uruchamiania przykładów
#add_executable(przyklad przyklady/06-suwak_i_tekst.cpp)
#target_link_libraries(przyklad PRIVATE imgui_sfml)

set(APP_SOURCES
    src/main.cpp
    src/view/view.cpp
    src/image/image.cpp
    src/app/app.cpp
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE imgui_sfml)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/app
    ${CMAKE_CURRENT_SOURCE_DIR}/include/image
    ${CMAKE_CURRENT_SOURCE_DIR}/include/view
)

# prosty test load/save BMP (bez GUI)
add_executable(test_bmp
    src/image/test_bmp.cpp
    src/image/image.cpp
)
target_include_directories(test_bmp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/image
)
target_compile_definitions(test_bmp PRIVATE
    SAMPLES_DIR="${CMAKE_SOURCE_DIR}/../assets/samples"
    SFML_STATIC
)
target_link_libraries(test_bmp PRIVATE
    SFML::Graphics
    SFML::System
)