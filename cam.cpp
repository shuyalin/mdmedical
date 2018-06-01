#include "cam.h"

typedef unsigned char uchar;//#define uchar unsigned char //






Opera_Cam::Opera_Cam()
{
	Init_Prop();
}

Opera_Cam::Opera_Cam(int which_cam)
{
	Init_Prop();
	Opened = Open(which_cam);
}

Opera_Cam::Opera_Cam(const string& cam_name)
{
	Init_Prop();
	Opened = Open(cam_name);
}

bool Opera_Cam::Open(int which_cam)
{
	char cam_name[80];
	sprintf(cam_name, "/dev/video%1d", which_cam);
	string str = cam_name;
	return Open(str);
}
#if 1
bool Opera_Cam::Open(const string& cam_name)
{
	if(IsOpened())
		Free();
	
	string str = cam_name;
	if(0!=access(str.c_str(),F_OK))
	{
        return false;
        /*
		if(0!=access("/dev/video0",F_OK))
			return false;
		else
			str = "/dev/video0";//sprintf(deviceName, "/dev/video0");  try to open default cam
        */
    }
	cam_fd = open(str.c_str(),O_RDWR | O_NONBLOCK,0);

	if(cam_fd<0)
		return false;

	if(!Init_V4l2())
		return false;

	data_len = weight*height*2;
	Opened = true;
    return Opened;
}
#endif
void Opera_Cam::Init_Prop()
{
    cam_fd = -1;
	Opened = false;
	weight = WEIGHT_DEFAULT;
	height = HEIGHT_DEFAULT;
	data_len = -1;
	for(int i=0;i<BUFFER_NUMS;i++)
		MmapOK[i] = false;
}

bool Opera_Cam::Init_V4l2()
{

    CLEAR(mybuffer);

    CLEAR(mycap);
    if(ioctl(cam_fd,VIDIOC_QUERYCAP,&mycap)<0)
    {
		return false;
    }
	
    CLEAR(myfmt);
    myfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    myfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;//V4L2_PIX_FMT_MJPEG;//V4L2_PIX_FMT_YUYV;//
    myfmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    myfmt.fmt.pix.width = weight;
    myfmt.fmt.pix.height = height;
    if(ioctl(cam_fd,VIDIOC_S_FMT,&myfmt)<0)
    {
		printf("VIDIOC_S_FMT is failed!\n");
		return false;
    }
    myfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(cam_fd,VIDIOC_G_FMT,&myfmt);
    if(myfmt.fmt.pix.pixelformat&V4L2_PIX_FMT_YUYV)//V4L2_PIX_FMT_YUYV)

    CLEAR(myreq);
    myreq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    myreq.memory = V4L2_MEMORY_MMAP;
    myreq.count = BUFFER_NUMS;
    if(ioctl(cam_fd,VIDIOC_REQBUFS,&myreq)<0)
    {
        return false;
    }

    for(int i = 0; i< (int)myreq.count;i++)
    {
        CLEAR(mybuffer);

        mybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        mybuffer.memory = V4L2_MEMORY_MMAP;
        mybuffer.index = i;
        if(ioctl(cam_fd,VIDIOC_QUERYBUF,&mybuffer)<0)
        {
            return false;
        }

        vsimg[i] = mmap(0,mybuffer.length,PROT_READ|PROT_WRITE,MAP_SHARED,cam_fd,mybuffer.m.offset);
        if(vsimg[i]  == MAP_FAILED)
        {
            return false;
        }
		MmapOK[i] = true;
    }

    for(int i = 0;i< (int)myreq.count;i++)
    {
        CLEAR(mybuffer);
        mybuffer.index = i;
        mybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        mybuffer.memory = V4L2_MEMORY_MMAP;
        if(ioctl(cam_fd,VIDIOC_QBUF,&mybuffer)<0)
        {
      		for(int i=0;i<BUFFER_NUMS;i++)
			{
				if(MmapOK[i])
					munmap(vsimg[i] ,mybuffer.length);
			}
			
            return false;
        }
    }

    mybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(ioctl(cam_fd,VIDIOC_STREAMON,&(mybuffer.type))<0)
    {
        if(ioctl(cam_fd,VIDIOC_STREAMOFF,&(mybuffer.type))<0)
			;
		
		for(int i=0;i<BUFFER_NUMS;i++)
		{
			if(MmapOK[i])
				munmap(vsimg[i] ,mybuffer.length);
		}
        close(cam_fd);
        return false;
    }
   

/*
    rgbbuf[cam_id] = (unsigned char *)malloc(WEIGHT * HEIGHT * 3);
    if(NULL == rgbbuf[cam_id])
    {
        DIGISERVER_TRACE(ERROR_INFO,"%s","MALLOC failed");
        return false;
    }
    */

    mybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mybuffer.memory = V4L2_MEMORY_MMAP;
	
	return true;
}

