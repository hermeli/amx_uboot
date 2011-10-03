/*
 * Stefan Wyss (swyss@kbr.kaba.com)
 * KABA AG, CC EAC
 *
 * Configuation settings for the AMM board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* ARM asynchronous clock */
#define AT91_MAIN_CLOCK		720000000	/* from 25.000 MHz crystal */
#define AT91_MASTER_CLOCK	90000000	/* peripheral = main / 2 */
#define CFG_HZ			1000000		/* 1us resolution */

#define AT91_SLOW_CLOCK		32768	/* slow clock */

#define CONFIG_ARM926EJS	1	/* This is an ARM926EJS Core	*/
#define CONFIG_AT91SAM9260	1	/* It's an Atmel AT91SAM9260 SoC*/
#define CONFIG_AMM		1	/* on an AMM Board	*/
#undef CONFIG_USE_IRQ			/* we don't need IRQ/FIQ stuff	*/

#define CONFIG_CMDLINE_TAG	1	/* enable passing of ATAGs	*/
#define CONFIG_SETUP_MEMORY_TAGS 1
#define CONFIG_INITRD_TAG	1

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SKIP_RELOCATE_UBOOT

/*
 * Hardware drivers
 */
#define CONFIG_ATMEL_USART	1
#undef CONFIG_USART0
#undef CONFIG_USART1
#undef CONFIG_USART2
#define CONFIG_USART3		1	/* USART 3 is DBGU */
#define CONFIG_BOOTDELAY	3

#define CONFIG_HW_WATCHDOG	1
#define CONFIG_AT91SAM9_WATCHDOG	1

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE	1
#define CONFIG_BOOTP_BOOTPATH		1
#define CONFIG_BOOTP_GATEWAY		1
#define CONFIG_BOOTP_HOSTNAME		1

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_IMI
#define CONFIG_CMD_AUTOSCRIPT	1
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_IMLS

#define CONFIG_CMD_NET		1
#define CONFIG_CMD_PING		1
#define CONFIG_CMD_DHCP		1
#define CONFIG_CMD_USB		1
#undef CONFIG_CMD_NAND

/* SDRAM */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			0x20000000
#define PHYS_SDRAM_SIZE			0x04000000	/* 64 megs */

/* NOR flash */
#define CFG_FLASH_CFI			1
#define CFG_FLASH_CFI_DRIVER		1
#define PHYS_FLASH_1			0x10000000
#define CFG_FLASH_BASE			PHYS_FLASH_1
#define CFG_MAX_FLASH_SECT		512
#define CFG_MAX_FLASH_BANKS		1
#define CONFIG_FLASH_SHOW_PROGRESS	100

/* USB */
#define CONFIG_USB_OHCI_NEW		1
#define LITTLEENDIAN			1
#define CONFIG_DOS_PARTITION		1
#define CFG_USB_OHCI_CPU_INIT		1
#define CFG_USB_OHCI_REGS_BASE		0x00500000	/* AT91SAM9260_UHP_BASE */
#define CFG_USB_OHCI_SLOT_NAME		"at91sam9260"
#define CFG_USB_OHCI_MAX_ROOT_PORTS	2
#define CONFIG_USB_STORAGE		1

/* Ethernet */
#define CONFIG_MACB			1
#undef CONFIG_RMII			
#define CONFIG_NET_MULTI		1
#define CONFIG_NET_RETRY_COUNT		20
#define CONFIG_RESET_PHY_R		1

#define CFG_LOAD_ADDR			0x22000000	/* load address */

#define CFG_MEMTEST_START		PHYS_SDRAM
#define CFG_MEMTEST_END			0x23e00000

/* bootstrap + u-boot + env + linux in norflash */
#define CONFIG_JFFS2_CMDLINE
#define MTDIDS_DEFAULT "nor=physmap-flash.0"
#define MTDPARTS_DEFAULT "mtdparts=nor:256k(u-boot),1792k(linux),2M(part1),4M(part2),8M(part3),-(part4)"

#define CFG_ENV_IS_IN_FLASH	1
#define CFG_MONITOR_BASE	(PHYS_FLASH_1 + 0x8000)
#define CFG_ENV_OFFSET		0x60000
#define CFG_ENV_ADDR		(PHYS_FLASH_1 + CFG_ENV_OFFSET)
#define CFG_ENV_SIZE		0x20000

#define CONFIG_BOOTCOMMAND	"if test -n ${doinstall}; then run bootscrinstall; else run bootdefault; fi"
#define CONFIG_BOOTARGS		"console=ttyS0,115200 "			\
				"mtdparts=physmap-flash.0:4M(bootstrap)ro,60M(roofs) "\
				"root=1f01 rw "			\
				"rootfstype=jffs2"
#define CONFIG_EXTRA_ENV_SETTINGS "doinstall=1\0" \
				  "iscript=setenv.bin\0" \
				  "kernel=uImage.bin\0" \
				  "rootfs=rootfs.tar.bz2\0" \
				  "autoload=no\0" \
				  "tftpserverip=192.168.2.29\0" \
				  "bootdefault=cp.b 0x10060000 0x22200000 0x200000; bootm 0x22200000\0" \
				  "bootscrinstall=dhcp; setenv serverip ${tftpserverip}; run tftpinstallscript\0" \
				  "bootinstall=dhcp; setenv serverip ${tftpserverip}; run tftpinstallkernel\0" \
				  "tftpinstallscript=tftp 0x21000000 ${iscript} && autoscr 0x21000000\0" \
				  "tftpinstallkernel=tftp 0x21000000 ${kernel} && cp.b 0x21000000 0x10060000 ${filesize} && run tftpinstallrootfs\0" \
				  "tftpinstallrootfs=tftp 0x21000000 ${rootfs} && cp.b 0x21000000 0x10400000 ${filesize} && echo FINISHED\0" \
				  ""
#define CONFIG_ETHADDR          00:01:02:03:04:05

#define CONFIG_BAUDRATE		115200
#define CFG_BAUDRATE_TABLE	{115200 , 19200, 38400, 57600, 9600 }

#define CFG_PROMPT		"U-Boot> "
#define CFG_CBSIZE		256
#define CFG_MAXARGS		16
#define CFG_PBSIZE		(CFG_CBSIZE + sizeof(CFG_PROMPT) + 16)
#define CFG_LONGHELP		1
#define CONFIG_CMDLINE_EDITING	1

#define ROUND(A, B)		(((A) + (B)) & ~((B) - 1))
/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN		ROUND(3 * CFG_ENV_SIZE + 128*1024, 0x1000)
#define CFG_GBL_DATA_SIZE	128	/* 128 bytes for initial data */

#define CONFIG_STACKSIZE	(32*1024)	/* regular stack */

#ifdef CONFIG_USE_IRQ
#error CONFIG_USE_IRQ not supported
#endif

/*
 * Miscellaneous configurable options
 */
#define CFG_HUSH_PARSER		1
#define CFG_PROMPT_HUSH_PS2	"> "

#define CFG_LONGHELP				/* undef to save memory		*/
#ifdef CFG_HUSH_PARSER
#define CFG_PROMPT		"$ "		/* Monitor Command Prompt */
#else
#define CFG_PROMPT		"=> "		/* Monitor Command Prompt */
#endif

#endif
