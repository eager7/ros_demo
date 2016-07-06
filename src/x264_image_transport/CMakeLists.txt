cmake_minimum_required(VERSION 2.8.3)
project(x264_image_transport)

find_package(OpenCV REQUIRED)
find_package(catkin REQUIRED COMPONENTS cv_bridge dynamic_reconfigure image_transport message_generation rosbag pluginlib std_msgs)

add_message_files(DIRECTORY msg FILES x264Packet.msg)

generate_messages(DEPENDENCIES std_msgs)

find_package(PkgConfig)
#pkg_check_modules(PC_OGG REQUIRED ogg)
#pkg_check_modules(PC_THEORA REQUIRED theora)
#pkg_check_modules(PC_THEORAENC REQUIRED theoraenc)
#pkg_check_modules(PC_THEORADEC REQUIRED theoradec)


# generate the dynamic_reconfigure config file
generate_dynamic_reconfigure_options(cfg/x264Publisher.cfg cfg/x264Subscriber.cfg)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS message_runtime std_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS}
#  ${PC_OGG_INCLUDE_DIRS}
#  ${PC_THEORA_INCLUDE_DIRS}
#  ${PC_THEORAENC_INCLUDE_DIRS}
#  ${PC_THEORADEC_INCLUDE_DIRS}
)

#link_directories(${PC_OGG_LIBRARY_DIRS} 
#                 ${PC_THEORA_LIBRARY_DIRS}
#                 ${PC_THEORAENC_LIBRARY_DIRS}
#                 ${PC_THEORADEC_LIBRARY_DIRS})
#add_definitions(${PC_OGG_CFLAGS_OTHER} 
#                ${PC_THEORA_CFLAGS_OTHER}
#                ${PC_THEORAENC_CFLAGS_OTHER}
#                ${PC_THEORADEC_CFLAGS_OTHER}
#)

#add_library(${PROJECT_NAME} src/x264_publisher.cpp src/x264_subscriber.cpp src/manifest.cpp)
#add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_gencfg ${PROJECT_NAME}_gencpp)
#target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES}
#                                      ${OpenCV_LIBRARIES}
#                                      ${PC_OGG_LIBRARIES}
#                                      ${PC_THEORA_LIBRARIES}
#                                      ${PC_THEORAENC_LIBRARIES}
#                                      ${PC_THEORADEC_LIBRARIES}
#)


include_directories(
  include
    ${catkin_INCLUDE_DIRS}
      )

        set( plugin_src
            src/x264_publisher.cpp
                src/x264_subscriber.cpp
                    src/manifest.cpp
                      )

                      add_library(${PROJECT_NAME} ${plugin_src})
                      add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_gencfg ${PROJECT_NAME}_gencpp)

                      target_link_libraries(${PROJECT_NAME}  ${catkin_LIBRARIES} x264 avcodec avformat avutil swscale)
                      class_loader_hide_library_symbols(${PROJECT_NAME})


                      find_package(catkin REQUIRED COMPONENTS roscpp image_transport cv_bridge)

                      include_directories(include ${catkin_INCLUDE_DIRS})
                      add_executable(x264_test_publisher src/x264_test_publisher.cpp)
                      target_link_libraries(x264_test_publisher ${catkin_LIBRARIES} x264 avcodec avformat avutil swscale)

                      add_executable(x264_test_subscriber src/x264_test_subscriber.cpp)
                      target_link_libraries(x264_test_subscriber ${catkin_LIBRARIES} x264 avcodec avformat avutil swscale) 

add_executable(vsubscriber src/my_subscriber.cpp)
target_link_libraries(vsubscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(jpegsubscriber src/jpeg_sub.cpp)
target_link_libraries(jpegsubscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} pthread)

# add the plugin examples
#add_library(resized_publisher src/manifest.cpp src/resized_publisher.cpp src/resized_subscriber.cpp)
#target_link_libraries(resized_publisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

# add the publisher example
add_executable(vpublisher src/video_publisher.cpp)
target_link_libraries(vpublisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
#@add_executable(zmq_subscriber src/zmq_subscriber.cpp)
#target_link_libraries(zmq_subscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES}
#  zmq)


#add_executable(ogg_saver src/ogg_saver.cpp)
#target_link_libraries(ogg_saver ${PC_THEORA_LIBRARY} 
#                                ${PC_OGG_LIBRARY} 
#                                ${OpenCV_LIBRARIES} 
#                                ${catkin_LIBRARIES}  
#                                ${PC_THEORAENC_LIBRARIES}
#                                ${PC_THEORADEC_LIBRARIES})
#add_dependencies(ogg_saver ${PROJECT_NAME}_gencpp)

#install(TARGETS ${PROJECT_NAME} ogg_saver
#  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#)
#install(DIRECTORY include/${PROJECT_NAME}/
#  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#)

# add xml file
install(FILES x264_plugins.xml
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
