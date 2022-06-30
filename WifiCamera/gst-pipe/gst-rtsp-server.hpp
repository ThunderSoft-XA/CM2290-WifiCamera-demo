#ifndef GST_RTSP_SERVER_HPP
#define GST_RTSP_SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <gst/gst.h>

#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-session-pool.h>

#include "gst-pipe.hpp"

namespace gstpipe {

class RTSPServer {

private :
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    std::vector<GstRTSPMediaFactory*> rtsp_factory_vec;

    static gboolean timeout (GstRTSPServer * server) {

        GstRTSPSessionPool *pool;

        pool = gst_rtsp_server_get_session_pool (server);
        gst_rtsp_session_pool_cleanup (pool);
        g_object_unref (pool);

        return TRUE;
    }

public :
    RTSPServer() {
        /* create server instance */
        server = gst_rtsp_server_new ();

        gst_rtsp_server_set_address(server,"192.168.0.1");
        gst_rtsp_server_set_service(server,"8900");

        g_print("rtsp server addr:port = %s:%s\n",gst_rtsp_server_get_address(server),gst_rtsp_server_get_service(server));

        /* get rtsp stream manager of server*/
        mounts = gst_rtsp_server_get_mount_points (server);

        /* create rtsp stream manager and set source of the stream*/
        this->rtsp_factory_vec.emplace_back(gst_rtsp_media_factory_new());
    }

    bool startServer() {
        /* create rtsp stream manager and set source of the stream*/
        this->rtsp_factory_vec.emplace_back(gst_rtsp_media_factory_new());
        gst_rtsp_media_factory_set_launch (this->rtsp_factory_vec.back(),
            "( udpsrc name=pay0 address=192.168.0.1 port=8554 \
            caps=\"application/x-rtp,media=video,\
            clock-rate=90000,encoding-name=H264,payload=96\" )");

        gst_rtsp_media_factory_set_shared (this->rtsp_factory_vec.back(), TRUE);

        /*bind address of stream and add to stream manager*/
        gst_rtsp_mount_points_add_factory (mounts, "/live", this->rtsp_factory_vec.back());

        g_object_unref (mounts);

        gst_rtsp_server_attach (server, NULL); 
            
        g_timeout_add_seconds (2, (GSourceFunc) timeout, server);

        g_print ("stream ready at rtsp://192.168.0.1:8900/live\n");
        return true;
    }

    bool addRTSPStream(std::string _rtsp_pipeline) {
        /* create rtsp stream manager and set source of the stream*/
        this->rtsp_factory_vec.emplace_back(gst_rtsp_media_factory_new());

        std::ostringstream rtsp_pipeline;
        rtsp_pipeline << "( " << _rtsp_pipeline << " )";

        gst_rtsp_media_factory_set_launch (this->rtsp_factory_vec.back(), rtsp_pipeline.str().c_str());

        gst_rtsp_media_factory_set_shared (this->rtsp_factory_vec.back(), TRUE);

        /*绑定流的地址，并加入流管理器中*/
        std::ostringstream stream_shuffix;
        stream_shuffix << "/live-" << this->rtsp_factory_vec.size();
        gst_rtsp_mount_points_add_factory (mounts, stream_shuffix.str().c_str(), this->rtsp_factory_vec.back());

        g_print ("addRTSPStream finished\n");
    }

};


}

#endif //GST_RTSP_SERVER_HPP
