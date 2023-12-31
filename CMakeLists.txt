cmake_minimum_required(VERSION 3.16)
project(kato LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin") # .exe and
# .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib") # .so and
                                                                   # .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib") # .lib and .a

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# TODO: Make this configurable
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.12/5.12.12/msvc2017_64")

find_package(
    Qt5 REQUIRED COMPONENTS Core Gui Multimedia Network Websockets Widgets
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "include/*.hpp")

qt5_add_resources(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.qrc)

if(ANDROID)
    add_library(kato SHARED ${SOURCES})
else()
    if(WIN32)
        list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/assets/app/app.rc)
    endif()
    add_executable(kato ${SOURCES})
endif()

include(cmake/Dependencies.cmake)

target_compile_features(kato PRIVATE cxx_std_17)
set_target_properties(kato PROPERTIES WIN32_EXECUTABLE TRUE MACOSX_BUNDLE TRUE)
target_include_directories(kato PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(
    kato PRIVATE Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::Network
                 Qt5::WebSockets Qt5::Widgets
)

if(WIN32)
    target_link_libraries(kato PRIVATE wsock32 ws2_32)
endif()

set(COMPONENT_NAME_MAIN kato)

include(GNUInstallDirs)
install(
    TARGETS kato
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${COMPONENT_NAME_MAIN}
)

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "IFW")
set(CPACK_IFW_ROOT "C:/Qt/QtIFW-4.5.1")

set(CPACK_IFW_PACKAGE_NAME "Kato")
set(CPACK_IFW_PACKAGE_TITLE "Kato Installer")
set(CPACK_IFW_PACKAGE_PUBLISHER "Xminent")
set(CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
set(CPACK_IFW_PACKAGE_WIZARD_SHOW_PAGE_LIST OFF)
set(CPACK_IFW_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/app/app.ico")
set(CPACK_IFW_PACKAGE_WINDOW_ICON
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/logo.png"
)
set(CPACK_IFW_PACKAGE_WATERMARK
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/installer/banner.png"
)
set(CPACK_IFW_PACKAGE_STYLE_SHEET
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/installer/styles.qss"
)
set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME "Kato Uninstaller")
set(CPACK_PACKAGE_FILE_NAME "Kato")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Kato")

find_program(WINDEPLOYQT windeployqt REQUIRED)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/deploy-qt-windows.cmake.in"
    "${CMAKE_BINARY_DIR}/deploy-qt-windows.cmake" @ONLY
)

set(CPACK_PRE_BUILD_SCRIPTS ${CMAKE_BINARY_DIR}/deploy-qt-windows.cmake)

include(CPack)
include(CPackIFW)

cpack_ifw_configure_component(
    ${COMPONENT_NAME_MAIN} ESSENTIAL FORCED_INSTALLATION
    LICENSES "License" ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
)
