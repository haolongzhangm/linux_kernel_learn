#!/usr/bin/env python3

import argparse
import os

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
            "-i",
            "--image",
            help="input system ext4 image path",
            dest="system_image",
            required=True,
            )

    parser.add_argument(
            "-g",
            "--gdb",
            action="store_true",
            help="enable gdb interface, append -s -S to command, use default port: 1234",
            )

    parser.add_argument(
            "-d",
            "--display",
            action="store_true",
            help="enable GUI",
            )

    parser.add_argument(
            "-c",
            "--cpu",
            dest="cpu_number",
            default=4,
            type=int,
            help="assgin cpu number",
            )

    parser.add_argument(
            "-m",
            "--memory",
            dest="memory",
            default=1024,
            type=int,
            help="assgin memory size",
            )

    parser.add_argument(
            "-b",
            "--bochs",
            action="store_true",
            help="use bochs-display, default use VGA"
            )

    args = parser.parse_args()

    if (not os.path.isfile(args.system_image)):
        print("ERR: invalid system image: %s" % args.system_image)
        exit(-1)

    cmd = "qemu-system-aarch64 -machine virt -cpu cortex-a72 -machine type=virt -smp %d -m %d -kernel arch/arm64/boot/Image --append \"root=/dev/vda rootfstype=ext4 rw exec init=init console=ttyAMA0\" -hda %s -net nic,model=virtio -net user,hostfwd=tcp::2222-:22,hostfwd=tcp::5555-:5555" % (args.cpu_number, args.memory, args.system_image)

    gui_args = " "
    if args.display:
        if args.bochs:
            gui_args = gui_args + "-device bochs-display"
        else:
            gui_args = gui_args + "-device VGA,vgamem_mb=128"
        gui_args = gui_args + " -device usb-ehci -device usb-kbd -device usb-mouse"
    else:
        gui_args = gui_args + "-nographic"
        if args.bochs:
            print("Warn: bochs args only invalid when -d enable")

    gdb_args = " "
    if args.gdb:
        gdb_args = gdb_args + "-s -S"

    cmd = cmd + gui_args + gdb_args

    print(cmd)

    os.system(cmd)

if __name__ == "__main__":
    main()
