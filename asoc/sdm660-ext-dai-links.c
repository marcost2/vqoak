// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015-2019, 2021, The Linux Foundation. All rights reserved.
 */

#include <linux/of.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include "msm-pcm-routing-v2.h"
#include <asoc/sdm660-common.h>
#include <asoc/sdm660-external.h>
#include <asoc/core.h>
#include <sound/soc/codecs/madera.h>
#include "codecs/wcd9335.h"
#include <linux/pm_qos.h>

#define DEV_NAME_STR_LEN            32
#define __CHIPSET__ "SDM660 "
#define MSM_DAILINK_NAME(name) (__CHIPSET__#name)

#define WCN_CDC_SLIM_RX_CH_MAX 2
#define WCN_CDC_SLIM_TX_CH_MAX 3
#define MSM_LL_QOS_VALUE 300 /* time in us to ensure LPM doesn't go in C3/C4 */

#define CS35L35_MCLK_RATE	12288000
#define CS35L35_SCLK_RATE	1536000

#ifndef CONFIG_SND_SOC_MADERA
static struct snd_soc_card snd_soc_card_msm_card_tavil = {
	.name = "sdm670-tavil-snd-card",
	.late_probe = msm_snd_card_tavil_late_probe,
};

static struct snd_soc_card snd_soc_card_msm_card_tasha = {
	.name = "sdm670-tasha-snd-card",
	.late_probe = msm_snd_card_tasha_late_probe,
};
#endif

#define AMP_CODEC_NAME "cs35l36.0-0040"
#define AMP_DAI_NAME   "cs35l36-pcm"

struct snd_soc_card snd_soc_card_msm_card_madera = {
	.name		= "sdm660-madera-snd-card",
	.codec_conf	= cirrus_amp_codec_conf,
	.num_configs	= ARRAY_SIZE(cirrus_amp_codec_conf),
};

static struct snd_soc_ops msm_ext_slimbus_be_ops = {
	.hw_params = msm_snd_hw_params,
};

static struct snd_soc_ops msm_ext_cpe_ops = {
	.hw_params = msm_snd_cpe_hw_params,
};

static struct snd_soc_ops msm_ext_slimbus_2_be_ops = {
	.hw_params = msm_ext_slimbus_2_hw_params,
};

static struct snd_soc_ops msm_mi2s_be_ops = {
	.startup = msm_mi2s_snd_startup,
	.shutdown = msm_mi2s_snd_shutdown,
};

static struct snd_soc_ops msm_aux_pcm_be_ops = {
	.startup = msm_aux_pcm_snd_startup,
	.shutdown = msm_aux_pcm_snd_shutdown,
};

#ifdef CONFIG_SND_SOC_QCOM_TDM
static struct snd_soc_ops msm_tdm_be_ops = {
	.startup = msm_tdm_snd_startup,
	.shutdown = msm_tdm_snd_shutdown,
	.hw_params = msm_tdm_snd_hw_params,
};
#endif

static int msm_wcn_init(struct snd_soc_pcm_runtime *rtd)
{
	unsigned int rx_ch[WCN_CDC_SLIM_RX_CH_MAX] = {157, 158};
	unsigned int tx_ch[WCN_CDC_SLIM_TX_CH_MAX]  = {159, 160, 161};
	struct snd_soc_dai *codec_dai = rtd->codec_dai;

	return snd_soc_dai_set_channel_map(codec_dai, ARRAY_SIZE(tx_ch),
					   tx_ch, ARRAY_SIZE(rx_ch), rx_ch);
}

static int msm_wcn_hw_params(struct snd_pcm_substream *substream,
			     struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai_link *dai_link = rtd->dai_link;
	u32 rx_ch[WCN_CDC_SLIM_RX_CH_MAX], tx_ch[WCN_CDC_SLIM_TX_CH_MAX];
	u32 rx_ch_cnt = 0, tx_ch_cnt = 0;
	int ret;

	dev_dbg(rtd->dev, "%s: %s_tx_dai_id_%d\n", __func__,
		 codec_dai->name, codec_dai->id);
	ret = snd_soc_dai_get_channel_map(codec_dai,
				 &tx_ch_cnt, tx_ch, &rx_ch_cnt, rx_ch);
	if (ret) {
		dev_err(rtd->dev,
			"%s: failed to get BTFM codec chan map\n, err:%d\n",
			__func__, ret);
		goto exit;
	}

	dev_dbg(rtd->dev, "%s: tx_ch_cnt(%d) id %d\n",
		__func__, tx_ch_cnt, dai_link->id);

	ret = snd_soc_dai_set_channel_map(cpu_dai,
					  tx_ch_cnt, tx_ch, rx_ch_cnt, rx_ch);
	if (ret)
		dev_err(rtd->dev, "%s: failed to set cpu chan map, err:%d\n",
			__func__, ret);

exit:
	return ret;
}

static struct snd_soc_ops msm_wcn_ops = {
	.hw_params = msm_wcn_hw_params,
};

static int msm_fe_qos_prepare(struct snd_pcm_substream *substream)
{
	cpumask_t mask;

	if (pm_qos_request_active(&substream->latency_pm_qos_req))
		pm_qos_remove_request(&substream->latency_pm_qos_req);

	cpumask_clear(&mask);
	cpumask_set_cpu(1, &mask); /* affine to core 1 */
	cpumask_set_cpu(2, &mask); /* affine to core 2 */
	cpumask_copy(&substream->latency_pm_qos_req.cpus_affine, &mask);

	substream->latency_pm_qos_req.type = PM_QOS_REQ_AFFINE_CORES;

	pm_qos_add_request(&substream->latency_pm_qos_req,
			  PM_QOS_CPU_DMA_LATENCY,
			  MSM_LL_QOS_VALUE);
	return 0;
}

static struct snd_soc_ops msm_fe_qos_ops = {
	.prepare = msm_fe_qos_prepare,
};

