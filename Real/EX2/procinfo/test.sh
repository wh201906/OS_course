sudo insmod ./procinfo.ko pidVal=$1
sudo rmmod procinfo
dmesg