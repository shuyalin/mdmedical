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




#define SW2 GPIOB(15) 
#define SW1 GPIOB(16) 
#define SW0 GPIOH(11) 



#define IIC_CS GPIOH(7)  //IIC_CS
#define IIC_SCL GPIOB(14) //IIC_SCL
#define IIC_DATA GPIOH(6) //IIC_DATA
						
#define PIN_PIPE GPIOA(7)



static int  major = 219;  
static struct class *gpio_class;
static char what;


struct cdev_gpio {  
struct cdev cdev;  
}; 
struct cdev_gpio *gpio_dev;  

#if 0
void temperature_channel(int n)
{
  if(n<=7 && n>=0)
  {    
     TEMPER_A2=(n&0x4)>>2;
     TEMPER_A1=(n&0x2)>>1;
     TEMPER_A0=n&0x1;
     //delay_us(100);
     delay_ms(2);
  }  
}
#endif


//PB15-----SW0  
//PB16-----SW1
//PH11-----SW2

/*  A0  A1  A2
	0   0   0     一通道针温度
	0   0   1     一通道粘膜温度
	0   1   0     二通道针温度
	0   1   1     二通道粘膜温服
	1   0   0     三通道针温度
	1   0   1     三通道粘膜温度
	1   1   0     四通道针温度
	1   1   1     四通道粘膜温度
*/


static long mdmical_tmp_ioctl(struct file* filp, unsigned int cmd, unsigned long argv)  
{

	switch(cmd)
	{
		case 3:
			__gpio_set_value(SW0, 0);
			__gpio_set_value(SW1, 0);
			__gpio_set_value(SW2, 0);
			break;
		case 4:
			__gpio_set_value(SW0, 1);
			__gpio_set_value(SW1, 0);
			__gpio_set_value(SW2,0);
			break;
		case 5:
			__gpio_set_value(SW0, 0);
			__gpio_set_value(SW1, 1);
			__gpio_set_value(SW2, 0);
			break;
		case 6:
			__gpio_set_value(SW0, 1);
			__gpio_set_value(SW1, 1);
			__gpio_set_value(SW2, 0);
			break;
		case 7:
			__gpio_set_value(SW0, 0);
			__gpio_set_value(SW1, 0);
			__gpio_set_value(SW2, 1);
			break;
		case 8:
			__gpio_set_value(SW0, 1);
			__gpio_set_value(SW1, 0);
			__gpio_set_value(SW2, 1);
			break;
		case 9:
			__gpio_set_value(SW0, 0);
			__gpio_set_value(SW1, 1);
			__gpio_set_value(SW2, 1);
			break;
		case 10:
			__gpio_set_value(SW0, 1);
			__gpio_set_value(SW1, 1);
			__gpio_set_value(SW2, 1);
			break;
		default:
			printk("illigal command!\n");
			;break;
	}
	return 0;  
}  


 
static int mdmical_tmp_open(struct inode* i_node, struct file* filp)  
{
	//printk("init gpio configure!\n");

	
	if(0 != gpio_request(IIC_SCL,NULL))
		printk("gpio_request IIC_SCL failed!\n");
	if(0 != gpio_request(SW0,NULL))
		printk("gpio_request SW0 failed!\n");
	if(0 != gpio_request(SW1,NULL))
		printk("gpio_request SW1 failed!\n");
	if(0 != gpio_request(SW2,NULL))
		printk("gpio_request SW2 failed!\n");		
	
	gpio_direction_output(IIC_SCL, 1);
	gpio_direction_output(SW0, 0);
	gpio_direction_output(SW1, 0);
	gpio_direction_output(SW2, 1);



	if(0 != gpio_request(IIC_CS,NULL))
		printk("gpio_request IIC_CS failed!\n");
	if(0 != gpio_request(IIC_DATA,NULL))
		printk("gpio_request IIC_DATA failed!\n");
	gpio_direction_output(IIC_CS,1);
	gpio_direction_input(IIC_DATA);
	

	if(0 != gpio_request(PIN_PIPE,NULL))
		printk("gpio_request PIN_PIPE failed!\n");
	gpio_direction_input(PIN_PIPE);
	
	return 0;  
}  


