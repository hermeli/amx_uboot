/*
 * (C) Copyright 2007-2008
 * Stefan Wyss (swyss@kbr.kaba.com)
 * KABA AG, CC EAC
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

#include <common.h>
#include <asm/arch/at91sam9260.h>
#include <asm/arch/at91sam9260_matrix.h>
#include <asm/arch/at91sam9_smc.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/at91_rstc.h>
#include <asm/arch/gpio.h>
#include <asm/arch/io.h>
#if defined(CONFIG_RESET_PHY_R) && defined(CONFIG_MACB)
#include <net.h>
#include <watchdog.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */
/*
 * Miscelaneous platform dependent initialisations
 */

static void amm_serial_hw_init(void)
{
#ifdef CONFIG_USART0
	at91_set_A_periph(AT91_PIN_PB4, 1);		/* TXD0 */
	at91_set_A_periph(AT91_PIN_PB5, 0);		/* RXD0 */
	at91_sys_write(AT91_PMC_PCER, 1 << AT91_ID_US0);
#endif

#ifdef CONFIG_USART1
	at91_set_A_periph(AT91_PIN_PB6, 1);		/* TXD1 */
	at91_set_A_periph(AT91_PIN_PB7, 0);		/* RXD1 */
	at91_sys_write(AT91_PMC_PCER, 1 << AT91_ID_US1);
#endif

#ifdef CONFIG_USART2
	at91_set_A_periph(AT91_PIN_PB8, 1);		/* TXD2 */
	at91_set_A_periph(AT91_PIN_PB9, 0);		/* RXD2 */
	at91_sys_write(AT91_PMC_PCER, 1 << AT91_ID_US2);
#endif

#ifdef CONFIG_USART3	/* DBGU */
	at91_set_A_periph(AT91_PIN_PB14, 0);		/* DRXD */
	at91_set_A_periph(AT91_PIN_PB15, 1);		/* DTXD */
	at91_sys_write(AT91_PMC_PCER, 1 << AT91_ID_SYS);
#endif
}

#ifdef CONFIG_CMD_NAND
static void amm_nor_hw_init(void)
{
	unsigned long csa;

	
}
#endif

#ifdef CONFIG_MACB
static void amm_macb_hw_init(void)
{
	/* Enable clock */
	at91_sys_write(AT91_PMC_PCER, 1 << AT91SAM9260_ID_EMAC);

	at91_set_A_periph(AT91_PIN_PA19, 0);	/* ETXCK_EREFCK */
	at91_set_A_periph(AT91_PIN_PA17, 0);	/* ERXDV */
	at91_set_A_periph(AT91_PIN_PA14, 0);	/* ERX0 */
	at91_set_A_periph(AT91_PIN_PA15, 0);	/* ERX1 */
	at91_set_A_periph(AT91_PIN_PA18, 0);	/* ERXER */
	at91_set_A_periph(AT91_PIN_PA16, 0);	/* ETXEN */
	at91_set_A_periph(AT91_PIN_PA12, 0);	/* ETX0 */
	at91_set_A_periph(AT91_PIN_PA13, 0);	/* ETX1 */
	at91_set_A_periph(AT91_PIN_PA21, 0);	/* EMDIO */
	at91_set_A_periph(AT91_PIN_PA20, 0);	/* EMDC */
	
	at91_set_B_periph(AT91_PIN_PA10, 0);	
	at91_set_B_periph(AT91_PIN_PA11, 0);	
	at91_set_B_periph(AT91_PIN_PA25, 0);	
	at91_set_B_periph(AT91_PIN_PA26, 0);	
	at91_set_B_periph(AT91_PIN_PA27, 0);	
	at91_set_B_periph(AT91_PIN_PA28, 0);	
	at91_set_B_periph(AT91_PIN_PA29, 0);	

	// disable PIO function for all EMAC pins
	writel(pin_to_mask(AT91_PIN_PA21) |
	       pin_to_mask(AT91_PIN_PA20) |
	       pin_to_mask(AT91_PIN_PA27) |
	       pin_to_mask(AT91_PIN_PA18) |
	       pin_to_mask(AT91_PIN_PA14) |
	       pin_to_mask(AT91_PIN_PA15) |
	       pin_to_mask(AT91_PIN_PA25) |
	       pin_to_mask(AT91_PIN_PA26) |
	       pin_to_mask(AT91_PIN_PA19) |
	       pin_to_mask(AT91_PIN_PA16) |
	       pin_to_mask(AT91_PIN_PA12) |
	       pin_to_mask(AT91_PIN_PA13) |
	       pin_to_mask(AT91_PIN_PA10) |
	       pin_to_mask(AT91_PIN_PA11) |
	       pin_to_mask(AT91_PIN_PA17) |
	       pin_to_mask(AT91_PIN_PA29) |
	       pin_to_mask(AT91_PIN_PA28),
	       pin_to_controller(AT91_PIN_PA0) + PIO_PDR);

}
#endif

int board_init(void)
{
	/* Enable Ctrlc */
	console_init_f();

	/* arch number of AMM-Board (is AT91SAM9260) */
	gd->bd->bi_arch_number = MACH_TYPE_AT91SAM9260EK;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	amm_serial_hw_init();
#ifdef CONFIG_MACB
	amm_macb_hw_init();
#endif
	return 0;
}

int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	return 0;
}

#ifdef CONFIG_RESET_PHY_R
void reset_phy(void)
{
#ifdef CONFIG_MACB
	/*
	 * Initialize ethernet HW addr prior to starting Linux,
	 * needed for nfsroot
	 */
	eth_init(gd->bd);
#endif
}
#endif
