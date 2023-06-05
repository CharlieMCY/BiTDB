#!/bin/sh
[ ! -d "./dl" ] && {
	mkdir dl
}

[ ! -d "./out" ] && {
    mkdir out
    mkdir out/boot
    mkdir out/rootfs
    mkdir out/rootfs/bin
    mkdir out/rootfs/lib
    mkdir out/rootfs/lib/optee_armtz
}

[ -f "./dl/arm-trusted-firmware.tar.gz" ] && {
	echo  "\033[32marm-trusted-firmware package existed.\n\033[0m"
} || {
	echo  "\033[32marm-trusted-firmware downloading...\n\033[0m"
	wget -O dl/arm-trusted-firmware.tar.gz https://github.com/ARM-software/arm-trusted-firmware/archive/v2.0.tar.gz
}

[ ! -d "./arm-trusted-firmware" ] && {
	tar zxf dl/arm-trusted-firmware.tar.gz && {
		mv arm-trusted-firmware-* arm-trusted-firmware
	}
}

[ -f "./dl/u-boot.tar.gz" ] && {
	echo  "\033[32mu-boot package existed.\n\033[0m"
} || {
	echo  "\033[32mu-boot downloading...\n\033[0m"
	wget -O dl/u-boot.tar.gz https://github.com/u-boot/u-boot/archive/v2018.11.tar.gz
}

[ ! -d "./u-boot" ] && {
	tar zxf dl/u-boot.tar.gz && {
		mv u-boot-* u-boot
	}
}

[ -f "./dl/linux.tar.gz" ] && {
	echo  "\033[32mlinux package existed.\n\033[0m"
} || {
	echo  "\033[32mlinux downloading...\n\033[0m"
	wget -O dl/linux.tar.gz https://github.com/raspberrypi/linux/archive/raspberrypi-kernel_1.20190215-1.tar.gz
}

[ ! -d "./linux" ] && {
	tar zxf dl/linux.tar.gz && {
		mv linux-* linux
	}
}

