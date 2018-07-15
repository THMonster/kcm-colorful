add_executable(${PROJECT_NAME} "main.cpp"
                               "kcmcolorfulhelper.cpp"
                               "mmcq.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core
                                      Qt5::DBus
                                      KF5::ConfigCore
                                      KF5::ConfigGui
                                      KF5::ConfigWidgets
                                      KF5::KCMUtils
                                      KF5::I18n
                                      KF5::KIOCore
                                      KF5::CoreAddons)
                                      
install(TARGETS kcmcolorfulhelper  DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
