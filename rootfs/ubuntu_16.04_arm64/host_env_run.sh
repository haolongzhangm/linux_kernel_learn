echo "host env prepare ..."
sudo apt-get install qemu-user-static binfmt-support debootstrap android-tools-fsutils
sudo rm -rf build
mkdir build
wget http://cdimage.ubuntu.com/ubuntu-base/releases/16.04.2/release/ubuntu-base-16.04.6-base-arm64.tar.gz
sudo tar -xvf ubuntu-base-16.04.6-base-arm64.tar.gz -C build
sudo cp -a /usr/bin/qemu-aarch64-static build/usr/bin/
sudo cp ./init build/init
sudo cp chroot_env_install.sh build/

echo "run chroot env install"
sudo chroot build /chroot_env_install.sh
if [ $? -eq 0 ]; then
	echo "chroot env success"
else
	echo "chroot env failed"
	exit -1
fi

echo "run file clear and create system.img"
sudo rm build/dev/*
sudo make_ext4fs -l 10240M system.img build
sudo chown $USER:$USER system.img
sudo rm -rf build