ssize_t mdmical_tmp_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned char i,j,k;
	unsigned short adc_value=0,tmp_valve[16]={0};
	unsigned long flags;
	int pin_status = -1;

	if(what == 0)
	{
		preempt_disable();
		local_irq_save(flags);

		for(j=0;j<16;j++)
		{
			__gpio_set_value(IIC_CS,0);//IIC_CS=0;      //powerup 至少拉低1US
		  	udelay(1);
			__gpio_set_value(IIC_CS,1);//IIC_CS=1; 

			
		  	for(i=0;i<16;i++)
		  	{
		  		adc_value<<=1;
				for(k=0;k<39;k++)nop();
				__gpio_set_value(IIC_SCL,1);//IIC_SCL=1;
				
		    	if(__gpio_get_value(IIC_DATA))	    		
					adc_value++; 	
	    		
				__gpio_set_value(IIC_SCL,0);//IIC_SCL=0;		

		  	}
			
			__gpio_set_value(IIC_SCL,1);//IIC_SCL=1;
			
			
			udelay(10);
			
			tmp_valve[j] = adc_value;
			//printk("a: %hu ",adc_value);
			
		}
		//printk("\n");

		local_irq_restore(flags);
		preempt_enable();

		copy_to_user(buf,tmp_valve,32);
	}
	else if(what == 1)
	{
		pin_status = __gpio_get_value(PIN_PIPE);
		copy_to_user(buf,&pin_status,4);
	}
	return 1;
}


static ssize_t mdmical_tmp_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
#if 1
	copy_from_user(&what,buf,1);

#endif	
	return 0;
}
static int mdmical_tmp_close(struct inode* i_node, struct file* filp)  
{
	//printk("free gpio resource!\n");

	gpio_free(IIC_SCL);
	gpio_free(SW0);
	gpio_free(SW1);
	gpio_free(SW2);

	gpio_free(IIC_CS);
	gpio_free(IIC_DATA);


	gpio_free(PIN_PIPE);
	return 0;  
}  


/* file operations */  
struct file_operations gpio_fops={  
	.owner  = THIS_MODULE,  
	.open   = mdmical_tmp_open,  
	.unlocked_ioctl = mdmical_tmp_ioctl, 
	.release= mdmical_tmp_close,
	.write  = mdmical_tmp_write,
	.read   = mdmical_tmp_read,
};  


static int __init gpio_init(void)  
{
	dev_t dev_no;  
	int result, err;

	err = alloc_chrdev_region(&dev_no, 0, 1, "mdmedical_tmp"); //鍔ㄦ€佺敵璇疯澶囧彿  
	if(err<0)  
	{  
		printk(KERN_INFO "ERROR\n");
		return err;  
	}  

	major = MAJOR(dev_no);  
	gpio_dev = kmalloc(sizeof(struct cdev_gpio), GFP_KERNEL);  

	if(!gpio_dev)  
	{  
		result = -ENOMEM;
		goto fail_malloc;  
	} 
	
	memset(gpio_dev, 0, sizeof(gpio_dev));  
	cdev_init(&gpio_dev->cdev,&gpio_fops); // 鍒濆鍖朿dev  
	gpio_dev->cdev.owner = THIS_MODULE;  
	result = cdev_add(&gpio_dev->cdev, dev_no, 1); //鍔犺浇璁惧  

	if(result <0)  
	{
		printk(KERN_INFO "error\n");  
		goto fail_add;  
	}  

	gpio_class = class_create(THIS_MODULE, "mdmedical_tmp");  //鍦╯ys/class涓嬪垱寤簊ysfs鏂囦欢  
	device_create(gpio_class, NULL, MKDEV(major, 0), NULL, "mdmedical_tmp"); //鍔ㄦ€佸垱寤鸿澶囨枃浠? /dev/mygpio锛?浠ュ悗涓嶇敤鎵嬪姩鍒涘缓浜?

	return 0;  

	fail_add:  
	kfree(gpio_dev);  
	fail_malloc:  
	unregister_chrdev_region(dev_no, 1); 

	return result;
}  


static void __exit gpio_exit(void)  
{  
	dev_t dev_no=MKDEV(major, 0);  
	unregister_chrdev_region(dev_no, 1);  
	cdev_del(&gpio_dev->cdev);  
	kfree(gpio_dev);  
	device_destroy(gpio_class, dev_no);  
	class_destroy(gpio_class);
}


module_init(gpio_init);
module_exit(gpio_exit);


MODULE_AUTHOR("koliy <xxxx@163.com>");  
MODULE_DESCRIPTION("ARM test gpio");  
MODULE_LICENSE("GPL");  
