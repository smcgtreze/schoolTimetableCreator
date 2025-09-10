cmake_minimum_required(VERSION 3.16)

project(SchoolTimetableCreator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES SchoolTimetableCreator_pt_PT.ts)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainWindow.cpp
    src/teacher_c.cpp
    src/rule_c.cpp
    src/master.cpp
    src/timetable_c.cpp
    src/mainWindow.ui
    ${TS_FILES}
)

set(PROJECT_HEADERS
    include/mainWindow.h
    include/teacher_c.h
    include/rule_c.h
    include/master.h
    include/timetable_c.h
)

include_directories(include)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(SchoolTimetableCreator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(SchoolTimetableCreator SHARED
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
        )
    else()
        add_executable(SchoolTimetableCreator
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(SchoolTimetableCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SchoolTimetableCreator PRIVATE Qt6::Core)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SchoolTimetableCreator)
endif()

set_target_properties(SchoolTimetableCreator PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SchoolTimetableCreator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SchoolTimetableCreator)
endif()
