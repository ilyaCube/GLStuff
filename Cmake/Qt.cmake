# General QT settings
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# QT components, etc.
find_package(Qt5 COMPONENTS Quick Widgets Core Network REQUIRED)
list(APPEND LINKED_LIBRARIES Qt5::Quick Qt5::Widgets Qt5::Core Qt5::Network)
