#ifndef GST_PIPE_HPP
#define GST_PIPE_HPP

#include <iostream>
#include <memory>
#include <chrono>

#include <gst/gst.h>
#include <gst/app/app.h>

#include "gst-common.h"
#include "gst-rtsp-server.hpp"

namespace gstpipe{

class GstPipe {

private :
    GstElement *pipeline;
    GstBus *gstbus;
    GError *error = NULL;

    static void onEOS(GstAppSink *appsink, void *user_data);
    static GstFlowReturn onPreroll(GstAppSink *appsink, void *user_data);
    static GstFlowReturn onBuffer(GstAppSink *appsink, void *user_data);
    static gboolean pipeBusCallback (GstBus *bus, GstMessage *message, gpointer data);

public :
    GstPipe() {
        g_print("constructor GstPipe for rnning gstreamer pipeline\n");

        gst_init(NULL,NULL);
    }

    ~GstPipe() {
        // clean up
        gst_object_unref(this->gstbus);

        // unreference pipeline
        gst_element_set_state(this->pipeline, GST_STATE_NULL);
        gst_object_unref(this->pipeline);
    }

    std::string pipeline_str;
    /**
     * @brief Create a Source Pipeline object
     *  Must end with the tee element
     */
    virtual void createSourcePipeline() = 0;

    /**
     * @brief Create a Sink Pipeline object
     *  Must start with the tee element
     */
    virtual void createSinkPipeline(GstSinkType _sink_type) = 0;

    /**
     * @brief  Used to control the running state of the entire pipeline
     * 
     */
    void changePipelineState(GstState _state);

    bool launchPipeline(std::string _pipeline_str);

};

bool GstPipe::launchPipeline(std::string _pipeline_str){

    this->pipeline = gst_parse_launch(_pipeline_str.c_str(),&this->error);
    if (this->error != NULL) {
        g_print("could not construct pipeline: %s\n", error->message);
        g_clear_error (&error);
        goto exit;
    }
        
    /* Run the pipeline for Start playing */
    if (GST_STATE_CHANGE_FAILURE == gst_element_set_state (this->pipeline, GST_STATE_PLAYING)) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
    }

    //get pipeline bus
    this->gstbus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

    // add a message handler: set it to pipeline bus
    gst_bus_add_watch(this->gstbus, pipeBusCallback, NULL);

    return true;

exit:
    if(this->pipeline != NULL) {
        gst_element_set_state (this->pipeline, GST_STATE_NULL);
        gst_object_unref (this->pipeline);
        this->pipeline = NULL;
    }
    return false;
}

// onEOS
void GstPipe::onEOS(GstAppSink *appsink, void *user_data)
{
    // GstPipe *dec = reinterpret_cast<GstPipe *>(user_data);
    g_print("gstreamer decoder onEOS\n");
}

// onPreroll
GstFlowReturn GstPipe::onPreroll(GstAppSink *appsink, void *user_data)
{
    g_print("gstreamer decoder onPreroll\n");
    return GST_FLOW_OK;
}

void GstPipe::changePipelineState(GstState _state)
{
    GstStateChangeReturn ret =  gst_element_set_state(this->pipeline, _state);
}

// onBuffer
GstFlowReturn GstPipe::onBuffer(GstAppSink *appsink, void *user_data)
{
    g_print("enter gst on buffer callback\n");
    GstPipe *dec = NULL;
    GstSample *sample = NULL;
    dec = reinterpret_cast<GstPipe *>(user_data);
    if(dec == NULL || appsink == NULL) {
        g_print ("decode or appsink is null\n");
        return GST_FLOW_ERROR;
    }

    // GstSample* sample = gst_app_sink_pull_sample(appsink);
    sample = gst_base_sink_get_last_sample(GST_BASE_SINK(appsink));


    g_print("exit gst on buffer callback\n");
    return GST_FLOW_OK;
}

gboolean GstPipe::pipeBusCallback (GstBus *bus, GstMessage *message, gpointer data) 
{
    g_print("Handle gstreamer pipeline bus message\n");
    GstPipe *gst_camera = reinterpret_cast<GstPipe *>(data);
    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
        GError *err;
        gchar *debug;

        gst_message_parse_error (message, &err, &debug);
        g_print ("Error: %s\n", err->message);
        g_error_free (err);
        g_free (debug);

        gst_element_set_state(gst_camera->pipeline,GST_STATE_READY);
        break;
    }
    case GST_MESSAGE_EOS:
        /* end-of-stream */
        gst_element_set_state(gst_camera->pipeline,GST_STATE_NULL);
        break;
    default:
    /* unhandled message */
        break;
    }
    /* we want to be notified again the next time there is a message
    * on the bus, so returning TRUE (FALSE means we want to stop watching
    * for messages on the bus and our callback should not be called again)
    */
    return TRUE;
}

} // namespace gstpipe

#endif // GST_PIPE_HPP