bool Opera_Cam::Read()//(uchar **data,int& used_len)
{
	GetFrameLoop();

	if(!getframeok)
		return false;


	df.real_len = mybuffer.bytesused;  //used_len = mybuffer.bytesused;//mybuffer.length;	
	//data_len = mybuffer.length;
	df.data = (uchar *)vsimg[mybuffer.index];//*data = (uchar *)vsimg[mybuffer.index];

	return true;
}


bool Opera_Cam::Set(int propId,int value)
{
	//if(!IsOpened()&&(propId!=PROP_WEIGHT&&propId!=PROP_HEIGHT&&propId!=PROP_FPS))
	//	return -1;

	//if(IsOpened()&&(propId==PROP_WEIGHT||propId==PROP_HEIGHT||propId==PROP_FPS))
	//	return -1;
	static int w = 0, h = 0;
	switch(propId)
	{
		case PROP_WEIGHT:
			w = value;
			if(w&&h)
			{
				weight = w;
				height = h;
				w = h = 0;
			}
			break;
			
		case PROP_HEIGHT:
			h = value;
			if(w&&h)
			{
				weight = w;
				height = h;
				w = h = 0;
			}
			break;
			
		default:
			break;
	}
	return 0;
}

int Opera_Cam::Get(int propId)
{
	if(!IsOpened())
		return -1;

	int value = -1;
	switch(propId)
	{
		case PROP_WEIGHT:
			value = weight;
			break;
		case PROP_HEIGHT:
			value = height;
			break;
		default:
			break;
	}

	return value;
}
bool Opera_Cam::Operation(bool on_off)
{
	if(!IsOpened())
		return false;
	
    if(on_off)
    {
        if(ioctl(cam_fd,VIDIOC_STREAMON,&(mybuffer.type))<0)
        {
            return false;
        }
    }
    else
    {
        if(ioctl(cam_fd,VIDIOC_STREAMOFF,&(mybuffer.type))<0)
        {
            return false;
        }
    }
    return true;
}

void Opera_Cam::GetFrameLoop()
{
	unsigned int count;
    count = 1;

    while (count-- > 0) 
	{
        while (1) 
		{
            fd_set fds;
            struct timeval tv;
            int r;
            FD_ZERO (&fds);
            FD_SET (cam_fd, &fds);

            tv.tv_sec = 1;
            tv.tv_usec = 0;
			
            r = select (cam_fd+1, &fds, NULL, NULL, &tv);
            if (-1 == r) 
			{
                if (EINTR == errno)
                    continue;
                printf("select");
            }

            if (0 == r) 
			{
                printf("select timeout\n");
                break;
            }

            if (GetFrameV4l2())
                break;
        }
    }
}

int Opera_Cam::GetFrameV4l2()
{
    CLEAR (mybuffer);
    mybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mybuffer.memory = V4L2_MEMORY_MMAP;
	getframeok = false;
    if (-1==ioctl (cam_fd, VIDIOC_DQBUF, &mybuffer)) 
	{
        switch (errno) 
		{
	        case EAGAIN:
	            return 0;

	        case EIO:
		        if (!(mybuffer.flags & (V4L2_BUF_FLAG_QUEUED | V4L2_BUF_FLAG_DONE)))
		        {
					if (ioctl(cam_fd, VIDIOC_QBUF, &mybuffer) == -1)
					{
						return 0;
					}
		        }
		        return 0;

	        default:
	            /* display the error and stop processing */
	            printf ("VIDIOC_DQBUF FAILED\n");
	            return 1;
        }
   }

   //printf("got data in buff %d, len=%d, flags=0x%X, seq=%d, used=%d)\n",
   //	  buf.index, buf.length, buf.flags, buf.sequence, buf.bytesused);

   if (-1==ioctl (cam_fd, VIDIOC_QBUF, &mybuffer))
       printf("VIDIOC_QBUF FAILED\n");

   getframeok = true; 

   timestamp = mybuffer.timestamp;
   return 2;
}

void Opera_Cam::Free()
{

    Operation(STREAMOFF);

	for(int i=0;i<BUFFER_NUMS;i++)
	{
		if(MmapOK[i])
			munmap(vsimg[i] ,mybuffer.length);
	}

    Opened = false;
    if(cam_fd>0)
        close(cam_fd);
}

Opera_Cam::~Opera_Cam()
{
	Free();
}




