# common config
HOST=arm64
USER=zhl


echo "config nameserver for chroot env"
mv resolv.conf /etc/resolv.conf

echo "now in chroot env"
echo "config net..."
echo $HOST > /etc/hostname
echo "127.0.0.1  localhost" > /etc/hosts
echo "127.0.0.1  $HOST" >> /etc/hosts
mkdir -p /etc/network/interfaces.d
echo "auto eth0" > /etc/network/interfaces.d/eth0
echo "iface eth0 inet dhcp" >> /etc/network/interfaces.d/eth0

# add qemu network devices enp0s1
echo "auto enp0s1" > /etc/network/interfaces.d/enp0s1
echo "iface enp0s1 inet dhcp" >> /etc/network/interfaces.d/enp0s1

echo "install more base package"
apt-get update
apt-get upgrade -y
apt-get install apt-utils ifupdown net-tools network-manager udev sudo ssh vim gdb iputils-ping -y
apt-get install systemd resolvconf tzdata -y
# for ubuntu GUI
apt-get install ubuntu-desktop -y
dpkg-reconfigure resolvconf
dpkg-reconfigure tzdata

echo "config user..."
adduser $USER
chmod +w /etc/sudoers
echo "$USER ALL=(ALL:ALL) ALL" >> /etc/sudoers
chmod -w /etc/sudoers

# FIXME: chown may failed, U may need do
# sudo chown chown $USER:$USER -R /home/$USER
# manually whne login system firstly
chown $USER:$USER -R /home/$USER

echo "exit chroot env..."
exit
