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

#include<linux/interrupt.h>
#include<linux/irq.h>
#include<linux/wait.h>
#include<asm/irq.h>

#include<linux/kthread.h>



static int  major = 219;  
static struct class *gpio_class;
static int button_irq[8];
static int is_press;
static struct timer_list buttons_timer;
static struct pin_desc *irq_pd;
static unsigned char key_val;


static DECLARE_WAIT_QUEUE_HEAD(btn_wq);

struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

struct pin_desc pins_desc[8] = {
	{GPIOH(4), 0x01},
	{GPIOH(5), 0x02},
	{GPIOH(0), 0x03},
	{GPIOH(13), 0x04},
	{GPIOI(16), 0x05},
	{GPIOI(17), 0x06},
	{GPIOI(12), 0x07},
	{GPIOH(2), 0x08},
};

char pin_num[8] = {4,5,0,13,16,17,12,2};
const char *key_name[16]={"key_4","key_5","key_8","key_13","key_16","key_17","key_12","key_I17"};
struct cdev_gpio {  
struct cdev cdev;  
}; 
struct cdev_gpio *mdgpio_dev;  


static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	/* 10ms后启动定时器 */
	irq_pd = (struct pin_desc *)dev_id;


	mod_timer(&buttons_timer, jiffies+HZ/100);
	
	
	return IRQ_RETVAL(IRQ_HANDLED);
}



static void buttons_timer_function(unsigned long data)
{
	struct pin_desc * pindesc = irq_pd;
	if (!pindesc)
		return;

	unsigned int pinval;
	
	pinval = __gpio_get_value(pindesc->pin);
	if (pinval)
	{
		/* 松开 */
		key_val = 0x80 | pindesc->key_val;
	}
	else
	{
		/* 按下 */
		key_val = pindesc->key_val;
	}

	//mdkey_val=pindesc->key_val;
	
	
	wake_up_interruptible(&btn_wq);
	is_press=1;
 
}

  


 
static int mdmical_buttons_open(struct inode* i_node, struct file* filp)  
{
	//printk("init gpio configure!\n");
	int error;
	char i;

	for(i = 0;i < 8;i++)
	{
		if(0 != gpio_request(pins_desc[i].pin,NULL))
			printk("gpio_request GPIOH(%d) failed!\n",pin_num[i]);
		
	}
	
	for(i = 0;i < 8;i++)
	{
		button_irq[i]=gpio_to_irq(pins_desc[i].pin);
		if (IS_ERR_VALUE(button_irq[i])) 
		{
			printk("map gpio [%d] to virq failed, errno = %d\n",pins_desc[i].pin, button_irq[i]);
		}
		
	}

	for(i = 0;i < 8;i++)
	{
		error=request_irq(button_irq[i],buttons_irq,IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,key_name[i],&pins_desc[i]); //上升沿下降沿触发
		if(error)
		{
			printk("request irq %s error\n",key_name[i]);
		}
	
	}
	
	return 0;  
}  




ssize_t mdmical_buttons_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	if (size != 1)
		return -EINVAL;

	/* 如果没有按键动作, 休眠 */
	wait_event_interruptible(btn_wq, is_press);

	/* 如果有按键动作, 返回键值 */
	copy_to_user(buf, &key_val, 1);
	is_press = 0;
	return 1;
}



static int mdmical_buttons_close(struct inode* i_node, struct file* filp)  
{
	printk("free gpio resource!\n");
	char i = 0;
	for(i = 0;i < 8;i++)
	{
		free_irq(button_irq[i],&pins_desc[i]);	
		gpio_free(pins_desc[i].pin);
	}

	return 0;  
}  


/* file operations */  
struct file_operations mdgpio_fops={  
	.owner  = THIS_MODULE,  
	.open   = mdmical_buttons_open,  
	.release= mdmical_buttons_close,
	.read   = mdmical_buttons_read,
};  


static int __init buttons_init(void)  
{
	dev_t dev_no;  
	int result, err;

	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	//buttons_timer.expires  = 0;
	add_timer(&buttons_timer);

	err = alloc_chrdev_region(&dev_no, 0, 1, "mdmedical_buttons"); //鍔ㄦ�佺敵璇疯澶囧彿  
	if(err<0)  
	{  
		printk(KERN_INFO "ERROR\n");
		return err;  
	}  

	major = MAJOR(dev_no);  
	mdgpio_dev = kmalloc(sizeof(struct cdev_gpio), GFP_KERNEL);  

	if(!mdgpio_dev)  
	{  
		result = -ENOMEM;
		goto fail_malloc;  
	} 
	
	memset(mdgpio_dev, 0, sizeof(mdgpio_dev));  
	cdev_init(&mdgpio_dev->cdev,&mdgpio_fops); 
	mdgpio_dev->cdev.owner = THIS_MODULE;  
	result = cdev_add(&mdgpio_dev->cdev, dev_no, 1); 

	if(result <0)  
	{
		printk(KERN_INFO "error\n");  
		goto fail_add;  
	}  

	gpio_class = class_create(THIS_MODULE, "mdmedical_buttons"); 
	device_create(gpio_class, NULL, MKDEV(major, 0), NULL, "mdmedical_buttons");
	return 0;  

	fail_add:  
	kfree(mdgpio_dev);  
	fail_malloc:  
	unregister_chrdev_region(dev_no, 1); 

	return result;
}  


static void __exit buttons_exit(void)  
{
	dev_t dev_no=MKDEV(major, 0); 

	
	
	unregister_chrdev_region(dev_no, 1);  
	cdev_del(&mdgpio_dev->cdev);  
	kfree(mdgpio_dev);  
	device_destroy(gpio_class, dev_no);  
	class_destroy(gpio_class);
}


module_init(buttons_init);
module_exit(buttons_exit);


MODULE_AUTHOR("koliy <xxxx@163.com>");  
MODULE_DESCRIPTION("ARM test gpio");  
MODULE_LICENSE("GPL");  

