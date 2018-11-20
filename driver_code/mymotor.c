#include <linux/module.h>               /* For module specific items */  
#include <linux/moduleparam.h>          /* For new moduleparam's */  
#include <linux/types.h>                /* For standard types (like size_t) */  
#include <linux/errno.h>                /* For the -ENODEV/... values */  
#include <linux/kernel.h>               /* For printk/panic/... */  
#include <linux/fs.h>                   /* For file operations */ 
#include <linux/ioport.h>               /* For io-port access */  
#include <linux/platform_device.h>      /* For platform_driver framework */  
#include <linux/init.h>                 /* For __init/__exit/... */  
#include <linux/uaccess.h>              /* For copy_to_user/put_user/... */  
#include <linux/io.h>                   /* For inb/outb/... */  
#include <linux/gpio.h>  
#include <linux/device.h>  
#include <linux/cdev.h>  
#include <linux/slab.h>               /*kamlloc */ 
#include <linux/delay.h>

#include <linux/sys_config.h>


//#define motor_en GPIOB(18)
//#define motor_reset  GPIOB(19)
#define motor_dir GPIOI(0)
//#define motor_sleep GPIOI(1)

#define motor_en GPIOB(18)
#define  motor_m0  GPIOB(19)
#define  motor_m1  GPIOI(1)
//#define  motor_m2  GPIOI(0)




static int  major = 219;  
static struct class *motor_class;


struct cdev_motor {  
struct cdev cdev;  
}; 
struct cdev_motor *motor_dev;  



static long motor_ioctl(struct file* filp, unsigned int cmd, unsigned long argv)  
{
#if 1	
	switch(cmd)
	{
		case 3:
		{
			gpio_direction_output(motor_m0, 0);
			gpio_direction_output(motor_m1, 0);
			break;
		}
		case 4:
		{
			gpio_direction_output(motor_m0, 1);
			gpio_direction_output(motor_m1, 0);
			break;
		}
		case 5:
		{
			//gpio_direction_output(motor_m0, 0);
			gpio_direction_output(motor_m1, 0);
			break;
		}
		case 6:
		{
			gpio_direction_output(motor_m0, 0);
			gpio_direction_output(motor_m1, 1);
			break;
		}
		case 7:
		{
			gpio_direction_output(motor_m0, 1);
			gpio_direction_output(motor_m1, 1);
			break;
		}
		case 8:
		{
			//gpio_direction_output(motor_m0, 0);
			gpio_direction_output(motor_m1, 1);
			break;
		}
		case 9:
		{
			gpio_direction_output(motor_m0, 0);
			//gpio_direction_output(motor_m1, 0);
			break;
		}
		case 10:
		{
			gpio_direction_output(motor_m0, 1);
			//gpio_direction_output(motor_m1, 0);
			break;
		}
		case 11:
		{
			//gpio_direction_output(motor_m0, 0);
			//gpio_direction_output(motor_m1, 0);
			break;
		}
		case 12:
		{
			gpio_direction_output(motor_en, 0);
			//gpio_direction_output(motor_m1, 0);
			break;
		}
		case 13:
		{
			gpio_direction_output(motor_en, 1);
			//gpio_direction_output(motor_m1, 0);
			break;
		}
		
		default:break;

	}

	return 0;
#endif
}  


 
static int motor_open(struct inode* i_node, struct file* filp)  
{
	//printk("init gpio configure!\n");

	
	if(0 != gpio_request(motor_en,NULL))
		printk("gpio_request motor_en failed!\n");
	
	if(0 != gpio_request(motor_m0,NULL))
		printk("gpio_request motor_reset failed!\n");

	if(0 != gpio_request(motor_m1,NULL))
		printk("gpio_request motor_dir failed!\n");

	if(0 != gpio_request(motor_dir,NULL))
		printk("gpio_request motor_sleep failed!\n");
#if 0	
	gpio_direction_output(motor_en, 1);
	gpio_direction_output(motor_m0, 0);
	gpio_direction_output(motor_m1, 0);
	gpio_direction_output(motor_dir, 1);

#endif	
	return 0;  
}  



ssize_t motor_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	
	

	
	
	return 1;
}


static ssize_t motor_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
#if 0
	char native_buf[6];
	char which_gpio[3];
	char match_status[3];
	char i;
	copy_from_user(native_buf,buf,sizeof(native_buf));
	for(i=0;i<3;i++)
	{
		which_gpio[i] = native_buf[i*2];
		match_status[i] = native_buf[i*2+1];
	}

	for(i=0;i<3;i++)
	{
		__gpio_set_value(which_gpio[i], match_status[i]);
	}
	
	return 0;
#endif
}
static int motor_close(struct inode* i_node, struct file* filp)  
{

	gpio_free(motor_en);
	gpio_free(motor_m0);
	gpio_free(motor_m1);
	gpio_free(motor_dir);

	return 0;  
}  


/* file operations */  
struct file_operations motor_fops={  
	.owner  = THIS_MODULE,  
	.open   = motor_open,  
	.unlocked_ioctl = motor_ioctl, 
	.release= motor_close,
	.write  = motor_write,
	.read   = motor_read,
};  


static int __init motor_init(void)  
{
	dev_t dev_no;  
	int result, err;

	err = alloc_chrdev_region(&dev_no, 0, 1, "mdmedical_motor"); //鍔ㄦ€佺敵璇疯澶囧彿  
	if(err<0)  
	{  
		printk(KERN_INFO "ERROR\n");
		return err;  
	}  

	major = MAJOR(dev_no);  
	motor_dev = kmalloc(sizeof(struct cdev_motor), GFP_KERNEL);  

	if(!motor_dev)  
	{  
		result = -ENOMEM;
		goto fail_malloc;  
	} 
	
	memset(motor_dev, 0, sizeof(motor_dev));  
	cdev_init(&motor_dev->cdev,&motor_fops); // 鍒濆鍖朿dev  
	motor_dev->cdev.owner = THIS_MODULE;  
	result = cdev_add(&motor_dev->cdev, dev_no, 1); //鍔犺浇璁惧  

	if(result <0)  
	{
		printk(KERN_INFO "error\n");  
		goto fail_add;  
	}  

	motor_class = class_create(THIS_MODULE, "mdmedical_motor");  //鍦╯ys/class涓嬪垱寤簊ysfs鏂囦欢  
	device_create(motor_class, NULL, MKDEV(major, 0), NULL, "mdmedical_motor"); //鍔ㄦ€佸垱寤鸿澶囨枃浠? /dev/mygpio锛?浠ュ悗涓嶇敤鎵嬪姩鍒涘缓浜?

	return 0;  

	fail_add:  
	kfree(motor_dev);  
	fail_malloc:  
	unregister_chrdev_region(dev_no, 1); 

	return result;
}  


static void __exit motor_exit(void)  
{  
	dev_t dev_no=MKDEV(major, 0);  
	unregister_chrdev_region(dev_no, 1);  
	cdev_del(&motor_dev->cdev);  
	kfree(motor_dev);  
	device_destroy(motor_class, dev_no);  
	class_destroy(motor_class);
}


module_init(motor_init);
module_exit(motor_exit);


MODULE_AUTHOR("koliy <xxxx@163.com>");  
MODULE_DESCRIPTION("ARM test gpio");  
MODULE_LICENSE("GPL");  
