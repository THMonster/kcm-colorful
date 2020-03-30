cmake_minimum_required(VERSION 3.0)
SET(CMAKE_INSTALL_PREFIX /usr CACHE PATH "Set install prefix.")
project(kcm-colorful)
set(PROJECT_VERSION "1.0.5")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


find_package(ECM 1.0.0 REQUIRED NO_MODULE)
find_package( Gettext REQUIRED )
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(KDEInstallDirs)
include(ECMInstallIcons)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets X11Extras Quick QuickControls2 QuickWidgets DBus)
find_package(KF5 REQUIRED COMPONENTS
    CoreAddons
    WidgetsAddons
    ConfigWidgets
    KIO
    KCMUtils
    I18n
    Declarative
)
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_colorful\")

add_subdirectory(src)
add_subdirectory(po)

file(GLOB schemefiles schemes/*.colors)
install( FILES ${schemefiles} DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes )
install(DIRECTORY plasmathemes/kde-theme-backup/Colorful DESTINATION ${KDE_INSTALL_DATADIR}/plasma/desktoptheme)
