cmake_minimum_required(VERSION 3.5)

project(mydemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(OpenCASCADE_DIR "E:/gitspace/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/cmake")
find_package (OpenCASCADE REQUIRED PATH "E:/gitspace/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/cmake" NO_DEFAULT_PATH)
include_directories(${OpenCASCADE_INCLUDE_DIR})

find_package(OpenGL REQUIRED)




set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mydemo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        OcctGlTools.h OcctGlTools.cpp
        OcctQtViewer.h OcctQtViewer.cpp
        Appfunc.h Appfunc.cpp
        MakeBottle.h MakeBottle.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET mydemo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(mydemo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(mydemo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(mydemo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

target_link_libraries(mydemo PRIVATE
  OpenGL::GL
)

foreach(LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(mydemo PRIVATE
    debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib
    optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib
  )
  #message("lib = ${OpenCASCADE_LIBRARY_DIR}/${LIB}")
endforeach()

#running environment
set_property(TARGET mydemo PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.mydemo)
endif()
set_target_properties(mydemo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mydemo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mydemo)
endif()
