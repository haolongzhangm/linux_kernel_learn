echo "now in chroot env"
echo "config net..."
echo "arm64" > /etc/hostname
echo "127.0.0.1  localhost" > /etc/hosts
echo "127.0.0.1  arm64" >> /etc/hosts
mkdir -p /etc/network/interfaces.d
echo "auto eth0" > /etc/network/interfaces.d/eth0
echo "iface eth0 inet dhcp" >> /etc/network/interfaces.d/eth0

#add qemu network devices enp0s1
echo "auto enp0s1" > /etc/network/interfaces.d/enp0s1
echo "iface enp0s1 inet dhcp" >> /etc/network/interfaces.d/enp0s1

#may change DNS for network work
echo "nameserver 127.0.0.53" >> /etc/resolv.conf

echo "install more base package"
apt-get update
apt-get upgrade -y
apt-get install apt-utils ifupdown net-tools network-manager udev sudo ssh vim gdb iputils-ping -y
apt-get install systemd
apt-get install resolvconf
dpkg-reconfigure resolvconf
apt-get install tzdata
dpkg-reconfigure tzdata

echo "config user..."
adduser zhl
chmod +w /etc/sudoers
echo "zhl ALL=(ALL:ALL) ALL" >> /etc/sudoers
chmod -w /etc/sudoers
chown zhl:zhl -R /home/zhl

echo "exit chroot env..."
exit
