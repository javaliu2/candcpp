cmake_minimum_required(VERSION 3.10)

project(ffmpeg_study)

include_directories(/usr/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(/usr/lib)

add_executable(ref_test ${CMAKE_SOURCE_DIR}/src/ref.cpp)

add_executable(demux_decode_demo ${CMAKE_SOURCE_DIR}/src/demux_decode.c)
target_link_libraries(demux_decode_demo avformat avutil avcodec)

add_executable(linesize_test ${CMAKE_SOURCE_DIR}/src/linesize_test.c)
target_link_libraries(linesize_test avutil)


add_executable(read_one_video_frame ${CMAKE_SOURCE_DIR}/src/read_one_video_frame.c)
target_link_libraries(read_one_video_frame avformat avutil avcodec swscale)

add_executable(read_one_audio_frame ${CMAKE_SOURCE_DIR}/src/read_one_audio_frame.c)
target_link_libraries(read_one_audio_frame avformat avutil avcodec swscale)
