#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/tfos.elf isodir/boot/install
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Install Techflash OS" {
	multiboot /boot/install
}
EOF
grub-mkrescue -o bin/TFOS_ISO.iso isodir
rm -r isodir