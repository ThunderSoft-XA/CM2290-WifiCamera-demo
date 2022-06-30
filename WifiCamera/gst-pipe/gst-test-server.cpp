#include "gst-pipe-factory.hpp"

int main(int argc, char** argv)
{
    gstpipe::GstPipeFactory* pipe_factory = gstpipe::GstPipeFactory::getInstance();

    gstpipe::RTSPServer* gst_rtsp_server;

    gst_rtsp_server = pipe_factory->createRTSPServer();
    gst_rtsp_server->startServer();

    pipe_factory->loopRunGstPipeFactory();

    return 0;
}
