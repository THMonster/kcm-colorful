cmake_minimum_required(VERSION 3.0)

project(kcmcolorfulhelper)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5Core)
find_package(Qt5DBus)
find_package(KF5 REQUIRED COMPONENTS
    CoreAddons
    WidgetsAddons
    ConfigWidgets
    KIO
    KCMUtils
    I18n
)

add_subdirectory(src)

file(GLOB schemefiles schemes/*.colors)
install( FILES ${schemefiles} DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes )
install(DIRECTORY plasmathemes/kde-theme-backup/Colorful DESTINATION ${KDE_INSTALL_DATADIR}/plasma/desktoptheme)
