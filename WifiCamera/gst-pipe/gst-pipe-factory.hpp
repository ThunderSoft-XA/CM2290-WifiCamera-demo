/**
 * @file gst-pipe-header.h
 * @author jhonkael(you@domain.com)
 * @brief  Include this header file to invoke any interface of this function
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef GST_PIPE_FACTORY_HPP
#define GST_PIPE_FACTORY_HPP

#include <iostream>

#include <gst/gst.h>

#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-session-pool.h>

#include "gst-common.h"

#include "gst-pipe.hpp"
#include "gst-rtsp-server.hpp"
#include "gst-camera.hpp"

namespace gstpipe {

class GstPipeFactory
{
public:
    ~GstPipeFactory() { };

    /**
     * @brief Get the Instance object
     *  The singleton pattern
     * @return GstPipeFactory* 
     */
    static GstPipeFactory* getInstance(){
        static GstPipeFactory* factory = new GstPipeFactory();
        return factory;
    }

    RTSPServer* createRTSPServer() {
        return new RTSPServer();
    }

    /**
     * @brief Create a Pipe Line object
     * 
     * @param _source_type 
     * @param _sink_type 
     * @return GstPipe* 
     */
    GstPipe* createPipeline(GstSourceType _source_type, GstSinkType _sink_type) {
        GstPipe* pipe_line;
        switch (_source_type) {
        case GstSourceType::LOCAL_FILE_SRC  :
            break;
        case GstSourceType::CAMERA_SRC:
            pipe_line = new GstCamera(_sink_type);
            break;
        case GstSourceType::RTSP_SRC:
            break;
        }

        return pipe_line;
    }

    /**
     * @brief g_main_loop_run
     * 
     */
    void loopRunGstPipeFactory() {
        // unrefrence main loop , destroy the whole gstreamer run enviment.
        g_main_loop_run (this->loop);
    }

    RTSPServer* getRTSPServer() {

        return rtsp_server;
    }

private:
    /* data */
    GMainLoop* loop;
    RTSPServer* rtsp_server = NULL;

    GstPipeFactory(/* args */) {
        gst_init(NULL,NULL);
        this->loop = g_main_loop_new (NULL, FALSE);
    };

};

}

#endif //GST_PIPE_FACTORY_HPP