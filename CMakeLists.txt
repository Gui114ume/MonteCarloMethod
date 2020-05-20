cmake_minimum_required(VERSION 3.5)

project(IntegrationMC LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( SOURCES
    main.cpp
    qcustomplot.cpp
    mainwindow.cpp
    )

set(UIS
    mainwindow.ui)

set( MOC_HEADERS
    mainwindow.h
    GeneralEDOMonteCarloIntegrator.hpp
    IMonteCarloIntegrator.hpp
    qcustomplot.hpp
    YIndependantedoMonteCarloIntegrator.hpp
    MonteCarloIntegrator.hpp
    couplededosysmontecarlointegrator.hpp)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)

QT5_WRAP_UI(UI_HEADERS ${UIS})

if(ANDROID)
  add_library(IntegrationMC SHARED
    main.cpp
  )
else()
  add_executable(IntegrationMC
      ${SOURCES}
      ${UI_HEADERS}
  )
endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(IntegrationMC PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(IntegrationMC PRIVATE Qt5::Widgets)
target_link_libraries(IntegrationMC PRIVATE Qt5::PrintSupport)
