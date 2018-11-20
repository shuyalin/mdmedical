#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>


static struct i2c_board_info pcf8563_info = {	
	I2C_BOARD_INFO("pcf8563", 0x51),
};

static struct i2c_client *pcf8563_client;

static int pcf8563_dev_init(void)
{
	struct i2c_adapter *i2c_adap;
	i2c_adap = i2c_get_adapter(3);
	pcf8563_client = i2c_new_device(i2c_adap, &pcf8563_info);
	i2c_put_adapter(i2c_adap);
	
	return 0;
}

static void pcf8563_dev_exit(void)
{
	i2c_unregister_device(pcf8563_client);
}


module_init(pcf8563_dev_init);
module_exit(pcf8563_dev_exit);
MODULE_LICENSE("GPL");


