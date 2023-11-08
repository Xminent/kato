include(cmake/CPM.cmake)

cpmaddpackage(
    NAME qtniceframelesswindow GITHUB_REPOSITORY
    "Bringer-of-Light/Qt-Nice-Frameless-Window" GIT_TAG "master"
)

add_library(qtniceframelesswindow STATIC)
target_include_directories(
    qtniceframelesswindow
    PUBLIC ${qtniceframelesswindow_SOURCE_DIR}/framelesswindow
)
target_link_libraries(
    qtniceframelesswindow PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets
)
target_sources(
    qtniceframelesswindow
    PRIVATE
        ${qtniceframelesswindow_SOURCE_DIR}/framelesswindow/framelesswindow.cpp
        ${qtniceframelesswindow_SOURCE_DIR}/framelesswindow/framelesswindow.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE qtniceframelesswindow)
