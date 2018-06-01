#ifndef __CAM_H_
#define __CAM_H_
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <QTimer>


using namespace std;

#define WEIGHT_DEFAULT 		640
#define HEIGHT_DEFAULT 		480

#define PROP_WEIGHT			0
#define PROP_HEIGHT			1
#define PROP_FPS			2

#define PROP_BRIGHTNESS		10
#define PROP_CONTRAST		11
#define PROP_HUE			12
#define PROP_GAIN			13
#define PROP_EXPOSURE		14

#define BUFFER_NUMS 2


#define STREAMON  true
#define STREAMOFF false

#define CLEAR(x) memset (&(x), 0, sizeof (x))




typedef unsigned char uchar;//#define uchar unsigned char //

class Opera_Cam
{
	public:
		Opera_Cam();
		Opera_Cam(int which_cam);
		Opera_Cam(const string& cam_name);
		bool Open(int which_cam=0);
		bool Open(const string& cam_name);
		bool IsOpened(){return Opened;}
		bool Read();//(uchar **data, int& used_len);
		bool Set(int propId, int value);
		int Get(int propId);
		int GetDataLen(){return data_len;};
		//Opera_Cam& operator>>(uchar **dst);
		virtual ~Opera_Cam();

		struct DataInfo
		{
			uchar *data;
			int real_len;
		};

		DataInfo df;
        int weight,height;

        void Free();

	private:
		void Init_Prop();
	    bool Init_V4l2();
		bool Operation(bool on_off);
		void GetFrameLoop();
		int GetFrameV4l2();

	
 		//unsigned char* rgbbuf[CAM_MAX];
		void *vsimg[BUFFER_NUMS] ;
		struct v4l2_buffer mybuffer;
		struct v4l2_capability mycap;
	    struct v4l2_format myfmt;
	    struct v4l2_requestbuffers myreq;
	    struct v4l2_streamparm mysetfps;
		struct timeval timestamp;
		
		int cam_fd,data_len;
		bool MmapOK[BUFFER_NUMS];
		bool Opened,getframeok;
};



#endif
