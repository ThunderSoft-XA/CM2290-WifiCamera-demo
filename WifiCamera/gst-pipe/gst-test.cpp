#include "gst-pipe-factory.hpp"

int main(int argc, char** argv)
{
    gstpipe::GstSourceType gst_source_type = gstpipe::GstSourceType::CAMERA_SRC;
    gstpipe::GstSinkType gst_sink_type = gstpipe::GstSinkType::RTSP_SINK;

    gstpipe::GstPipeFactory* pipe_factory = gstpipe::GstPipeFactory::getInstance();

    gstpipe::GstPipe* gst_pipe;

    gst_pipe = pipe_factory->createPipeline(gst_source_type,gst_sink_type);

    gst_pipe->launchPipeline(gst_pipe->pipeline_str);

    pipe_factory->loopRunGstPipeFactory();

    return 0;
}
