/* Copyright (c) 2015-2017, 2020, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef SDM660_WCD_REGISTERS_H
#define SDM660_WCD_REGISTERS_H

#define CDC_DIG_BASE		0xF000
#define CDC_ANA_BASE		0xF100

#define MSM89XX_PMIC_DIGITAL_REVISION1		(CDC_DIG_BASE+0x000)
#define MSM89XX_PMIC_DIGITAL_REVISION1__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_REVISION2		(CDC_DIG_BASE+0x001)
#define MSM89XX_PMIC_DIGITAL_REVISION2__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_PERPH_TYPE		(CDC_DIG_BASE+0x004)
#define MSM89XX_PMIC_DIGITAL_PERPH_TYPE__POR		(0x23)
#define MSM89XX_PMIC_DIGITAL_PERPH_SUBTYPE		(CDC_DIG_BASE+0x005)
#define MSM89XX_PMIC_DIGITAL_PERPH_SUBTYPE__POR		(0x01)
#define MSM89XX_PMIC_DIGITAL_INT_RT_STS		(CDC_DIG_BASE+0x010)
#define MSM89XX_PMIC_DIGITAL_INT_RT_STS__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_SET_TYPE		(CDC_DIG_BASE+0x011)
#define MSM89XX_PMIC_DIGITAL_INT_SET_TYPE__POR		(0xFF)
#define MSM89XX_PMIC_DIGITAL_INT_POLARITY_HIGH		(CDC_DIG_BASE+0x012)
#define MSM89XX_PMIC_DIGITAL_INT_POLARITY_HIGH__POR		(0xFF)
#define MSM89XX_PMIC_DIGITAL_INT_POLARITY_LOW		(CDC_DIG_BASE+0x013)
#define MSM89XX_PMIC_DIGITAL_INT_POLARITY_LOW__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_LATCHED_CLR		(CDC_DIG_BASE+0x014)
#define MSM89XX_PMIC_DIGITAL_INT_LATCHED_CLR__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_EN_SET		(CDC_DIG_BASE+0x015)
#define MSM89XX_PMIC_DIGITAL_INT_EN_SET__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_EN_CLR		(CDC_DIG_BASE+0x016)
#define MSM89XX_PMIC_DIGITAL_INT_EN_CLR__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_LATCHED_STS		(CDC_DIG_BASE+0x018)
#define MSM89XX_PMIC_DIGITAL_INT_LATCHED_STS__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_PENDING_STS		(CDC_DIG_BASE+0x019)
#define MSM89XX_PMIC_DIGITAL_INT_PENDING_STS__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_MID_SEL		(CDC_DIG_BASE+0x01A)
#define MSM89XX_PMIC_DIGITAL_INT_MID_SEL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_PRIORITY		(CDC_DIG_BASE+0x01B)
#define MSM89XX_PMIC_DIGITAL_INT_PRIORITY__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_GPIO_MODE		(CDC_DIG_BASE+0x040)
#define MSM89XX_PMIC_DIGITAL_GPIO_MODE__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_PIN_CTL_OE		(CDC_DIG_BASE+0x041)
#define MSM89XX_PMIC_DIGITAL_PIN_CTL_OE__POR		(0x01)
#define MSM89XX_PMIC_DIGITAL_PIN_CTL_DATA		(CDC_DIG_BASE+0x042)
#define MSM89XX_PMIC_DIGITAL_PIN_CTL_DATA__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_PIN_STATUS		(CDC_DIG_BASE+0x043)
#define MSM89XX_PMIC_DIGITAL_PIN_STATUS__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_HDRIVE_CTL		(CDC_DIG_BASE+0x044)
#define MSM89XX_PMIC_DIGITAL_HDRIVE_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_RST_CTL		(CDC_DIG_BASE+0x046)
#define MSM89XX_PMIC_DIGITAL_CDC_RST_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_TOP_CLK_CTL		(CDC_DIG_BASE+0x048)
#define MSM89XX_PMIC_DIGITAL_CDC_TOP_CLK_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_ANA_CLK_CTL		(CDC_DIG_BASE+0x049)
#define MSM89XX_PMIC_DIGITAL_CDC_ANA_CLK_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_DIG_CLK_CTL		(CDC_DIG_BASE+0x04A)
#define MSM89XX_PMIC_DIGITAL_CDC_DIG_CLK_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_TX1_CTL		(CDC_DIG_BASE+0x050)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_TX1_CTL__POR		(0x02)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_TX2_CTL		(CDC_DIG_BASE+0x051)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_TX2_CTL__POR		(0x02)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_HPHR_DAC_CTL	(CDC_DIG_BASE+0x052)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_HPHR_DAC_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX1_CTL		(CDC_DIG_BASE+0x053)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX1_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX2_CTL		(CDC_DIG_BASE+0x054)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX2_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX3_CTL		(CDC_DIG_BASE+0x055)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX3_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX_LB_CTL	(CDC_DIG_BASE+0x056)
#define MSM89XX_PMIC_DIGITAL_CDC_CONN_RX_LB_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_CDC_RX_CTL1		(CDC_DIG_BASE+0x058)
#define MSM89XX_PMIC_DIGITAL_CDC_RX_CTL1__POR		(0x7C)
#define MSM89XX_PMIC_DIGITAL_CDC_RX_CTL2		(CDC_DIG_BASE+0x059)
#define MSM89XX_PMIC_DIGITAL_CDC_RX_CTL2__POR		(0x7C)
#define MSM89XX_PMIC_DIGITAL_CDC_RX_CTL3		(CDC_DIG_BASE+0x05A)
#define MSM89XX_PMIC_DIGITAL_CDC_RX_CTL3__POR		(0x7C)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA0		(CDC_DIG_BASE+0x05B)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA0__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA1		(CDC_DIG_BASE+0x05C)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA1__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA2		(CDC_DIG_BASE+0x05D)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA2__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA3		(CDC_DIG_BASE+0x05E)
#define MSM89XX_PMIC_DIGITAL_DEM_BYPASS_DATA3__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_DIG_DEBUG_CTL		(CDC_DIG_BASE+0x068)
#define MSM89XX_PMIC_DIGITAL_DIG_DEBUG_CTL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_DIG_DEBUG_EN		(CDC_DIG_BASE+0x069)
#define MSM89XX_PMIC_DIGITAL_DIG_DEBUG_EN__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_SPARE_0		(CDC_DIG_BASE+0x070)
#define MSM89XX_PMIC_DIGITAL_SPARE_0__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_SPARE_1		(CDC_DIG_BASE+0x071)
#define MSM89XX_PMIC_DIGITAL_SPARE_1__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_SPARE_2		(CDC_DIG_BASE+0x072)
#define MSM89XX_PMIC_DIGITAL_SPARE_2__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_SEC_ACCESS		(CDC_DIG_BASE+0x0D0)
#define MSM89XX_PMIC_DIGITAL_SEC_ACCESS__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL1		(CDC_DIG_BASE+0x0D8)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL1__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL2		(CDC_DIG_BASE+0x0D9)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL2__POR		(0x01)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL3		(CDC_DIG_BASE+0x0DA)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL3__POR		(0x05)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL4		(CDC_DIG_BASE+0x0DB)
#define MSM89XX_PMIC_DIGITAL_PERPH_RESET_CTL4__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_TEST1		(CDC_DIG_BASE+0x0E0)
#define MSM89XX_PMIC_DIGITAL_INT_TEST1__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_INT_TEST_VAL		(CDC_DIG_BASE+0x0E1)
#define MSM89XX_PMIC_DIGITAL_INT_TEST_VAL__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_TRIM_NUM		(CDC_DIG_BASE+0x0F0)
#define MSM89XX_PMIC_DIGITAL_TRIM_NUM__POR		(0x00)
#define MSM89XX_PMIC_DIGITAL_TRIM_CTRL		(CDC_DIG_BASE+0x0F1)
#define MSM89XX_PMIC_DIGITAL_TRIM_CTRL__POR		(0x00)

#define MSM89XX_PMIC_ANALOG_REVISION1		(CDC_ANA_BASE+0x00)
#define MSM89XX_PMIC_ANALOG_REVISION1__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_REVISION2		(CDC_ANA_BASE+0x01)
#define MSM89XX_PMIC_ANALOG_REVISION2__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_REVISION3		(CDC_ANA_BASE+0x02)
#define MSM89XX_PMIC_ANALOG_REVISION3__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_REVISION4		(CDC_ANA_BASE+0x03)
#define MSM89XX_PMIC_ANALOG_REVISION4__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_PERPH_TYPE		(CDC_ANA_BASE+0x04)
#define MSM89XX_PMIC_ANALOG_PERPH_TYPE__POR		(0x23)
#define MSM89XX_PMIC_ANALOG_PERPH_SUBTYPE		(CDC_ANA_BASE+0x05)
#define MSM89XX_PMIC_ANALOG_PERPH_SUBTYPE__POR		(0x09)
#define MSM89XX_PMIC_ANALOG_INT_RT_STS		(CDC_ANA_BASE+0x10)
#define MSM89XX_PMIC_ANALOG_INT_RT_STS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_SET_TYPE		(CDC_ANA_BASE+0x11)
#define MSM89XX_PMIC_ANALOG_INT_SET_TYPE__POR		(0x3F)
#define MSM89XX_PMIC_ANALOG_INT_POLARITY_HIGH		(CDC_ANA_BASE+0x12)
#define MSM89XX_PMIC_ANALOG_INT_POLARITY_HIGH__POR		(0x3F)
#define MSM89XX_PMIC_ANALOG_INT_POLARITY_LOW		(CDC_ANA_BASE+0x13)
#define MSM89XX_PMIC_ANALOG_INT_POLARITY_LOW__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_LATCHED_CLR		(CDC_ANA_BASE+0x14)
#define MSM89XX_PMIC_ANALOG_INT_LATCHED_CLR__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_EN_SET		(CDC_ANA_BASE+0x15)
#define MSM89XX_PMIC_ANALOG_INT_EN_SET__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_EN_CLR		(CDC_ANA_BASE+0x16)
#define MSM89XX_PMIC_ANALOG_INT_EN_CLR__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_LATCHED_STS		(CDC_ANA_BASE+0x18)
#define MSM89XX_PMIC_ANALOG_INT_LATCHED_STS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_PENDING_STS		(CDC_ANA_BASE+0x19)
#define MSM89XX_PMIC_ANALOG_INT_PENDING_STS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_MID_SEL		(CDC_ANA_BASE+0x1A)
#define MSM89XX_PMIC_ANALOG_INT_MID_SEL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_PRIORITY		(CDC_ANA_BASE+0x1B)
#define MSM89XX_PMIC_ANALOG_INT_PRIORITY__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MICB_1_EN		(CDC_ANA_BASE+0x40)
#define MSM89XX_PMIC_ANALOG_MICB_1_EN__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MICB_1_VAL		(CDC_ANA_BASE+0x41)
#define MSM89XX_PMIC_ANALOG_MICB_1_VAL__POR		(0x20)
#define MSM89XX_PMIC_ANALOG_MICB_1_CTL		(CDC_ANA_BASE+0x42)
#define MSM89XX_PMIC_ANALOG_MICB_1_CTL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MICB_1_INT_RBIAS		(CDC_ANA_BASE+0x43)
#define MSM89XX_PMIC_ANALOG_MICB_1_INT_RBIAS__POR		(0x49)
#define MSM89XX_PMIC_ANALOG_MICB_2_EN		(CDC_ANA_BASE+0x44)
#define MSM89XX_PMIC_ANALOG_MICB_2_EN__POR		(0x20)
#define MSM89XX_PMIC_ANALOG_TX_1_2_ATEST_CTL_2		(CDC_ANA_BASE+0x45)
#define MSM89XX_PMIC_ANALOG_TX_1_2_ATEST_CTL_2__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MASTER_BIAS_CTL		(CDC_ANA_BASE+0x46)
#define MSM89XX_PMIC_ANALOG_MASTER_BIAS_CTL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MBHC_DET_CTL_1		(CDC_ANA_BASE+0x47)
#define MSM89XX_PMIC_ANALOG_MBHC_DET_CTL_1__POR		(0x35)
#define MSM89XX_PMIC_ANALOG_MBHC_DET_CTL_2		(CDC_ANA_BASE+0x50)
#define MSM89XX_PMIC_ANALOG_MBHC_DET_CTL_2__POR		(0x08)
#define MSM89XX_PMIC_ANALOG_MBHC_FSM_CTL		(CDC_ANA_BASE+0x51)
#define MSM89XX_PMIC_ANALOG_MBHC_FSM_CTL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MBHC_DBNC_TIMER		(CDC_ANA_BASE+0x52)
#define MSM89XX_PMIC_ANALOG_MBHC_DBNC_TIMER__POR		(0x98)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN0_ZDETL_CTL	(CDC_ANA_BASE+0x53)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN0_ZDETL_CTL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN1_ZDETM_CTL	(CDC_ANA_BASE+0x54)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN1_ZDETM_CTL__POR		(0x20)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN2_ZDETH_CTL	(CDC_ANA_BASE+0x55)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN2_ZDETH_CTL__POR		(0x40)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN3_CTL		(CDC_ANA_BASE+0x56)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN3_CTL__POR		(0x61)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN4_CTL		(CDC_ANA_BASE+0x57)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN4_CTL__POR		(0x80)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN_RESULT		(CDC_ANA_BASE+0x58)
#define MSM89XX_PMIC_ANALOG_MBHC_BTN_RESULT__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_MBHC_ZDET_ELECT_RESULT	(CDC_ANA_BASE+0x59)
#define MSM89XX_PMIC_ANALOG_MBHC_ZDET_ELECT_RESULT__POR	(0x00)
#define MSM89XX_PMIC_ANALOG_TX_1_EN		(CDC_ANA_BASE+0x60)
#define MSM89XX_PMIC_ANALOG_TX_1_EN__POR		(0x03)
#define MSM89XX_PMIC_ANALOG_TX_2_EN		(CDC_ANA_BASE+0x61)
#define MSM89XX_PMIC_ANALOG_TX_2_EN__POR		(0x03)
#define MSM89XX_PMIC_ANALOG_TX_1_2_TEST_CTL_1		(CDC_ANA_BASE+0x62)
#define MSM89XX_PMIC_ANALOG_TX_1_2_TEST_CTL_1__POR		(0xBF)
#define MSM89XX_PMIC_ANALOG_TX_1_2_TEST_CTL_2		(CDC_ANA_BASE+0x63)
#define MSM89XX_PMIC_ANALOG_TX_1_2_TEST_CTL_2__POR		(0x8C)
#define MSM89XX_PMIC_ANALOG_TX_1_2_ATEST_CTL		(CDC_ANA_BASE+0x64)
#define MSM89XX_PMIC_ANALOG_TX_1_2_ATEST_CTL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_TX_1_2_OPAMP_BIAS		(CDC_ANA_BASE+0x65)
#define MSM89XX_PMIC_ANALOG_TX_1_2_OPAMP_BIAS__POR		(0x6B)
#define MSM89XX_PMIC_ANALOG_TX_1_2_TXFE_CLKDIV		(CDC_ANA_BASE+0x66)
#define MSM89XX_PMIC_ANALOG_TX_1_2_TXFE_CLKDIV__POR		(0x51)
#define MSM89XX_PMIC_ANALOG_TX_3_EN		(CDC_ANA_BASE+0x67)
#define MSM89XX_PMIC_ANALOG_TX_3_EN__POR		(0x02)
#define MSM89XX_PMIC_ANALOG_NCP_EN		(CDC_ANA_BASE+0x80)
#define MSM89XX_PMIC_ANALOG_NCP_EN__POR		(0x26)
#define MSM89XX_PMIC_ANALOG_NCP_CLK		(CDC_ANA_BASE+0x81)
#define MSM89XX_PMIC_ANALOG_NCP_CLK__POR		(0x23)
#define MSM89XX_PMIC_ANALOG_NCP_DEGLITCH		(CDC_ANA_BASE+0x82)
#define MSM89XX_PMIC_ANALOG_NCP_DEGLITCH__POR		(0x5B)
#define MSM89XX_PMIC_ANALOG_NCP_FBCTRL		(CDC_ANA_BASE+0x83)
#define MSM89XX_PMIC_ANALOG_NCP_FBCTRL__POR		(0x08)
#define MSM89XX_PMIC_ANALOG_NCP_BIAS		(CDC_ANA_BASE+0x84)
#define MSM89XX_PMIC_ANALOG_NCP_BIAS__POR		(0x29)
#define MSM89XX_PMIC_ANALOG_NCP_VCTRL		(CDC_ANA_BASE+0x85)
#define MSM89XX_PMIC_ANALOG_NCP_VCTRL__POR		(0x24)
#define MSM89XX_PMIC_ANALOG_NCP_TEST		(CDC_ANA_BASE+0x86)
#define MSM89XX_PMIC_ANALOG_NCP_TEST__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_NCP_CLIM_ADDR		(CDC_ANA_BASE+0x87)
#define MSM89XX_PMIC_ANALOG_NCP_CLIM_ADDR__POR		(0xD5)
#define MSM89XX_PMIC_ANALOG_RX_CLOCK_DIVIDER		(CDC_ANA_BASE+0x90)
#define MSM89XX_PMIC_ANALOG_RX_CLOCK_DIVIDER__POR		(0xE8)
#define MSM89XX_PMIC_ANALOG_RX_COM_OCP_CTL		(CDC_ANA_BASE+0x91)
#define MSM89XX_PMIC_ANALOG_RX_COM_OCP_CTL__POR		(0xCF)
#define MSM89XX_PMIC_ANALOG_RX_COM_OCP_COUNT		(CDC_ANA_BASE+0x92)
#define MSM89XX_PMIC_ANALOG_RX_COM_OCP_COUNT__POR		(0x6E)
#define MSM89XX_PMIC_ANALOG_RX_COM_BIAS_DAC		(CDC_ANA_BASE+0x93)
#define MSM89XX_PMIC_ANALOG_RX_COM_BIAS_DAC__POR		(0x18)
#define MSM89XX_PMIC_ANALOG_RX_HPH_BIAS_PA		(CDC_ANA_BASE+0x94)
#define MSM89XX_PMIC_ANALOG_RX_HPH_BIAS_PA__POR		(0x5A)
#define MSM89XX_PMIC_ANALOG_RX_HPH_BIAS_LDO_OCP	(CDC_ANA_BASE+0x95)
#define MSM89XX_PMIC_ANALOG_RX_HPH_BIAS_LDO_OCP__POR		(0x69)
#define MSM89XX_PMIC_ANALOG_RX_HPH_BIAS_CNP		(CDC_ANA_BASE+0x96)
#define MSM89XX_PMIC_ANALOG_RX_HPH_BIAS_CNP__POR		(0x29)
#define MSM89XX_PMIC_ANALOG_RX_HPH_CNP_EN		(CDC_ANA_BASE+0x97)
#define MSM89XX_PMIC_ANALOG_RX_HPH_CNP_EN__POR		(0x80)
#define MSM89XX_PMIC_ANALOG_RX_HPH_CNP_WG_CTL		(CDC_ANA_BASE+0x98)
#define MSM89XX_PMIC_ANALOG_RX_HPH_CNP_WG_CTL__POR		(0xDA)
#define MSM89XX_PMIC_ANALOG_RX_HPH_CNP_WG_TIME		(CDC_ANA_BASE+0x99)
#define MSM89XX_PMIC_ANALOG_RX_HPH_CNP_WG_TIME__POR		(0x16)
#define MSM89XX_PMIC_ANALOG_RX_HPH_L_TEST		(CDC_ANA_BASE+0x9A)
#define MSM89XX_PMIC_ANALOG_RX_HPH_L_TEST__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_RX_HPH_L_PA_DAC_CTL	(CDC_ANA_BASE+0x9B)
#define MSM89XX_PMIC_ANALOG_RX_HPH_L_PA_DAC_CTL__POR		(0x20)
#define MSM89XX_PMIC_ANALOG_RX_HPH_R_TEST		(CDC_ANA_BASE+0x9C)
#define MSM89XX_PMIC_ANALOG_RX_HPH_R_TEST__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_RX_HPH_R_PA_DAC_CTL	(CDC_ANA_BASE+0x9D)
#define MSM89XX_PMIC_ANALOG_RX_HPH_R_PA_DAC_CTL__POR		(0x20)
#define MSM89XX_PMIC_ANALOG_RX_EAR_CTL		(CDC_ANA_BASE+0x9E)
#define MSM89XX_PMIC_ANALOG_RX_EAR_CTL___POR		(0x12)
#define MSM89XX_PMIC_ANALOG_RX_ATEST		(CDC_ANA_BASE+0x9F)
#define MSM89XX_PMIC_ANALOG_RX_ATEST__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_RX_HPH_STATUS		(CDC_ANA_BASE+0xA0)
#define MSM89XX_PMIC_ANALOG_RX_HPH_STATUS__POR		(0x0C)
#define MSM89XX_PMIC_ANALOG_RX_EAR_STATUS		(CDC_ANA_BASE+0xA1)
#define MSM89XX_PMIC_ANALOG_RX_EAR_STATUS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_RX_LO_DAC_CTL		(CDC_ANA_BASE+0xAC)
#define MSM89XX_PMIC_ANALOG_RX_LO_DAC_CTL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_RX_LO_EN_CTL		(CDC_ANA_BASE+0xAD)
#define MSM89XX_PMIC_ANALOG_RX_RX_LO_EN_CTL__POR	(0x00)
#define MSM89XX_PMIC_ANALOG_SPKR_DAC_CTL		(CDC_ANA_BASE+0xB0)
#define MSM89XX_PMIC_ANALOG_SPKR_DAC_CTL__POR		(0x83)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_CLIP_DET		(CDC_ANA_BASE+0xB1)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_CLIP_DET__POR		(0x91)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_CTL		(CDC_ANA_BASE+0xB2)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_CTL__POR		(0x29)
#define MSM89XX_PMIC_ANALOG_SPKR_ANA_BIAS_SET		(CDC_ANA_BASE+0xB3)
#define MSM89XX_PMIC_ANALOG_SPKR_ANA_BIAS_SET__POR		(0x4D)
#define MSM89XX_PMIC_ANALOG_SPKR_OCP_CTL		(CDC_ANA_BASE+0xB4)
#define MSM89XX_PMIC_ANALOG_SPKR_OCP_CTL__POR		(0xE1)
#define MSM89XX_PMIC_ANALOG_SPKR_PWRSTG_CTL		(CDC_ANA_BASE+0xB5)
#define MSM89XX_PMIC_ANALOG_SPKR_PWRSTG_CTL__POR		(0x1E)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_MISC		(CDC_ANA_BASE+0xB6)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_MISC__POR		(0xCB)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_DBG		(CDC_ANA_BASE+0xB7)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_DBG__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_CURRENT_LIMIT		(CDC_ANA_BASE+0xC0)
#define MSM89XX_PMIC_ANALOG_CURRENT_LIMIT__POR		(0x02)
#define MSM89XX_PMIC_ANALOG_OUTPUT_VOLTAGE		(CDC_ANA_BASE+0xC1)
#define MSM89XX_PMIC_ANALOG_OUTPUT_VOLTAGE__POR		(0x14)
#define MSM89XX_PMIC_ANALOG_BYPASS_MODE		(CDC_ANA_BASE+0xC2)
#define MSM89XX_PMIC_ANALOG_BYPASS_MODE__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_BOOST_EN_CTL		(CDC_ANA_BASE+0xC3)
#define MSM89XX_PMIC_ANALOG_BOOST_EN_CTL__POR		(0x1F)
#define MSM89XX_PMIC_ANALOG_SLOPE_COMP_IP_ZERO		(CDC_ANA_BASE+0xC4)
#define MSM89XX_PMIC_ANALOG_SLOPE_COMP_IP_ZERO__POR		(0x8C)
#define MSM89XX_PMIC_ANALOG_RDSON_MAX_DUTY_CYCLE	(CDC_ANA_BASE+0xC5)
#define MSM89XX_PMIC_ANALOG_RDSON_MAX_DUTY_CYCLE__POR		(0xC0)
#define MSM89XX_PMIC_ANALOG_BOOST_TEST1_1		(CDC_ANA_BASE+0xC6)
#define MSM89XX_PMIC_ANALOG_BOOST_TEST1_1__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_BOOST_TEST_2		(CDC_ANA_BASE+0xC7)
#define MSM89XX_PMIC_ANALOG_BOOST_TEST_2__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_SPKR_SAR_STATUS		(CDC_ANA_BASE+0xC8)
#define MSM89XX_PMIC_ANALOG_SPKR_SAR_STATUS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_STATUS		(CDC_ANA_BASE+0xC9)
#define MSM89XX_PMIC_ANALOG_SPKR_DRV_STATUS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_PBUS_ADD_CSR		(CDC_ANA_BASE+0xCE)
#define MSM89XX_PMIC_ANALOG_PBUS_ADD_CSR__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_PBUS_ADD_SEL		(CDC_ANA_BASE+0xCF)
#define MSM89XX_PMIC_ANALOG_PBUS_ADD_SEL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_SEC_ACCESS		(CDC_ANA_BASE+0xD0)
#define MSM89XX_PMIC_ANALOG_SEC_ACCESS__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL1		(CDC_ANA_BASE+0xD8)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL1__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL2		(CDC_ANA_BASE+0xD9)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL2__POR		(0x01)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL3		(CDC_ANA_BASE+0xDA)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL3__POR		(0x05)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL4		(CDC_ANA_BASE+0xDB)
#define MSM89XX_PMIC_ANALOG_PERPH_RESET_CTL4__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_TEST1		(CDC_ANA_BASE+0xE0)
#define MSM89XX_PMIC_ANALOG_INT_TEST1__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_INT_TEST_VAL		(CDC_ANA_BASE+0xE1)
#define MSM89XX_PMIC_ANALOG_INT_TEST_VAL__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_TRIM_NUM		(CDC_ANA_BASE+0xF0)
#define MSM89XX_PMIC_ANALOG_TRIM_NUM__POR		(0x04)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL1		(CDC_ANA_BASE+0xF1)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL1__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL2		(CDC_ANA_BASE+0xF2)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL2__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL3		(CDC_ANA_BASE+0xF3)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL3__POR		(0x00)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL4		(CDC_ANA_BASE+0xF4)
#define MSM89XX_PMIC_ANALOG_TRIM_CTRL4__POR		(0x00)

#define MSM89XX_PMIC_CDC_NUM_REGISTERS \
		(MSM89XX_PMIC_ANALOG_TRIM_CTRL4+1)
#define MSM89XX_PMIC_CDC_MAX_REGISTER \
		(MSM89XX_PMIC_CDC_NUM_REGISTERS-1)
#define MSM89XX_PMIC_CDC_CACHE_SIZE \
		MSM89XX_PMIC_CDC_NUM_REGISTERS


#define MSM89XX_CDC_CORE_CLK_RX_RESET_CTL	(0x00)
#define MSM89XX_CDC_CORE_CLK_RX_RESET_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_TX_RESET_B1_CTL	(0x04)
#define MSM89XX_CDC_CORE_CLK_TX_RESET_B1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_CLK_DMIC_B1_CTL	(0x08)
#define MSM89XX_CDC_CORE_CLK_DMIC_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_RX_I2S_CTL		(0x0C)
#define MSM89XX_CDC_CORE_CLK_RX_I2S_CTL__POR		(0x13)
#define MSM89XX_CDC_CORE_CLK_TX_I2S_CTL		(0x10)
#define MSM89XX_CDC_CORE_CLK_TX_I2S_CTL__POR		(0x13)
#define MSM89XX_CDC_CORE_CLK_OTHR_RESET_B1_CTL	(0x14)
#define MSM89XX_CDC_CORE_CLK_OTHR_RESET_B1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_CLK_TX_CLK_EN_B1_CTL	(0x18)
#define MSM89XX_CDC_CORE_CLK_TX_CLK_EN_B1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_CLK_OTHR_CTL		(0x1C)
#define MSM89XX_CDC_CORE_CLK_OTHR_CTL__POR		(0x04)
#define MSM89XX_CDC_CORE_CLK_RX_B1_CTL		(0x20)
#define MSM89XX_CDC_CORE_CLK_RX_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_MCLK_CTL		(0x24)
#define MSM89XX_CDC_CORE_CLK_MCLK_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_PDM_CTL		(0x28)
#define MSM89XX_CDC_CORE_CLK_PDM_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_SD_CTL		(0x2C)
#define MSM89XX_CDC_CORE_CLK_SD_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_DMIC_B2_CTL	(0x30)
#define MSM89XX_CDC_CORE_CLK_DMIC_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_RX_B2_CTL		(0x34)
#define MSM89XX_CDC_CORE_CLK_RX_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CLK_TX2_I2S_CTL	(0x38)
#define MSM89XX_CDC_CORE_CLK_TX2_I2S_CTL__POR		(0x13)
#define MSM89XX_CDC_CORE_RX1_B1_CTL		(0x40)
#define MSM89XX_CDC_CORE_RX1_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX2_B1_CTL		(0x60)
#define MSM89XX_CDC_CORE_RX2_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX3_B1_CTL		(0x80)
#define MSM89XX_CDC_CORE_RX3_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX1_B2_CTL		(0x44)
#define MSM89XX_CDC_CORE_RX1_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX2_B2_CTL		(0x64)
#define MSM89XX_CDC_CORE_RX2_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX3_B2_CTL		(0x84)
#define MSM89XX_CDC_CORE_RX3_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX1_B3_CTL		(0x48)
#define MSM89XX_CDC_CORE_RX1_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX2_B3_CTL		(0x68)
#define MSM89XX_CDC_CORE_RX2_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX3_B3_CTL		(0x88)
#define MSM89XX_CDC_CORE_RX3_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX1_B4_CTL		(0x4C)
#define MSM89XX_CDC_CORE_RX1_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX2_B4_CTL		(0x6C)
#define MSM89XX_CDC_CORE_RX2_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX3_B4_CTL		(0x8C)
#define MSM89XX_CDC_CORE_RX3_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX1_B5_CTL		(0x50)
#define MSM89XX_CDC_CORE_RX1_B5_CTL__POR		(0x68)
#define MSM89XX_CDC_CORE_RX2_B5_CTL		(0x70)
#define MSM89XX_CDC_CORE_RX2_B5_CTL__POR		(0x68)
#define MSM89XX_CDC_CORE_RX3_B5_CTL		(0x90)
#define MSM89XX_CDC_CORE_RX3_B5_CTL__POR		(0x68)
#define MSM89XX_CDC_CORE_RX1_B6_CTL		(0x54)
#define MSM89XX_CDC_CORE_RX1_B6_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX2_B6_CTL		(0x74)
#define MSM89XX_CDC_CORE_RX2_B6_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX3_B6_CTL		(0x94)
#define MSM89XX_CDC_CORE_RX3_B6_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_RX1_VOL_CTL_B1_CTL	(0x58)
#define MSM89XX_CDC_CORE_RX1_VOL_CTL_B1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_RX2_VOL_CTL_B1_CTL	(0x78)
#define MSM89XX_CDC_CORE_RX2_VOL_CTL_B1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_RX3_VOL_CTL_B1_CTL	(0x98)
#define MSM89XX_CDC_CORE_RX3_VOL_CTL_B1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_RX1_VOL_CTL_B2_CTL	(0x5C)
#define MSM89XX_CDC_CORE_RX1_VOL_CTL_B2_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_RX2_VOL_CTL_B2_CTL	(0x7C)
#define MSM89XX_CDC_CORE_RX2_VOL_CTL_B2_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_RX3_VOL_CTL_B2_CTL	(0x9C)
#define MSM89XX_CDC_CORE_RX3_VOL_CTL_B2_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_TOP_GAIN_UPDATE	(0xA0)
#define MSM89XX_CDC_CORE_TOP_GAIN_UPDATE__POR		(0x00)
#define MSM89XX_CDC_CORE_TOP_CTL		(0xA4)
#define MSM89XX_CDC_CORE_TOP_CTL__POR			(0x01)
#define MSM89XX_CDC_CORE_COMP0_B1_CTL		(0xB0)
#define MSM89XX_CDC_CORE_COMP0_B1_CTL__POR		(0x30)
#define MSM89XX_CDC_CORE_COMP0_B2_CTL		(0xB4)
#define MSM89XX_CDC_CORE_COMP0_B2_CTL__POR		(0xB5)
#define MSM89XX_CDC_CORE_COMP0_B3_CTL		(0xB8)
#define MSM89XX_CDC_CORE_COMP0_B3_CTL__POR		(0x28)
#define MSM89XX_CDC_CORE_COMP0_B4_CTL		(0xBC)
#define MSM89XX_CDC_CORE_COMP0_B4_CTL__POR		(0x37)
#define MSM89XX_CDC_CORE_COMP0_B5_CTL		(0xC0)
#define MSM89XX_CDC_CORE_COMP0_B5_CTL__POR		(0x7F)
#define MSM89XX_CDC_CORE_COMP0_B6_CTL		(0xC4)
#define MSM89XX_CDC_CORE_COMP0_B6_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_COMP0_SHUT_DOWN_STATUS	(0xC8)
#define MSM89XX_CDC_CORE_COMP0_SHUT_DOWN_STATUS__POR	(0x03)
#define MSM89XX_CDC_CORE_COMP0_FS_CFG		(0xCC)
#define MSM89XX_CDC_CORE_COMP0_FS_CFG__POR		(0x03)
#define MSM89XX_CDC_CORE_COMP0_DELAY_BUF_CTL	(0xD0)
#define MSM89XX_CDC_CORE_COMP0_DELAY_BUF_CTL__POR	(0x02)
#define MSM89XX_CDC_CORE_DEBUG_DESER1_CTL	(0xE0)
#define MSM89XX_CDC_CORE_DEBUG_DESER1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_DEBUG_DESER2_CTL	(0xE4)
#define MSM89XX_CDC_CORE_DEBUG_DESER2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_DEBUG_B1_CTL_CFG	(0xE8)
#define MSM89XX_CDC_CORE_DEBUG_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_DEBUG_B2_CTL_CFG	(0xEC)
#define MSM89XX_CDC_CORE_DEBUG_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_DEBUG_B3_CTL_CFG	(0xF0)
#define MSM89XX_CDC_CORE_DEBUG_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B1_CTL	(0x100)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B1_CTL	(0x140)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B2_CTL	(0x104)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B2_CTL	(0x144)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B3_CTL	(0x108)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B3_CTL	(0x148)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B4_CTL	(0x10C)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B4_CTL	(0x14C)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B5_CTL	(0x110)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B5_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B5_CTL	(0x150)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B5_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B6_CTL	(0x114)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B6_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B6_CTL	(0x154)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B6_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B7_CTL	(0x118)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B7_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B7_CTL	(0x158)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B7_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B8_CTL	(0x11C)
#define MSM89XX_CDC_CORE_IIR1_GAIN_B8_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B8_CTL	(0x15C)
#define MSM89XX_CDC_CORE_IIR2_GAIN_B8_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_CTL		(0x120)
#define MSM89XX_CDC_CORE_IIR1_CTL__POR			(0x40)
#define MSM89XX_CDC_CORE_IIR2_CTL		(0x160)
#define MSM89XX_CDC_CORE_IIR2_CTL__POR			(0x40)
#define MSM89XX_CDC_CORE_IIR1_GAIN_TIMER_CTL	(0x124)
#define MSM89XX_CDC_CORE_IIR1_GAIN_TIMER_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_IIR2_GAIN_TIMER_CTL	(0x164)
#define MSM89XX_CDC_CORE_IIR2_GAIN_TIMER_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_IIR1_COEF_B1_CTL	(0x128)
#define MSM89XX_CDC_CORE_IIR1_COEF_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_COEF_B1_CTL	(0x168)
#define MSM89XX_CDC_CORE_IIR2_COEF_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR1_COEF_B2_CTL	(0x12C)
#define MSM89XX_CDC_CORE_IIR1_COEF_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_IIR2_COEF_B2_CTL	(0x16C)
#define MSM89XX_CDC_CORE_IIR2_COEF_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX1_B1_CTL	(0x180)
#define MSM89XX_CDC_CORE_CONN_RX1_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX1_B2_CTL	(0x184)
#define MSM89XX_CDC_CORE_CONN_RX1_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX1_B3_CTL	(0x188)
#define MSM89XX_CDC_CORE_CONN_RX1_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX2_B1_CTL	(0x18C)
#define MSM89XX_CDC_CORE_CONN_RX2_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX2_B2_CTL	(0x190)
#define MSM89XX_CDC_CORE_CONN_RX2_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX2_B3_CTL	(0x194)
#define MSM89XX_CDC_CORE_CONN_RX2_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX3_B1_CTL	(0x198)
#define MSM89XX_CDC_CORE_CONN_RX3_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_RX3_B2_CTL	(0x19C)
#define MSM89XX_CDC_CORE_CONN_RX3_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_TX_B1_CTL		(0x1A0)
#define MSM89XX_CDC_CORE_CONN_TX_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_TX_B2_CTL		(0x1A4)
#define MSM89XX_CDC_CORE_CONN_TX_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ1_B1_CTL	(0x1A8)
#define MSM89XX_CDC_CORE_CONN_EQ1_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ1_B2_CTL	(0x1AC)
#define MSM89XX_CDC_CORE_CONN_EQ1_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ1_B3_CTL	(0x1B0)
#define MSM89XX_CDC_CORE_CONN_EQ1_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ1_B4_CTL	(0x1B4)
#define MSM89XX_CDC_CORE_CONN_EQ1_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ2_B1_CTL	(0x1B8)
#define MSM89XX_CDC_CORE_CONN_EQ2_B1_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ2_B2_CTL	(0x1BC)
#define MSM89XX_CDC_CORE_CONN_EQ2_B2_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ2_B3_CTL	(0x1C0)
#define MSM89XX_CDC_CORE_CONN_EQ2_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_EQ2_B4_CTL	(0x1C4)
#define MSM89XX_CDC_CORE_CONN_EQ2_B4_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_CONN_TX_I2S_SD1_CTL	(0x1C8)
#define MSM89XX_CDC_CORE_CONN_TX_I2S_SD1_CTL__POR	(0x00)
#define MSM89XX_CDC_CORE_CONN_TX_B3_CTL		(0x1CC)
#define MSM89XX_CDC_CORE_CONN_TX_B3_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX5_VOL_CTL_TIMER	(0x1E0)
#define MSM89XX_CDC_CORE_TX5_VOL_CTL_TIMER__POR		(0x00)
#define MSM89XX_CDC_CORE_TX5_VOL_CTL_GAIN	(0x1E4)
#define MSM89XX_CDC_CORE_TX5_VOL_CTL_GAIN__POR		(0x00)
#define MSM89XX_CDC_CORE_TX5_VOL_CTL_CFG	(0x1E8)
#define MSM89XX_CDC_CORE_TX5_VOL_CTL_CFG__POR		(0x00)
#define MSM89XX_CDC_CORE_TX5_MUX_CTL		(0x1EC)
#define MSM89XX_CDC_CORE_TX5_MUX_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX5_CLK_FS_CTL		(0x1F0)
#define MSM89XX_CDC_CORE_TX5_CLK_FS_CTL__POR		(0x03)
#define MSM89XX_CDC_CORE_TX5_DMIC_CTL		(0x1F4)
#define MSM89XX_CDC_CORE_TX5_DMIC_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX1_VOL_CTL_TIMER	(0x280)
#define MSM89XX_CDC_CORE_TX1_VOL_CTL_TIMER__POR		(0x00)
#define MSM89XX_CDC_CORE_TX2_VOL_CTL_TIMER	(0x2A0)
#define MSM89XX_CDC_CORE_TX2_VOL_CTL_TIMER__POR		(0x00)
#define MSM89XX_CDC_CORE_TX3_VOL_CTL_TIMER	(0x2C0)
#define MSM89XX_CDC_CORE_TX3_VOL_CTL_TIMER__POR		(0x00)
#define MSM89XX_CDC_CORE_TX4_VOL_CTL_TIMER	(0x2E0)
#define MSM89XX_CDC_CORE_TX4_VOL_CTL_TIMER__POR		(0x00)
#define MSM89XX_CDC_CORE_TX1_VOL_CTL_GAIN	(0x284)
#define MSM89XX_CDC_CORE_TX1_VOL_CTL_GAIN__POR		(0x00)
#define MSM89XX_CDC_CORE_TX2_VOL_CTL_GAIN	(0x2A4)
#define MSM89XX_CDC_CORE_TX2_VOL_CTL_GAIN__POR		(0x00)
#define MSM89XX_CDC_CORE_TX3_VOL_CTL_GAIN	(0x2C4)
#define MSM89XX_CDC_CORE_TX3_VOL_CTL_GAIN__POR		(0x00)
#define MSM89XX_CDC_CORE_TX4_VOL_CTL_GAIN	(0x2E4)
#define MSM89XX_CDC_CORE_TX4_VOL_CTL_GAIN__POR		(0x00)
#define MSM89XX_CDC_CORE_TX1_VOL_CTL_CFG	(0x288)
#define MSM89XX_CDC_CORE_TX1_VOL_CTL_CFG__POR		(0x00)
#define MSM89XX_CDC_CORE_TX2_VOL_CTL_CFG	(0x2A8)
#define MSM89XX_CDC_CORE_TX2_VOL_CTL_CFG__POR		(0x00)
#define MSM89XX_CDC_CORE_TX3_VOL_CTL_CFG	(0x2C8)
#define MSM89XX_CDC_CORE_TX3_VOL_CTL_CFG__POR		(0x00)
#define MSM89XX_CDC_CORE_TX4_VOL_CTL_CFG	(0x2E8)
#define MSM89XX_CDC_CORE_TX4_VOL_CTL_CFG__POR		(0x00)
#define MSM89XX_CDC_CORE_TX1_MUX_CTL		(0x28C)
#define MSM89XX_CDC_CORE_TX1_MUX_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX2_MUX_CTL		(0x2AC)
#define MSM89XX_CDC_CORE_TX2_MUX_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX3_MUX_CTL		(0x2CC)
#define MSM89XX_CDC_CORE_TX3_MUX_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX4_MUX_CTL		(0x2EC)
#define MSM89XX_CDC_CORE_TX4_MUX_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX1_CLK_FS_CTL		(0x290)
#define MSM89XX_CDC_CORE_TX1_CLK_FS_CTL__POR		(0x03)
#define MSM89XX_CDC_CORE_TX2_CLK_FS_CTL		(0x2B0)
#define MSM89XX_CDC_CORE_TX2_CLK_FS_CTL__POR		(0x03)
#define MSM89XX_CDC_CORE_TX3_CLK_FS_CTL		(0x2D0)
#define MSM89XX_CDC_CORE_TX3_CLK_FS_CTL__POR		(0x03)
#define MSM89XX_CDC_CORE_TX4_CLK_FS_CTL		(0x2F0)
#define MSM89XX_CDC_CORE_TX4_CLK_FS_CTL__POR		(0x03)
#define MSM89XX_CDC_CORE_TX1_DMIC_CTL		(0x294)
#define MSM89XX_CDC_CORE_TX1_DMIC_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX2_DMIC_CTL		(0x2B4)
#define MSM89XX_CDC_CORE_TX2_DMIC_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX3_DMIC_CTL		(0x2D4)
#define MSM89XX_CDC_CORE_TX3_DMIC_CTL__POR		(0x00)
#define MSM89XX_CDC_CORE_TX4_DMIC_CTL		(0x2F4)
#define MSM89XX_CDC_CORE_TX4_DMIC_CTL__POR		(0x00)

#define MSM89XX_CDC_CORE_NUM_REGISTERS \
		(MSM89XX_CDC_CORE_TX4_DMIC_CTL+1)
#define MSM89XX_CDC_CORE_MAX_REGISTER \
		(MSM89XX_CDC_CORE_NUM_REGISTERS-1)
#define MSM89XX_CDC_CORE_CACHE_SIZE \
		MSM89XX_CDC_CORE_NUM_REGISTERS
#endif
