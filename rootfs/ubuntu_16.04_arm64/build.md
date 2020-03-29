# host side env prepare
sudo apt-get install qemu-user-static binfmt-support debootstrap android-tools-fsutils

mkdir build

wget http://cdimage.ubuntu.com/ubuntu-base/releases/16.04.2/release/ubuntu-base-16.04.5-base-arm64.tar.gz

sudo tar -xvf ubuntu-base-16.04.5-base-arm64.tar.gz -C build

sudo cp -a /usr/bin/qemu-aarch64-static build/usr/bin/

sudo cp ./init build/init

sudo chroot build


# rootfs side after run: sudo chroot build
## config net
echo "home" > /etc/hostname 

echo "localhost.localdomain" > /etc/hostname

echo "127.0.0.1    localhost.localdomain localhost" > /etc/hosts

echo "127.0.0.1    $HOST" >> /etc/hosts

mkdir -p /etc/network/interfaces.d

echo "auto eth0" > /etc/network/interfaces.d/eth0

echo "iface eth0 inet dhcp" >> /etc/network/interfaces.d/eth0

echo "nameserver 127.0.1.1" > /etc/resolv.conf

## config qemu tty compat
ln -s /lib/systemd/system/serial-getty\@.service /etc/systemd/system/getty.target.wants/serial-getty@ttyS0.service

ln -s /lib/systemd/system/serial-getty\@.service /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service



## install more base package
apt-get update

apt-get upgrade

apt-get install ifupdown net-tools network-manager udev sudo ssh vim gdb iputils-ping

## config user
adduser zhl

chmod +w /etc/sudoers

echo "zhl ALL=(ALL:ALL) ALL" >> /etc/sudoers

chmod -w /etc/sudoers


## exit chroot env, create system.img and clear tmp file
exit

sudo rm build/dev/*

sudo make_ext4fs -l 4096M system.img build

sudo chown $USER:$USER system.img

sudo rm -rf build
