import sys
import os

if 'EN' != os.environ["LANG"]:
    print('ERR LANG ENV!')
    print('please run command like:\n   LANG=EN python %s rootfs' % sys.argv[0])
    exit()

if len(sys.argv) != 2:
    print("need a param, which means ramdisk root dir!")
    exit()

if not os.path.exists(sys.argv[1]):
    print("Invalid root dir!")
    exit()

if not os.path.exists('./scripts/gen_initramfs_list.sh'):
    print("can not find ./scripts/gen_initramfs_list.sh, pls make kernel firstly!")
    exit()

if not os.path.exists('./usr/gen_init_cpio'):
    print("can not find ./usr/gen_init_cpio, pls make kernel firstly!")
    exit()

print("gen file list now")
if os.path.exists("/tmp/rootfs_file"):
    print("rm old file list!")
    os.system("rm /tmp/rootfs_file")

gen_exec = "./scripts/gen_initramfs_list.sh %s > /tmp/rootfs_file" % sys.argv[1]
os.system(gen_exec)

print("gen new rootfs.cpio")
if not os.path.exists('/tmp/rootfs_file'):
    print("can not find /tmp/rootfs_file, gen file list failed!")
    exit()

gen_rootfs_exec = "./usr/gen_init_cpio /tmp/rootfs_file > rootfs.cpio"
os.system(gen_rootfs_exec)