static struct snd_soc_dai_link msm_ext_madera_fe_dai[] = {
	{
		.name = LPASS_BE_SLIMBUS_4_TX,
		.stream_name = "Slimbus4 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16393",
		.platform_name = "msm-pcm-hostless",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.id = MSM_BACKEND_DAI_SLIMBUS_4_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
	},
	/* Ultrasound RX DAI Link */
	{
		.name = "SLIMBUS_2 Hostless Playback",
		.stream_name = "SLIMBUS_2 Hostless Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16388",
		.platform_name = "msm-pcm-hostless",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.ignore_suspend = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ops = &msm_ext_slimbus_2_be_ops,
	},
	/* Ultrasound TX DAI Link */
	{
		.name = "SLIMBUS_2 Hostless Capture",
		.stream_name = "SLIMBUS_2 Hostless Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16389",
		.platform_name = "msm-pcm-hostless",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.ignore_suspend = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ops = &msm_ext_slimbus_2_be_ops,
	},
	{
		.name = "SLIMBUS_6 Hostless Playback",
		.stream_name = "SLIMBUS_6 Hostless",
		.cpu_dai_name = "SLIMBUS6_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		 /* this dailink has playback support */
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
#ifdef CONFIG_SND_SOC_CS47L90
	/* TBD: cs47l90 CPU-DSP dai */
#else
	{
		.name = "CPU-DSP Voice Control",
		.stream_name = "CPU-DSP Voice Control",
		.cpu_dai_name = "cs47l35-cpu-voicectrl",
		.platform_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-dsp-voicectrl",
		.codec_name = "cs47l35-codec",
		.ignore_suspend = 1,
		.dynamic = 0,
	},
	{
		.name = "CPU-DSP Trace",
		.stream_name = "CPU-DSP Voice Trace",
		.cpu_dai_name = "cs47l35-cpu-trace",
		.platform_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-dsp-trace",
		.codec_name = "cs47l35-codec",
		.ignore_suspend = 1,
		.dynamic = 0,
	},
	{
		.name = "CPU-DSP2 Text",
		.stream_name = "CPU-DSP2 Text",
		.cpu_dai_name = "cs47l35-dsp2-cpu-txt",
		.platform_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-dsp2-txt",
		.codec_name = "cs47l35-codec",
		.ignore_suspend = 1,
		.dynamic = 0,
	},
	{
		.name = "CPU-DSP3 Text",
		.stream_name = "CPU-DSP3 Text",
		.cpu_dai_name = "cs47l35-dsp3-cpu-txt",
		.platform_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-dsp3-txt",
		.codec_name = "cs47l35-codec",
		.ignore_suspend = 1,
		.dynamic = 0,
	},
	{
		.name = "CPU-DSP1 Text",
		.stream_name = "CPU-DSP1 Text",
		.cpu_dai_name = "cs47l35-dsp1-cpu-txt",
		.platform_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-dsp1-txt",
		.codec_name = "cs47l35-codec",
		.ignore_suspend = 1,
		.dynamic = 0,
	}
#endif
};

static struct snd_soc_dai_link msm_ext_tasha_fe_dai[] = {
	/* tasha_vifeedback for speaker protection */
	{
		.name = LPASS_BE_SLIMBUS_4_TX,
		.stream_name = "Slimbus4 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16393",
		.platform_name = "msm-pcm-hostless",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_vifeedback",
		.id = MSM_BACKEND_DAI_SLIMBUS_4_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
	},
	/* Ultrasound RX DAI Link */
	{
		.name = "SLIMBUS_2 Hostless Playback",
		.stream_name = "SLIMBUS_2 Hostless Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16388",
		.platform_name = "msm-pcm-hostless",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_rx2",
		.ignore_suspend = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_pmdown_time = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ops = &msm_ext_slimbus_2_be_ops,
	},
	/* Ultrasound TX DAI Link */
	{
		.name = "SLIMBUS_2 Hostless Capture",
		.stream_name = "SLIMBUS_2 Hostless Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16389",
		.platform_name = "msm-pcm-hostless",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_tx2",
		.ignore_suspend = 1,
		.dpcm_capture = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ops = &msm_ext_slimbus_2_be_ops,
	},
	/* CPE LSM direct dai-link */
	{
		.name = "CPE Listen service",
		.stream_name = "CPE Listen Audio Service",
		.cpu_dai_name = "msm-dai-slim",
		.platform_name = "msm-cpe-lsm",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.dpcm_capture = 1,
		.codec_dai_name = "tasha_mad1",
		.codec_name = "tasha_codec",
		.ops = &msm_ext_cpe_ops,
	},
	{
		.name = "SLIMBUS_6 Hostless Playback",
		.stream_name = "SLIMBUS_6 Hostless",
		.cpu_dai_name = "SLIMBUS6_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		 /* this dailink has playback support */
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	/* CPE LSM EC PP direct dai-link */
	{
		.name = "CPE Listen service ECPP",
		.stream_name = "CPE Listen Audio Service ECPP",
		.cpu_dai_name = "CPE_LSM_NOHOST",
		.platform_name = "msm-cpe-lsm.3",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.codec_dai_name = "tasha_cpe",
		.codec_name = "tasha_codec",
	},
};

static struct snd_soc_dai_link msm_ext_tavil_fe_dai[] = {
	{
		.name = LPASS_BE_SLIMBUS_4_TX,
		.stream_name = "Slimbus4 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16393",
		.platform_name = "msm-pcm-hostless",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_vifeedback",
		.id = MSM_BACKEND_DAI_SLIMBUS_4_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
	},
	/* Ultrasound RX DAI Link */
	{
		.name = "SLIMBUS_2 Hostless Playback",
		.stream_name = "SLIMBUS_2 Hostless Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16388",
		.platform_name = "msm-pcm-hostless",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx2",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ops = &msm_ext_slimbus_2_be_ops,
	},
	/* Ultrasound TX DAI Link */
	{
		.name = "SLIMBUS_2 Hostless Capture",
		.stream_name = "SLIMBUS_2 Hostless Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16389",
		.platform_name = "msm-pcm-hostless",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_tx2",
		.ignore_suspend = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ops = &msm_ext_slimbus_2_be_ops,
	},
};

static const struct snd_soc_pcm_stream cs35l35_params = {
	.formats = SNDRV_PCM_FORMAT_S16_LE,
	.rate_min = 48000,
	.rate_max = 48000,
	.channels_min = 1,
	.channels_max = 2,
};

static const struct snd_soc_pcm_stream cs35l36_params[] = {
	{
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
		.rate_min = 48000,
		.rate_max = 48000,
		.channels_min = 2,
		.channels_max = 2,  /* 2 channels for 1.536MHz SCLK */
	},
	{
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
		.rate_min = 96000,
		.rate_max = 96000,
		.channels_min = 2,
		.channels_max = 2, /* 2 channels for 3.072MHz SCLK */
	},
};

static int cs35l35_dai_init(struct snd_soc_pcm_runtime *rtd)
{
	int ret;
	int codec_clock = CS35L35_MCLK_RATE;
	struct snd_soc_component *component = rtd->codec_dai->component;
	struct snd_soc_dapm_context *dapm = snd_soc_component_get_dapm(component);
	struct snd_soc_dai *aif1_dai = rtd->cpu_dai;
	struct snd_soc_dai *cs35l35_dai = rtd->codec_dai;

	ret = snd_soc_dai_set_sysclk(aif1_dai, MADERA_CLK_SYSCLK_3, 0, 0);
	if (ret != 0) {
		dev_err(cs35l35_dai->dev, "Failed to set SYSCLK %d\n", ret);
		return ret;
	}

	ret = snd_soc_dai_set_sysclk(cs35l35_dai, 0, CS35L35_SCLK_RATE, 0);
	if (ret != 0) {
		dev_err(cs35l35_dai->dev, "Failed to set SCLK %d\n", ret);
		return ret;
	}

#ifdef CONFIG_SND_SOC_CS35L36
	codec_clock = CS35L35_SCLK_RATE;
#endif
	ret = snd_soc_component_set_sysclk(component, 0, 0, codec_clock, 0);
	if (ret != 0) {
		dev_err(cs35l35_dai->dev, "Failed to set MCLK %d\n", ret);
		return ret;
	}
	snd_soc_dapm_ignore_suspend(dapm, "AMP Playback");
#ifdef CONFIG_SND_SOC_CS35L36
	snd_soc_dapm_ignore_suspend(dapm, "SDIN");
	snd_soc_dapm_ignore_suspend(dapm, "SPK");
	snd_soc_dapm_ignore_suspend(dapm, "VP");
	snd_soc_dapm_ignore_suspend(dapm, "AMP Enable");
	snd_soc_dapm_ignore_suspend(dapm, "VSENSE");
	snd_soc_dapm_ignore_suspend(dapm, "Main AMP");
	snd_soc_dapm_sync(dapm);
#endif
	return 0;
}

#ifndef CONFIG_SND_SOC_CS35L36
static const struct snd_soc_pcm_stream cs35l35_pdm_params = {
	.formats = SNDRV_PCM_FMTBIT_S16_LE,
	.rate_min = 96000,
	.rate_max = 96000,
	.channels_min = 1,
	.channels_max = 2,
};
#endif

static struct snd_soc_dai_link msm_ext_madera_be_dai[] = {
	/* Backend DAI Links */
	{
		.name = LPASS_BE_SLIMBUS_0_RX,
		.stream_name = "Slimbus Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16384",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_0_RX,
		.init = &msm_madera_init,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_ext_slimbus_be_ops,
	},
	{
		.name = LPASS_BE_SLIMBUS_0_TX,
		.stream_name = "Slimbus Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16385",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_0_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_ext_slimbus_be_ops,
	},
	{
		.name = LPASS_BE_SLIMBUS_1_RX,
		.stream_name = "Slimbus1 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16386",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim2",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim2",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_1_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_1_TX,
		.stream_name = "Slimbus1 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16387",
		.platform_name = "msm-pcm-hostless",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim2",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim2",
#endif
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_1_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_2_RX,
		.stream_name = "Slimbus2 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16388",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_2_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},

