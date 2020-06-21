#use to learn linux kernel with arm64 env

#get base system img
read rootfs/ubuntu_20.04_arm64/build.md

#init env
source ./aarch64_env_export
make defconfig
make -j16

#refs command to start a vm
./start_qemu.py -h
