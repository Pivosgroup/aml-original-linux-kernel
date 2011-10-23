- Setup TFTP Server 
sudo apt-get install tftpd-hpa
check /etc/default/tftpd-hpa for TFTP_OPTIONS="-s" or TFTP_OPTIONS="--secure", add it if not
sudo /etc/init.d/tftpd-hpa restart

- Setup NFS Server
sudo apt-get install nfs-kernel-server
add to /etc/exports and change 192.168.2 to match your local net setup
/opt/aml-rootfs 192.168.2.0/255.255.255.0(rw,sync,no_root_squash,no_all_squash,anonuid=0,anongid=0,no_subtree_check)
sudo mkdir -p /opt/aml-rootfs
sudo /etc/init.d/nfs-kernel-server restart

The kernel uses the gnu cross toolchain. I place it in /opt. 
I also create a symlink so buildroot can also use it:
 cd /opt
 #wget http://openlinux.amlogic.com/download/linux/ARM/gnutools/arm-2010q1-202-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
 wget http://www.codesourcery.com/sgpp/lite/arm/portal/package6488/public/arm-none-linux-gnueabi/arm-2010q1-202-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
 tar xf arm-2010q1-202-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
 ls /opt/arm-2010q1
 sudo mkdir -p /opt/CodeSourcery
 sudo ln -s /opt/arm-2010q1 /opt/CodeSourcery/Sourcery_G++_Lite

Building kernel:
 export PATH=/opt/arm-2010q1/bin:$PATH
 cp kernel.config arm-kernel-src/.config
 cd arm-kernel-src
 make uImage
 make modules

 uImage for 8726 is at arch/arm/boot/uImage
 Mali driver is at drivers/amlogic/mali/mali.ko

 sudo cp arch/arm/boot/uImage /var/lib/tftpboot
 sudo mkdir -p /opt/aml-rootfs/lib/modules/
 sudo cp drivers/amlogic/mali/mali.ko /opt/aml-rootfs/lib/modules/

To boot the amlogic board:
 attach usb/serial-ttl to pin white connector (pwr , tx, gnd, and rx)
 picocom --baud=115200 /dev/ttyUSB0
 power up and do a few returns to halt the boot.

Change the boot vars to (change IPs to reflect your setup),
You only need to do this once as the setting will get saved:
 setenv machid 2956
 setenv serverip 192.168.2.200
 setenv gatewayip 192.168.2.1
 setenv ipaddr 192.168.2.214
 setenv bootargs root=/dev/nfs nfsroot=192.168.2.200:/opt/aml-rootfs rw noinitrd init=/init ip=192.168.2.214:192.168.2.200:192.168.2.1:255.255.255.0:target:eth0:off console=ttyS0,115200 a9_clk=800M clk81=200000k
 saveenv

Do a tftpboot from the server:
 tftpboot 0x82000000 uImage;bootm 0x82000000

To restore android kernel boot:
  setenv machid 2957
  saveenv

