make
sudo insmod ./calc.ko

echo 3+4 > /proc/calc_write
cat /proc/calc_read
echo 10-5 > /proc/calc_write
cat /proc/calc_read
echo 3*5 > /proc/calc_write
cat /proc/calc_read
echo 15/4 > /proc/calc_write
cat /proc/calc_read

dmesg | tail -n 1
sudo rmmod calc
make clean
