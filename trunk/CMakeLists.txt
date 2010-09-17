project(plasma-xalertu)
 
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${QT_QTPHONON_INCLUDE_DIR}
   )
 
# source code
set(xalertu_SRCS xalertu.cpp)

kde4_add_ui_files(xalertu_SRCS form/xalertuConfigWebCam.ui form/xalertuConfigSound.ui form/xalertuConfigRemote.ui form/xalertuConfigMotionSensor.ui form/xalertuConfig.ui)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_xalertu ${xalertu_SRCS})

target_link_libraries(plasma_applet_xalertu phonon
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
 
add_subdirectory( icons )
add_subdirectory( sounds )
 
install(TARGETS plasma_applet_xalertu
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-xalertu.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
        
#install(FILES images/menuBarIconRed.svgz images/menuBarIconBlack.svgz
#        DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/)
