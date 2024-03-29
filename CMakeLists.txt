cmake_minimum_required(VERSION 2.8)
project( OffseasonVision2019 )

set( BOOST_ROOT ~/boost_1_58_0 )

set( OpenCV_DIR ~/opencv-3.4.2 )
add_definitions( -std=c++17 )

set( OpenCV_FOUND 1 )

find_package( PkgConfig )
find_package( OpenCV 3.4.2 REQUIRED )
find_package( Boost COMPONENTS system REQUIRED )
pkg_check_modules( GST REQUIRED gstreamer-1.0>=1.4
                               gstreamer-sdp-1.0>=1.4
                               gstreamer-video-1.0>=1.4
                               gstreamer-app-1.0>=1.4 )

INCLUDE_DIRECTORIES( ~/yaml-cpp-master/include ${Boost_INCLUDE_DIR} ${GST_INCLUDE_DIRS} /usr/include/gstreamer-1.0 include )

file (GLOB OffseasonVision2019_SRC
    "src/*.cpp"
)

add_executable( OffseasonVision2019 ${OffseasonVision2019_SRC} )
target_link_libraries( OffseasonVision2019 /home/pi/yaml-cpp-master/build/libyaml-cpp.a ${OpenCV_LIBS} ${Boost_LIBRARIES} ${GST_LIBRARIES} gstapp-1.0 gstriff-1.0 gstbase-1.0 gstvideo-1.0 gstpbutils-1.0 X11 pthread )

