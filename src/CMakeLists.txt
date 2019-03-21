add_subdirectory(helper)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

#add_definitions(-DTRANSLATION_DOMAIN=\"kcm5_colorful\")

set(kcm_file_SRCS
  kcm_colorful.cpp
)

qt5_add_resources(kcm_file_SRCS res.qrc)
add_library(kcm_colorful MODULE ${kcm_file_SRCS})
target_compile_definitions(kcm_colorful PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(kcm_colorful
  KF5::KIOWidgets
  KF5::CoreAddons
  KF5::ConfigWidgets
  KF5::I18n
  KF5::Solid
  Qt5::Core
  Qt5::Gui
  Qt5::QuickControls2
  Qt5::Widgets
  Qt5::QuickWidgets
)



install(FILES kcm_colorful.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS kcm_colorful DESTINATION ${KDE_INSTALL_PLUGINDIR})
