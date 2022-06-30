#ifndef GST_COMMON_H
#define GST_COMMON_H

namespace gstpipe {

/**
 * @brief The input form of the gstreamer pipeline
 * 
 */
typedef enum _GstSourceType {
    CAMERA_SRC,     // The pipeline source is camera
    RTSP_SRC,           // The pipeline source is rtsp stream
    LOCAL_FILE_SRC           // The pipeline source is local file
} GstSourceType;

/**
 * @brief final output style of gstreamer pipeline 
 * 
 */
typedef enum _GstSinkType {
    WAYLAND_SINK,             // The pipeline sink is waylandsink for LCD displayer
    RTSP_SINK,        // The pipeline sink is rtsp stream for remote live play
    LOCAL_FILE_SINK,     // The pipeline sink is local file for store data to local file
    APPSINK     // The pipeline sink is gstreamer appsink elements for secondary processing of data
} GstSinkType;

}


#endif //GST_COMMON_H