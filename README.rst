BiTDB with OP-TEE Support (based on the project raspbian-tee)
============================

Introduction
------------

This project discribed how to integrating BiTDB on Raspbian Pi 3B.

Dependence
----------

- Host OS: Ubuntu 16.04 or later.
- Cross Build Toolchain: AARCH64 & AARCH32 both needed, and AARCH32 must > 6.0. (you can get one from `linaro`_)

- Hardware: Raspberry Pi 3B board.

Build
-----
First, config your cross build toolchain at config.mk.

Then:

.. code:: bash

	$ ./prepare-env.sh # if your had download all packages, skip this.
	$ make patch # this will patch linux kernel & ATF, if you have done before, skip this.
	$ make

When success, it should seem as:

.. code:: bash

    ......
    INSTALL sound/usb/hiface/snd-usb-hiface.ko
    INSTALL sound/usb/misc/snd-ua101.ko
    INSTALL sound/usb/snd-usb-audio.ko
    INSTALL sound/usb/snd-usbmidi-lib.ko
    DEPMOD  4.14.98-v7
  make[1]: Leaving directory '/***/raspbian-tee/linux'
  cp /***/raspbian-tee/linux/arch/arm/boot/uImage ./out/boot/
  cp /***/raspbian-tee/linux/arch/arm/boot/dts/bcm2710-rpi-3-b-plus.dtb ./out/boot/
  cp /***/raspbian-tee/linux/arch/arm/boot/dts/bcm2710-rpi-3-b.dtb ./out/boot/
  cp /***/raspbian-tee/firmware/config.txt ./out/boot/

Install
-------

Prepare a SD Card Flashed with an official Raspbian image (2019-06-20-raspbian-buster.img), mount it in linux, such as /media/user/boot & /media/user/rootfs.

Then:

.. code:: bash

	$ cp ./out/boot/* /media/user/boot
	$ sudo cp -r ./out/rootfs/* /media/user/rootfs

Boot Rsapberry Pi with the Modified image in SD Card.

When you are logined,then:

.. code:: bash

	$ ls /dev/tee*
	/dev/tee0 /dev/teepriv0 # this prove tee driver & optee-os works.
	$ sudo tee-supplicant &
	$ sudo optee_example_bitdb tpc #this will run a TPC-H test, you need to put TPC-H-small.db in /home/pi

It should be no errors, then all is OK.

.. _linaro: https://releases.linaro.org/components/toolchain/binaries/

Publication
-------
Our paper **BiTDB: Constructing A Built-in TEE Secure Database for Embedded Systems** have been published in the journal **IEEE Transactions on Knowledge and Data Engineering**.

.. code::

	@ARTICLE{10477533,
  		author={Ma, Chengyan and Lu, Di and Lv, Chaoyue and Xi, Ning and Jiang, Xiaohong and Shen, Yulong and Ma, Jianfeng},
  		journal={IEEE Transactions on Knowledge and Data Engineering}, 
  		title={BiTDB: Constructing A Built-in TEE Secure Database for Embedded Systems}, 
  		year={2024},
  		volume={36},
  		number={9},
  		pages={4472-4485},
  		doi={10.1109/TKDE.2024.3380367}
	}
