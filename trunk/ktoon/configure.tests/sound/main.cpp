
#include <gst/gst.h>

int main()
{
	GError *err;
	
	if ( !gst_init_check( NULL, NULL, &err ) )
	{
		return -1;
	}
	GstElement* dummy = gst_element_factory_make ( "fakesink", "fakesink" );

	if(!dummy)
	{
		return -1;
	}
	
	gst_object_unref( dummy );

#if GST_VERSION_MINOR < 10 && GST_VERSION_MAJOR == 0 
#error Please install gstreamer-0.10
#endif
	
	return 0;
}