	{
		.name = LPASS_BE_SLIMBUS_3_RX,
		.stream_name = "Slimbus3 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16390",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim3",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_3_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_3_TX,
		.stream_name = "Slimbus3 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16391",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim3",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.no_pcm = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_3_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_4_RX,
		.stream_name = "Slimbus4 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16392",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim1",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim1",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_4_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_5_RX,
		.stream_name = "Slimbus5 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16394",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim2",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim2",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_5_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_6_RX,
		.stream_name = "Slimbus6 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16396",
		.platform_name = "msm-pcm-routing",
#ifdef CONFIG_SND_SOC_CS47L90
		.codec_name = "cs47l90-codec",
		.codec_dai_name = "cs47l90-slim2",
#else
		.codec_name = "cs47l35-codec",
		.codec_dai_name = "cs47l35-slim2",
#endif
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_6_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
#if defined(CONFIG_SND_SOC_CS47L35) && defined(CONFIG_SND_SOC_CS35L35)
	{ /* codec to amp link */
		.name = "MADERA-AMP",
		.stream_name = "MADERA-AMP Playback",
		.cpu_name = "cs47l35-codec",
		.cpu_dai_name = "cs47l35-aif1",
		.codec_name = "cs35l35.2-0040",
		.codec_dai_name = "cs35l35-pcm",
		.init = cs35l35_dai_init,
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.no_pcm = 1,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.params = &cs35l35_params,
	}
#elif defined(CONFIG_SND_SOC_CS47L35) && defined(CONFIG_SND_SOC_CS35L36)
	{ /* codec to amp link */
		.name = "MADERA-AMP",
		.stream_name = "MADERA-AMP Playback",
		.cpu_name = "cs47l35-codec",
		.cpu_dai_name = "cs47l35-aif1",
		.codec_name = "cs35l36.2-0040",
		.codec_dai_name = "cs35l36-pcm",
		.init = cs35l35_dai_init,
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.no_pcm = 1,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.params = &cs35l36_params[0],
		.num_params = ARRAY_SIZE(cs35l36_params),
	},

#else
	{ /* codec to amp link */
		.name = "MADERA-AMP",
		.stream_name = "MADERA-AMP Playback",
		.cpu_name = "cs47l90-codec",
		.cpu_dai_name = "cs47l90-aif1",
		.codec_name = AMP_CODEC_NAME,
		.codec_dai_name = AMP_DAI_NAME,
		.init = cs35l35_dai_init,
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.no_pcm = 1,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.params = &cs35l36_params[0],
		.num_params = ARRAY_SIZE(cs35l36_params),
	}
#endif
};

static struct snd_soc_dai_link msm_ext_tasha_be_dai[] = {
	/* Backend DAI Links */
	{
		.name = LPASS_BE_SLIMBUS_0_RX,
		.stream_name = "Slimbus Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16384",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_mix_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_0_RX,
		.init = &msm_audrx_init,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_ext_slimbus_be_ops,
	},
	{
		.name = LPASS_BE_SLIMBUS_0_TX,
		.stream_name = "Slimbus Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16385",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_tx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_0_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_ext_slimbus_be_ops,
	},
	{
		.name = LPASS_BE_SLIMBUS_1_RX,
		.stream_name = "Slimbus1 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16386",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_mix_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_1_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_1_TX,
		.stream_name = "Slimbus1 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16387",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_tx3",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_1_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_3_RX,
		.stream_name = "Slimbus3 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16390",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_mix_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_3_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_3_TX,
		.stream_name = "Slimbus3 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16391",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_tx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_3_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_4_RX,
		.stream_name = "Slimbus4 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16392",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_mix_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_4_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_5_RX,
		.stream_name = "Slimbus5 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16394",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_rx3",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_5_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	/* MAD BE */
	{
		.name = LPASS_BE_SLIMBUS_5_TX,
		.stream_name = "Slimbus5 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16395",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_mad1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_5_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_6_RX,
		.stream_name = "Slimbus6 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16396",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tasha_codec",
		.codec_dai_name = "tasha_rx4",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_6_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
};

static struct snd_soc_dai_link msm_ext_tavil_be_dai[] = {
	{
		.name = LPASS_BE_SLIMBUS_0_RX,
		.stream_name = "Slimbus Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16384",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_0_RX,
		.init = &msm_audrx_init,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_ext_slimbus_be_ops,
	},
	{
		.name = LPASS_BE_SLIMBUS_0_TX,
		.stream_name = "Slimbus Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16385",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_tx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_0_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_ext_slimbus_be_ops,
	},
	{
		.name = LPASS_BE_SLIMBUS_1_RX,
		.stream_name = "Slimbus1 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16386",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_1_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_1_TX,
		.stream_name = "Slimbus1 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16387",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_tx3",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_1_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_2_RX,
		.stream_name = "Slimbus2 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16388",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx2",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_2_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_3_RX,
		.stream_name = "Slimbus3 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16390",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_3_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_3_TX,
		.stream_name = "Slimbus3 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16391",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_tx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_3_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_4_RX,
		.stream_name = "Slimbus4 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16392",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_4_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_5_RX,
		.stream_name = "Slimbus5 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16394",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx3",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_5_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	/* MAD BE */
	{
		.name = LPASS_BE_SLIMBUS_5_TX,
		.stream_name = "Slimbus5 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16395",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_mad1",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_5_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_6_RX,
		.stream_name = "Slimbus6 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16396",
		.platform_name = "msm-pcm-routing",
		.codec_name = "tavil_codec",
		.codec_dai_name = "tavil_rx4",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_6_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_ext_slimbus_be_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
};

static struct snd_soc_dai_link msm_ext_common_fe_dai[] = {
	/* FrontEnd DAI Links */
	{/* hw:x,0 */
		.name = MSM_DAILINK_NAME(Media1),
		.stream_name = "MultiMedia1",
		.cpu_dai_name	= "MultiMedia1",
		.platform_name  = "msm-pcm-dsp.0",
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_MULTIMEDIA1
	},
	{/* hw:x,1 */
		.name = MSM_DAILINK_NAME(Media2),
		.stream_name = "MultiMedia2",
		.cpu_dai_name   = "MultiMedia2",
		.platform_name  = "msm-pcm-dsp.0",
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_MULTIMEDIA2,
	},
	{/* hw:x,2 */
		.name = "VoiceMMode1",
		.stream_name = "VoiceMMode1",
		.cpu_dai_name = "VoiceMMode1",
		.platform_name = "msm-pcm-voice",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_VOICEMMODE1,
	},
	{/* hw:x,3 */
		.name = "MSM VoIP",
		.stream_name = "VoIP",
		.cpu_dai_name	= "VoIP",
		.platform_name  = "msm-voip-dsp",
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_VOIP,
	},
	{/* hw:x,4 */
		.name = MSM_DAILINK_NAME(ULL),
		.stream_name = "ULL",
		.cpu_dai_name	= "MultiMedia3",
		.platform_name  = "msm-pcm-dsp.2",
		.dynamic = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_MULTIMEDIA3,
	},
	/* Hostless PCM purpose */
	{/* hw:x,5 */
		.name = "SLIMBUS_0 Hostless",
		.stream_name = "SLIMBUS_0 Hostless",
		.cpu_dai_name = "SLIMBUS0_HOSTLESS",
		.platform_name	= "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* This dai link has MI2S support */
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,6 */
		.name = "MSM AFE-PCM RX",
		.stream_name = "AFE-PROXY RX",
		.cpu_dai_name = "msm-dai-q6-dev.241",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.platform_name  = "msm-pcm-afe",
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
	},
	{/* hw:x,7 */
		.name = "MSM AFE-PCM TX",
		.stream_name = "AFE-PROXY TX",
		.cpu_dai_name = "msm-dai-q6-dev.240",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.platform_name  = "msm-pcm-afe",
		.ignore_suspend = 1,
	},
	{/* hw:x,8 */
		.name = MSM_DAILINK_NAME(Compress1),
		.stream_name = "Compress1",
		.cpu_dai_name	= "MultiMedia4",
		.platform_name  = "msm-compress-dsp",
		.async_ops = ASYNC_DPCM_SND_SOC_HW_PARAMS,
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA4,
	},
	{/* hw:x,9*/
		.name = "AUXPCM Hostless",
		.stream_name = "AUXPCM Hostless",
		.cpu_dai_name   = "AUXPCM_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,10 */
		.name = "SLIMBUS_1 Hostless",
		.stream_name = "SLIMBUS_1 Hostless",
		.cpu_dai_name = "SLIMBUS1_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* dai link has playback support */
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,11 */
		.name = "SLIMBUS_3 Hostless",
		.stream_name = "SLIMBUS_3 Hostless",
		.cpu_dai_name = "SLIMBUS3_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* dai link has playback support */
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,12 */
		.name = "SLIMBUS_4 Hostless",
		.stream_name = "SLIMBUS_4 Hostless",
		.cpu_dai_name = "SLIMBUS4_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* dai link has playback support */
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,13 */
		.name = MSM_DAILINK_NAME(LowLatency),
		.stream_name = "MultiMedia5",
		.cpu_dai_name   = "MultiMedia5",
		.platform_name  = "msm-pcm-dsp.1",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_MULTIMEDIA5,
		.ops = &msm_fe_qos_ops,
	},
	/* LSM FE */
	{/* hw:x,14 */
		.name = "Listen 1 Audio Service",
		.stream_name = "Listen 1 Audio Service",
		.cpu_dai_name = "LSM1",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM1,
	},
	{/* hw:x,15 */
		.name = MSM_DAILINK_NAME(Compress2),
		.stream_name = "Compress2",
		.cpu_dai_name   = "MultiMedia7",
		.platform_name  = "msm-compress-dsp",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_MULTIMEDIA7,
	},
	{/* hw:x,16 */
		.name = MSM_DAILINK_NAME(MultiMedia10),
		.stream_name = "MultiMedia10",
		.cpu_dai_name	= "MultiMedia10",
		.platform_name  = "msm-pcm-dsp.1",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA10,
	},
	{/* hw:x,17 */
		.name = MSM_DAILINK_NAME(ULL_NOIRQ),
		.stream_name = "MM_NOIRQ",
		.cpu_dai_name	= "MultiMedia8",
		.platform_name  = "msm-pcm-dsp-noirq",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA8,
		.ops = &msm_fe_qos_ops,
	},
	{/* hw:x,18 */
		.name = "HDMI_RX_HOSTLESS",
		.stream_name = "HDMI_RX_HOSTLESS",
		.cpu_dai_name	= "HDMI_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,19 */
		.name = "VoiceMMode2",
		.stream_name = "VoiceMMode2",
		.cpu_dai_name = "VoiceMMode2",
		.platform_name = "msm-pcm-voice",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_VOICEMMODE2,
	},
	{/* hw:x,20 */
		.name = "Listen 2 Audio Service",
		.stream_name = "Listen 2 Audio Service",
		.cpu_dai_name = "LSM2",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM2,
	},
	{/* hw:x,21 */
		.name = "Listen 3 Audio Service",
		.stream_name = "Listen 3 Audio Service",
		.cpu_dai_name = "LSM3",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM3,
	},
	{/* hw:x,22 */
		.name = "Listen 4 Audio Service",
		.stream_name = "Listen 4 Audio Service",
		.cpu_dai_name = "LSM4",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM4,
	},
	{/* hw:x,23 */
		.name = "Listen 5 Audio Service",
		.stream_name = "Listen 5 Audio Service",
		.cpu_dai_name = "LSM5",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM5,
	},
	{/* hw:x,24 */
		.name = "Listen 6 Audio Service",
		.stream_name = "Listen 6 Audio Service",
		.cpu_dai_name = "LSM6",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM6
	},
	{/* hw:x,25 */
		.name = "Listen 7 Audio Service",
		.stream_name = "Listen 7 Audio Service",
		.cpu_dai_name = "LSM7",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM7,
	},
	{/* hw:x,26 */
		.name = "Listen 8 Audio Service",
		.stream_name = "Listen 8 Audio Service",
		.cpu_dai_name = "LSM8",
		.platform_name = "msm-lsm-client",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST },
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.id = MSM_FRONTEND_DAI_LSM8,
	},
	{/* hw:x,27 */
		.name = MSM_DAILINK_NAME(Media9),
		.stream_name = "MultiMedia9",
		.cpu_dai_name	= "MultiMedia9",
		.platform_name  = "msm-pcm-dsp.0",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA9,
	},
	{/* hw:x,28 */
		.name = MSM_DAILINK_NAME(Compress4),
		.stream_name = "Compress4",
		.cpu_dai_name	= "MultiMedia11",
		.platform_name  = "msm-compress-dsp",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA11,
	},
	{/* hw:x,29 */
		.name = MSM_DAILINK_NAME(Compress5),
		.stream_name = "Compress5",
		.cpu_dai_name	= "MultiMedia12",
		.platform_name  = "msm-compress-dsp",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA12,
	},
	{/* hw:x,30 */
		.name = MSM_DAILINK_NAME(Compress6),
		.stream_name = "Compress6",
		.cpu_dai_name	= "MultiMedia13",
		.platform_name  = "msm-compress-dsp",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA13,
	},
	{/* hw:x,31 */
		.name = MSM_DAILINK_NAME(Compress7),
		.stream_name = "Compress7",
		.cpu_dai_name	= "MultiMedia14",
		.platform_name  = "msm-compress-dsp",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA14,
	},
	{/* hw:x,32 */
		.name = MSM_DAILINK_NAME(Compress8),
		.stream_name = "Compress8",
		.cpu_dai_name	= "MultiMedia15",
		.platform_name  = "msm-compress-dsp",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA15,
	},
	{/* hw:x,33 */
		.name = MSM_DAILINK_NAME(ULL_NOIRQ_2),
		.stream_name = "MM_NOIRQ_2",
		.cpu_dai_name	= "MultiMedia16",
		.platform_name  = "msm-pcm-dsp-noirq",
		.dynamic = 1,
		.dpcm_capture = 1,
		.dpcm_playback = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			 SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		 /* this dai link has playback support */
		.id = MSM_FRONTEND_DAI_MULTIMEDIA16,
	},
	{/* hw:x,34 */
		.name = "SLIMBUS_8 Hostless",
		.stream_name = "SLIMBUS8_HOSTLESS Capture",
		.cpu_dai_name = "SLIMBUS8_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,35 */
		.name = "SLIMBUS7 Hostless",
		.stream_name = "SLIMBUS7 Hostless",
		.cpu_dai_name = "SLIMBUS7_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{/* hw:x,36 */
		.name = "SDM660 HFP TX",
		.stream_name = "MultiMedia6",
		.cpu_dai_name = "MultiMedia6",
		.platform_name  = "msm-pcm-loopback",
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_pmdown_time = 1,
		.id = MSM_FRONTEND_DAI_MULTIMEDIA6,
	},
};

static struct snd_soc_dai_link msm_ext_common_be_dai[] = {
	{
		.name = LPASS_BE_AFE_PCM_RX,
		.stream_name = "AFE Playback",
		.cpu_dai_name = "msm-dai-q6-dev.224",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_AFE_PCM_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		/* this dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_AFE_PCM_TX,
		.stream_name = "AFE Capture",
		.cpu_dai_name = "msm-dai-q6-dev.225",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_AFE_PCM_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
	},
	/* Incall Record Uplink BACK END DAI Link */
	{
		.name = LPASS_BE_INCALL_RECORD_TX,
		.stream_name = "Voice Uplink Capture",
		.cpu_dai_name = "msm-dai-q6-dev.32772",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_INCALL_RECORD_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
	},
	/* Incall Record Downlink BACK END DAI Link */
	{
		.name = LPASS_BE_INCALL_RECORD_RX,
		.stream_name = "Voice Downlink Capture",
		.cpu_dai_name = "msm-dai-q6-dev.32771",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_INCALL_RECORD_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
	},
	/* Incall Music BACK END DAI Link */
	{
		.name = LPASS_BE_VOICE_PLAYBACK_TX,
		.stream_name = "Voice Farend Playback",
		.cpu_dai_name = "msm-dai-q6-dev.32773",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_VOICE_PLAYBACK_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	/* Incall Music 2 BACK END DAI Link */
	{
		.name = LPASS_BE_VOICE2_PLAYBACK_TX,
		.stream_name = "Voice2 Farend Playback",
		.cpu_dai_name = "msm-dai-q6-dev.32770",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_VOICE2_PLAYBACK_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	/* Proxy Tx BACK END DAI Link */
	{
		.name = LPASS_BE_PROXY_TX,
		.stream_name = "Proxy Capture",
		.cpu_dai_name = "msm-dai-q6-dev.8195",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_PROXY_TX,
		.ignore_suspend = 1,
	},
	/* Proxy Rx BACK END DAI Link */
	{
		.name = LPASS_BE_PROXY_RX,
		.stream_name = "Proxy Playback",
		.cpu_dai_name = "msm-dai-q6-dev.8194",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_PROXY_RX,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_USB_AUDIO_RX,
		.stream_name = "USB Audio Playback",
		.cpu_dai_name = "msm-dai-q6-dev.28672",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_USB_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_USB_AUDIO_TX,
		.stream_name = "USB Audio Capture",
		.cpu_dai_name = "msm-dai-q6-dev.28673",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_USB_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
	},
#ifdef CONFIG_SND_SOC_QCOM_TDM
	{
		.name = LPASS_BE_PRI_TDM_RX_0,
		.stream_name = "Primary TDM0 Playback",
		.cpu_dai_name = "msm-dai-q6-tdm.36864",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_PRI_TDM_RX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_PRI_TDM_TX_0,
		.stream_name = "Primary TDM0 Capture",
		.cpu_dai_name = "msm-dai-q6-tdm.36865",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_PRI_TDM_TX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SEC_TDM_RX_0,
		.stream_name = "Secondary TDM0 Playback",
		.cpu_dai_name = "msm-dai-q6-tdm.36880",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SEC_TDM_RX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_SEC_TDM_TX_0,
		.stream_name = "Secondary TDM0 Capture",
		.cpu_dai_name = "msm-dai-q6-tdm.36881",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SEC_TDM_TX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_TERT_TDM_RX_0,
		.stream_name = "Tertiary TDM0 Playback",
		.cpu_dai_name = "msm-dai-q6-tdm.36896",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_TERT_TDM_RX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_TERT_TDM_TX_0,
		.stream_name = "Tertiary TDM0 Capture",
		.cpu_dai_name = "msm-dai-q6-tdm.36897",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_TERT_TDM_TX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_QUAT_TDM_RX_0,
		.stream_name = "Quaternary TDM0 Playback",
		.cpu_dai_name = "msm-dai-q6-tdm.36912",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_QUAT_TDM_RX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_QUAT_TDM_TX_0,
		.stream_name = "Quaternary TDM0 Capture",
		.cpu_dai_name = "msm-dai-q6-tdm.36913",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_QUAT_TDM_TX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_QUIN_TDM_RX_0,
		.stream_name = "Quinary TDM0 Playback",
		.cpu_dai_name = "msm-dai-q6-tdm.36928",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_QUIN_TDM_RX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_QUIN_TDM_TX_0,
		.stream_name = "Quinary TDM0 Capture",
		.cpu_dai_name = "msm-dai-q6-tdm.36929",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_QUIN_TDM_TX_0,
		.be_hw_params_fixup = msm_tdm_be_hw_params_fixup,
		.ops = &msm_tdm_be_ops,
		.ignore_suspend = 1,
	},
#endif
};

static struct snd_soc_dai_link msm_mi2s_be_dai_links[] = {
	{
		.name = LPASS_BE_PRI_MI2S_RX,
		.stream_name = "Primary MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s.0",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_PRI_MI2S_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_PRI_MI2S_TX,
		.stream_name = "Primary MI2S Capture",
		.cpu_dai_name = "msm-dai-q6-mi2s.0",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_PRI_MI2S_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SEC_MI2S_RX,
		.stream_name = "Secondary MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s.1",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SECONDARY_MI2S_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_SEC_MI2S_TX,
		.stream_name = "Secondary MI2S Capture",
		.cpu_dai_name = "msm-dai-q6-mi2s.1",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SECONDARY_MI2S_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_TERT_MI2S_RX,
		.stream_name = "Tertiary MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s.2",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_TERTIARY_MI2S_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_TERT_MI2S_TX,
		.stream_name = "Tertiary MI2S Capture",
		.cpu_dai_name = "msm-dai-q6-mi2s.2",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_TERTIARY_MI2S_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_QUAT_MI2S_RX,
		.stream_name = "Quaternary MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s.3",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_QUATERNARY_MI2S_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_QUAT_MI2S_TX,
		.stream_name = "Quaternary MI2S Capture",
		.cpu_dai_name = "msm-dai-q6-mi2s.3",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_QUATERNARY_MI2S_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_QUIN_MI2S_RX,
		.stream_name = "Quinary MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s.4",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_QUINARY_MI2S_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
	{
		.name = LPASS_BE_QUIN_MI2S_TX,
		.stream_name = "Quinary MI2S Capture",
		.cpu_dai_name = "msm-dai-q6-mi2s.4",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_QUINARY_MI2S_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
	},
};

static struct snd_soc_dai_link msm_hdmi_dba_dai_links[] = {
	{
		.name = LPASS_BE_TERT_MI2S_RX,
		.stream_name = "Tertiary MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s.2",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-ext-disp-audio-codec-rx",
		.codec_dai_name = "msm_hdmi_audio_codec_rx_dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.be_id = MSM_BACKEND_DAI_TERTIARY_MI2S_RX,
		.be_hw_params_fixup = msm_tert_mi2s_params_fixup,
		.ops = &msm_mi2s_be_ops,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1,
	},
};

static struct snd_soc_dai_link msm_auxpcm_be_dai_links[] = {
	/* Primary AUX PCM Backend DAI Links */
	{
		.name = LPASS_BE_AUXPCM_RX,
		.stream_name = "AUX PCM Playback",
		.cpu_dai_name = "msm-dai-q6-auxpcm.1",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_AUXPCM_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	{
		.name = LPASS_BE_AUXPCM_TX,
		.stream_name = "AUX PCM Capture",
		.cpu_dai_name = "msm-dai-q6-auxpcm.1",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_AUXPCM_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	/* Secondary AUX PCM Backend DAI Links */
	{
		.name = LPASS_BE_SEC_AUXPCM_RX,
		.stream_name = "Sec AUX PCM Playback",
		.cpu_dai_name = "msm-dai-q6-auxpcm.2",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SEC_AUXPCM_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	{
		.name = LPASS_BE_SEC_AUXPCM_TX,
		.stream_name = "Sec AUX PCM Capture",
		.cpu_dai_name = "msm-dai-q6-auxpcm.2",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SEC_AUXPCM_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	/* Tertiary AUX PCM Backend DAI Links */
	{
		.name = LPASS_BE_TERT_AUXPCM_RX,
		.stream_name = "Tert AUX PCM Playback",
		.cpu_dai_name = "msm-dai-q6-auxpcm.3",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_TERT_AUXPCM_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	{
		.name = LPASS_BE_TERT_AUXPCM_TX,
		.stream_name = "Tert AUX PCM Capture",
		.cpu_dai_name = "msm-dai-q6-auxpcm.3",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_TERT_AUXPCM_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	/* Quaternary AUX PCM Backend DAI Links */
	{
		.name = LPASS_BE_QUAT_AUXPCM_RX,
		.stream_name = "Quat AUX PCM Playback",
		.cpu_dai_name = "msm-dai-q6-auxpcm.4",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_QUAT_AUXPCM_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	{
		.name = LPASS_BE_QUAT_AUXPCM_TX,
		.stream_name = "Quat AUX PCM Capture",
		.cpu_dai_name = "msm-dai-q6-auxpcm.4",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_QUAT_AUXPCM_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	/* Quinary AUX PCM Backend DAI Links */
	{
		.name = LPASS_BE_QUIN_AUXPCM_RX,
		.stream_name = "Quin AUX PCM Playback",
		.cpu_dai_name = "msm-dai-q6-auxpcm.5",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_QUIN_AUXPCM_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
	{
		.name = LPASS_BE_QUIN_AUXPCM_TX,
		.stream_name = "Quin AUX PCM Capture",
		.cpu_dai_name = "msm-dai-q6-auxpcm.5",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_QUIN_AUXPCM_TX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_suspend = 1,
		.ops = &msm_aux_pcm_be_ops,
	},
};

static struct snd_soc_dai_link msm_wcn_be_dai_links[] = {
	{
		.name = LPASS_BE_SLIMBUS_7_RX,
		.stream_name = "Slimbus7 Playback",
		.cpu_dai_name = "msm-dai-q6-dev.16398",
		.platform_name = "msm-pcm-routing",
		.codec_name = "btfmslim_slave",
		/* BT codec driver determines capabilities based on
		 * dai name, bt codecdai name should always contains
		 * supported usecase information
		 */
		.codec_dai_name = "btfm_bt_sco_a2dp_slim_rx",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_7_RX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_wcn_ops,
		/* dai link has playback support */
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_7_TX,
		.stream_name = "Slimbus7 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16399",
		.platform_name = "msm-pcm-routing",
		.codec_name = "btfmslim_slave",
		.codec_dai_name = "btfm_bt_sco_slim_tx",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_7_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.ops = &msm_wcn_ops,
		.ignore_suspend = 1,
	},
	{
		.name = LPASS_BE_SLIMBUS_8_TX,
		.stream_name = "Slimbus8 Capture",
		.cpu_dai_name = "msm-dai-q6-dev.16401",
		.platform_name = "msm-pcm-routing",
		.codec_name = "btfmslim_slave",
		.codec_dai_name = "btfm_fm_slim_tx",
		.dynamic_be = 1,
		.no_pcm = 1,
		.dpcm_capture = 1,
		.id = MSM_BACKEND_DAI_SLIMBUS_8_TX,
		.be_hw_params_fixup = msm_ext_be_hw_params_fixup,
		.init = &msm_wcn_init,
		.ops = &msm_wcn_ops,
		.ignore_suspend = 1,
	},
};

static struct snd_soc_dai_link ext_disp_be_dai_link[] = {
	/* DISP PORT BACK END DAI Link */
	{
		.name = LPASS_BE_DISPLAY_PORT,
		.stream_name = "Display Port Playback",
		.cpu_dai_name = "msm-dai-q6-dp.0",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-ext-disp-audio-codec-rx",
		.codec_dai_name = "msm_dp_audio_codec_rx_dai",
		.no_pcm = 1,
		.dpcm_playback = 1,
		.id = MSM_BACKEND_DAI_DISPLAY_PORT_RX,
		.be_hw_params_fixup = msm_common_be_hw_params_fixup,
		.ignore_pmdown_time = 1,
		.ignore_suspend = 1,
	},
};

static struct snd_soc_dai_link msm_ext_madera_dai_links[
ARRAY_SIZE(msm_ext_common_fe_dai) +
ARRAY_SIZE(msm_ext_madera_fe_dai) +
ARRAY_SIZE(msm_ext_common_be_dai) +
ARRAY_SIZE(msm_ext_madera_be_dai) +
ARRAY_SIZE(msm_mi2s_be_dai_links) +
ARRAY_SIZE(msm_auxpcm_be_dai_links) +
ARRAY_SIZE(msm_wcn_be_dai_links) +
ARRAY_SIZE(msm_hdmi_dba_dai_links)];

static struct snd_soc_dai_link msm_ext_tasha_dai_links[
ARRAY_SIZE(msm_ext_common_fe_dai) +
ARRAY_SIZE(msm_ext_tasha_fe_dai) +
ARRAY_SIZE(msm_ext_common_be_dai) +
ARRAY_SIZE(msm_ext_tasha_be_dai) +
ARRAY_SIZE(msm_mi2s_be_dai_links) +
ARRAY_SIZE(msm_auxpcm_be_dai_links) +
ARRAY_SIZE(msm_wcn_be_dai_links) +
ARRAY_SIZE(ext_disp_be_dai_link)];

static struct snd_soc_dai_link msm_ext_tavil_dai_links[
ARRAY_SIZE(msm_ext_common_fe_dai) +
ARRAY_SIZE(msm_ext_tavil_fe_dai) +
ARRAY_SIZE(msm_ext_common_be_dai) +
ARRAY_SIZE(msm_ext_tavil_be_dai) +
ARRAY_SIZE(msm_mi2s_be_dai_links) +
ARRAY_SIZE(msm_auxpcm_be_dai_links) +
ARRAY_SIZE(msm_wcn_be_dai_links) +
ARRAY_SIZE(ext_disp_be_dai_link)];

/**
 * populate_snd_card_dailinks - prepares dailink array and initializes card.
 *
 * @dev: device handle
 *
 * Returns card on success or NULL on failure.
 */
struct snd_soc_card *populate_snd_card_dailinks(struct device *dev,
						int snd_card_val)
{
	struct snd_soc_card *card;
	struct snd_soc_dai_link *msm_ext_dai_links = NULL;
	int ret, len1, len2, len3, len4;
	enum codec_variant codec_ver = 0;

	if (snd_card_val == EXT_SND_CARD_MADERA) {
		card = &snd_soc_card_msm_card_madera;
#ifndef CONFIG_SND_SOC_MADERA
	} else if (snd_card_val == EXT_SND_CARD_TASHA) {
		card = &snd_soc_card_msm_card_tasha;
	} else if (snd_card_val == EXT_SND_CARD_TAVIL) {
		card = &snd_soc_card_msm_card_tavil;
#endif
	} else {
		dev_err(dev, "%s: failing as no matching card name\n",
			__func__);
		return NULL;
	}

	card->dev = dev;
	ret = snd_soc_of_parse_card_name(card, "qcom,model");
	if (ret) {
		dev_err(dev, "%s: parse card name failed, err:%d\n",
			__func__, ret);
		return NULL;
	}

	if (strnstr(card->name, "madera", strlen(card->name))) {
		len1 = ARRAY_SIZE(msm_ext_common_fe_dai);
		len2 = len1 + ARRAY_SIZE(msm_ext_madera_fe_dai);
		len3 = len2 + ARRAY_SIZE(msm_ext_common_be_dai);
		memcpy(msm_ext_madera_dai_links, msm_ext_common_fe_dai,
		       sizeof(msm_ext_common_fe_dai));
		memcpy(msm_ext_madera_dai_links + len1,
		       msm_ext_madera_fe_dai, sizeof(msm_ext_madera_fe_dai));
		memcpy(msm_ext_madera_dai_links + len2,
		       msm_ext_common_be_dai, sizeof(msm_ext_common_be_dai));
		memcpy(msm_ext_madera_dai_links + len3,
		       msm_ext_madera_be_dai, sizeof(msm_ext_madera_be_dai));

		len4 = len3 + ARRAY_SIZE(msm_ext_madera_be_dai);
		if (of_property_read_bool(dev->of_node,
					  "qcom,mi2s-audio-intf")) {
			memcpy(msm_ext_madera_dai_links + len4,
			       msm_mi2s_be_dai_links,
			       sizeof(msm_mi2s_be_dai_links));
			len4 += ARRAY_SIZE(msm_mi2s_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node,
					  "qcom,auxpcm-audio-intf")) {
			memcpy(msm_ext_madera_dai_links + len4,
			       msm_auxpcm_be_dai_links,
			       sizeof(msm_auxpcm_be_dai_links));
			len4 += ARRAY_SIZE(msm_auxpcm_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node, "qcom,wcn-btfm")) {
			dev_dbg(dev, "%s(): WCN BTFM support present\n",
					__func__);
			memcpy(msm_ext_madera_dai_links + len4,
				   msm_wcn_be_dai_links,
				   sizeof(msm_wcn_be_dai_links));
			len4 += ARRAY_SIZE(msm_wcn_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node,
					  "qcom,hdmi-dba-codec-rx")) {
			dev_dbg(dev, "%s(): hdmi dba audio present\n",
					__func__);
			memcpy(msm_ext_madera_dai_links + len4,
				   msm_hdmi_dba_dai_links,
				   sizeof(msm_hdmi_dba_dai_links));
			len4 += ARRAY_SIZE(msm_hdmi_dba_dai_links);
		}
		msm_ext_dai_links = msm_ext_madera_dai_links;
	} else if (strnstr(card->name, "tasha", strlen(card->name))) {
		codec_ver = tasha_codec_ver();
		if (codec_ver == WCD9326) {
			card->name = "sdm670-tashalite-snd-card";
		} else if (codec_ver == WCD9XXX) {
			dev_err(dev, "%s: Invalid codec version %d\n",
				__func__, codec_ver);
			return NULL;
		}

		len1 = ARRAY_SIZE(msm_ext_common_fe_dai);
		len2 = len1 + ARRAY_SIZE(msm_ext_tasha_fe_dai);
		len3 = len2 + ARRAY_SIZE(msm_ext_common_be_dai);
		memcpy(msm_ext_tasha_dai_links, msm_ext_common_fe_dai,
		       sizeof(msm_ext_common_fe_dai));
		memcpy(msm_ext_tasha_dai_links + len1,
		       msm_ext_tasha_fe_dai, sizeof(msm_ext_tasha_fe_dai));
		memcpy(msm_ext_tasha_dai_links + len2,
		       msm_ext_common_be_dai, sizeof(msm_ext_common_be_dai));
		memcpy(msm_ext_tasha_dai_links + len3,
		       msm_ext_tasha_be_dai, sizeof(msm_ext_tasha_be_dai));
		len4 = len3 + ARRAY_SIZE(msm_ext_tasha_be_dai);
		if (of_property_read_bool(dev->of_node,
					  "qcom,mi2s-audio-intf")) {
			memcpy(msm_ext_tasha_dai_links + len4,
			       msm_mi2s_be_dai_links,
			       sizeof(msm_mi2s_be_dai_links));
			len4 += ARRAY_SIZE(msm_mi2s_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node,
					  "qcom,auxpcm-audio-intf")) {
			memcpy(msm_ext_tasha_dai_links + len4,
			       msm_auxpcm_be_dai_links,
			       sizeof(msm_auxpcm_be_dai_links));
			len4 += ARRAY_SIZE(msm_auxpcm_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node, "qcom,wcn-btfm")) {
			dev_dbg(dev, "%s(): WCN BTFM support present\n",
					__func__);
			memcpy(msm_ext_tasha_dai_links + len4,
				   msm_wcn_be_dai_links,
				   sizeof(msm_wcn_be_dai_links));
			len4 += ARRAY_SIZE(msm_wcn_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node,
					  "qcom,ext-disp-audio-rx")) {
			dev_dbg(dev, "%s(): ext disp audio support present\n",
					__func__);
			memcpy(msm_ext_tasha_dai_links + len4,
				ext_disp_be_dai_link,
				sizeof(ext_disp_be_dai_link));
			len4 += ARRAY_SIZE(ext_disp_be_dai_link);
		}
		msm_ext_dai_links = msm_ext_tasha_dai_links;
	} else if (strnstr(card->name, "tavil", strlen(card->name))) {
		len1 = ARRAY_SIZE(msm_ext_common_fe_dai);
		len2 = len1 + ARRAY_SIZE(msm_ext_tavil_fe_dai);
		len3 = len2 + ARRAY_SIZE(msm_ext_common_be_dai);
		memcpy(msm_ext_tavil_dai_links, msm_ext_common_fe_dai,
		       sizeof(msm_ext_common_fe_dai));
		memcpy(msm_ext_tavil_dai_links + len1,
		       msm_ext_tavil_fe_dai, sizeof(msm_ext_tavil_fe_dai));
		memcpy(msm_ext_tavil_dai_links + len2,
		       msm_ext_common_be_dai, sizeof(msm_ext_common_be_dai));
		memcpy(msm_ext_tavil_dai_links + len3,
		       msm_ext_tavil_be_dai, sizeof(msm_ext_tavil_be_dai));
		len4 = len3 + ARRAY_SIZE(msm_ext_tavil_be_dai);
		if (of_property_read_bool(dev->of_node,
					  "qcom,mi2s-audio-intf")) {
			memcpy(msm_ext_tavil_dai_links + len4,
			       msm_mi2s_be_dai_links,
			       sizeof(msm_mi2s_be_dai_links));
			len4 += ARRAY_SIZE(msm_mi2s_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node,
					  "qcom,auxpcm-audio-intf")) {
			memcpy(msm_ext_tavil_dai_links + len4,
			       msm_auxpcm_be_dai_links,
			       sizeof(msm_auxpcm_be_dai_links));
			len4 += ARRAY_SIZE(msm_auxpcm_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node, "qcom,wcn-btfm")) {
			dev_dbg(dev, "%s(): WCN BTFM support present\n",
					__func__);
			memcpy(msm_ext_tavil_dai_links + len4,
				   msm_wcn_be_dai_links,
				   sizeof(msm_wcn_be_dai_links));
			len4 += ARRAY_SIZE(msm_wcn_be_dai_links);
		}
		if (of_property_read_bool(dev->of_node,
					  "qcom,ext-disp-audio-rx")) {
			dev_dbg(dev, "%s(): ext disp audio support present\n",
					__func__);
			memcpy(msm_ext_tavil_dai_links + len4,
				ext_disp_be_dai_link,
				sizeof(ext_disp_be_dai_link));
			len4 += ARRAY_SIZE(ext_disp_be_dai_link);
		}
		msm_ext_dai_links = msm_ext_tavil_dai_links;
	} else {
		dev_err(dev, "%s: failing as no matching card name\n",
			__func__);
		return NULL;
	}
	card->dai_link = msm_ext_dai_links;
	card->num_links = len4;

	return card;
}
EXPORT_SYMBOL(populate_snd_card_dailinks);
