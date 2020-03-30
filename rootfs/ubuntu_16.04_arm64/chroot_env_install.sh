echo "now in chroot env"
echo "config net..."
echo "arm64" > /etc/hostname
echo "127.0.0.1    arm64 localhost" > /etc/hosts
echo "127.0.0.1    $HOST" >> /etc/hosts
mkdir -p /etc/network/interfaces.d
echo "auto eth0" > /etc/network/interfaces.d/eth0
echo "iface eth0 inet dhcp" >> /etc/network/interfaces.d/eth0
echo "nameserver 127.0.1.1" > /etc/resolv.conf

echo "config qemu tty compat..."
ln -s /lib/systemd/system/serial-getty\@.service /etc/systemd/system/getty.target.wants/serial-getty@ttyS0.service
ln -s /lib/systemd/system/serial-getty\@.service /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service



echo "install more base package"
apt-get update
apt-get upgrade -y
apt-get install ifupdown net-tools network-manager udev sudo ssh vim gdb iputils-ping -y

echo "config user..."
adduser zhl
chmod +w /etc/sudoers
echo "zhl ALL=(ALL:ALL) ALL" >> /etc/sudoers
chmod -w /etc/sudoers

echo "exit chroot env..."
exit
