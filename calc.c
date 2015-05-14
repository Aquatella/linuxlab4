#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>

int first, second;
char operation;

ssize_t calc_write(struct file *filp, const char *buf, size_t count, loff_t *offp)
{
	int i, tmp = 0, sign = 1, start_index = 0;
	bool operation_flag = false;
	for (i = 0 ; i < count - 1; ++i) {
		if (buf[i] >= '0' && buf[i] <= '9') {
			tmp = tmp * 10 + (buf[i]-'0');
		}
		else {
			if (buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/'){
				if(i == start_index && buf[i] == '-')
					sign = -1;
				else{
					if(!operation_flag){
						operation = buf[i];
						operation_flag = true;
						start_index = i+1;
						first = tmp * sign;
						tmp = 0;
						sign = 1;
					}
					else{
						printk(KERN_INFO "Wrong input.\n");
						return count;
					}
				}
			}
			else {
				printk(KERN_INFO "Wrong input.\n");
				return count;
			}
		}
	}
	second = tmp * sign;
	return count;
}

ssize_t calc_read(struct file *filp, char *buf, size_t count, loff_t *offp)
{
	char str[15];
	int length = 0, res = 0;
	static int finished = 0;
	if (operation == '+')
		res = first + second;
	if (operation == '-')
		res = first - second;
	if (operation == '*')
		res = first * second;
	if (operation == '/') {
		if (second == 0){
			printk(KERN_INFO "Division by zero.\n");
			return 0;
		}
		res = first / second;
	}
	length = sprintf(str, "%d%c%d = %d\n",first, operation, second, res);
	if ( finished ) {
		finished = 0;
		return 0;
	}
	finished = 1;
	strcpy(buf, str);
	return length;
}

static const struct file_operations calc_read_fops = {
	.owner = THIS_MODULE,
	.read = calc_read
};

static const struct file_operations calc_write_fops = {
	.owner = THIS_MODULE,
	.write = calc_write
};
	

static int __init proc_init(void)
{
	proc_create("calc_write", 0666, NULL, &calc_write_fops);
	proc_create("calc_read", 0, NULL, &calc_read_fops);
	return 0;
}
static void __exit proc_exit(void)
{
	remove_proc_entry("calc_read", NULL);
	remove_proc_entry("calc_write", NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_exit);

