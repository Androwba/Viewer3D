cmake_minimum_required(VERSION 3.5)

set(PROJECT 3DViewer)
project(${PROJECT} VERSION 2.0 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_definitions(-w)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3dparty)

set(SRC_DIR
        ${SOURCE_DIR}/controller
        ${SOURCE_DIR}/model
        ${SOURCE_DIR}/view
        ${EXTERNAL_DIR}/QtGifImage/gifimage
        ${EXTERNAL_DIR}/QtGifImage/giflib
)
set(PROJECT_SOURCES ${SOURCE_DIR}/main.cc)

foreach(dir ${SRC_DIR})
    file(GLOB CPP_SOURCES "${dir}/*.cc")
    file(GLOB H_SOURCES "${dir}/*.h")
    file(GLOB C_SOURCES "${dir}/*.c")
    file(GLOB UI_SOURCES "${dir}/*.ui")
    list(APPEND PROJECT_SOURCES ${CPP_SOURCES} ${H_SOURCES} ${UI_SOURCES} ${C_SOURCES})
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT}
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(${PROJECT} PRIVATE
                                    ${EXTERNAL_DIR}
                                    ${SOURCE_DIR}
)

target_link_libraries(${PROJECT} PRIVATE
                                Qt${QT_VERSION_MAJOR}::Widgets
                                Qt${QT_VERSION_MAJOR}::Gui
                                Qt${QT_VERSION_MAJOR}::OpenGLWidgets
                                Qt${QT_VERSION_MAJOR}::Core
)

set_target_properties(${PROJECT} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION .)

add_custom_command(TARGET ${PROJECT}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/models ${CMAKE_CURRENT_SOURCE_DIR}/models/rename_files.sh
    COMMENT "Renaming obj files"
)

if(APPLE)
    add_custom_command(TARGET ${PROJECT}
        POST_BUILD
        COMMAND open $<TARGET_BUNDLE_DIR:${PROJECT}>)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT})
endif()
