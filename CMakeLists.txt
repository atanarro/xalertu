project(plasma-xalertu)

cmake_minimum_required(VERSION 2.8)
 
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
#####################################################################
# DBUS
#####################################################################
set(dbus_SRCS
# dbus/RootDBusHandler.cpp
# dbus/PlayerDBusHandler.cpp
# dbus/TracklistDBusHandler.cpp
# dbus/CollectionDBusHandler.cpp
# dbus/DBusQueryHelper.cpp
)

set(xalertu_SRCS ${dbus_SRCS} usbpad.cpp mousesensor.cpp keyboardsensor.cpp xalertu.cpp)

kde4_add_ui_files(xalertu_SRCS form/xalertuConfigWebCam.ui 
                  form/xalertuConfigSound.ui 
                  form/xalertuConfigRemote.ui
                  form/xalertuConfigMotionSensor.ui
                  form/xalertuConfigEmail.ui
                  form/xalertuConfigScreenshot.ui
                  form/xalertuConfig.ui)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_xalertu ${xalertu_SRCS})

target_link_libraries(plasma_applet_xalertu phonon kdeui #solidcontrol
                      #${KDE4_SOLID_LIBS} 
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS})
 
add_subdirectory( icons )
add_subdirectory( sounds )
# add_subdirectory( dbus )


# qt4_add_dbus_adaptor( amaroklib_LIB_SRCS dbus/org.freedesktop.MediaPlayer.root.xml RootDBusHandler.h Amarok::RootDBusHandler RootAdaptor RootAdaptor)
# qt4_add_dbus_adaptor( amaroklib_LIB_SRCS dbus/org.freedesktop.MediaPlayer.player.xml PlayerDBusHandler.h Amarok::PlayerDBusHandler PlayerAdaptor PlayerAdaptor)
# qt4_add_dbus_adaptor( amaroklib_LIB_SRCS dbus/org.freedesktop.MediaPlayer.tracklist.xml TracklistDBusHandler.h Amarok::TracklistDBusHandler TracklistAdaptor TracklistAdaptor)
# qt4_add_dbus_adaptor( amaroklib_LIB_SRCS dbus/org.kde.amarok.Collection.xml CollectionDBusHandler.h CollectionDBusHandler CollectionAdaptor CollectionAdaptor)

 
install(TARGETS plasma_applet_xalertu
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-xalertu.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
        
# install(FILES dbus/org.freedesktop.MediaPlayer.root.xml
#               dbus/org.freedesktop.MediaPlayer.player.xml
#               dbus/org.freedesktop.MediaPlayer.tracklist.xml
#               dbus/org.kde.amarok.Collection.xml
#         DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
