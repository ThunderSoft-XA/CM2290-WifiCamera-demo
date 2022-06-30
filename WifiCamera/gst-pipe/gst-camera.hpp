#include <iostream>
#include <sstream>

#include "gst-pipe.hpp"

namespace gstpipe {

class GstCamera: public GstPipe
{
public:
    GstCamera(GstSinkType _sink_type){
		this->createSourcePipeline();
		this->createSinkPipeline(_sink_type);
		std::cout << "GST Pipeline: " << this->pipeline_str << std::endl;
    }
    ~GstCamera();

  void createSourcePipeline();
  void createSinkPipeline(GstSinkType _sink_type);
};

void GstCamera::createSourcePipeline() 
{
	std::ostringstream cameraPath;
	cameraPath << "qtiqmmfsrc camera=0 ! video/x-raw,format=NV12,width=1920,height=1080,framerate=30/1 ! qtic2venc ! h264parse config-interval=-1 ! ";

	this->pipeline_str = cameraPath.str();

}

void GstCamera::createSinkPipeline(GstSinkType _sink_type)
{
	std::ostringstream cameraPath;
	if(GstSinkType::RTSP_SINK == _sink_type) {
		cameraPath << "rtph264pay ! udpsink host=192.168.0.1 port=8554";
	} else if (GstSinkType::LOCAL_FILE_SINK == _sink_type) {
		cameraPath << "mp4mux ! queue ! filesink location=/data/mux.mp4";
	}

	this->pipeline_str += cameraPath.str();
}


} // namespace gstpipe
