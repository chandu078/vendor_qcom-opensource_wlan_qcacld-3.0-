/*
 * Copyright (c) 2016-2021, The Linux Foundation. All rights reserved.

 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _DP_MON_H_
#define _DP_MON_H_

#include "dp_htt.h"
#ifdef WLAN_TX_PKT_CAPTURE_ENH
#include "dp_tx_capture.h"
#endif

#define DP_INTR_POLL_TIMER_MS	5

#define MON_VDEV_TIMER_INIT 0x1
#define MON_VDEV_TIMER_RUNNING 0x2

/* Budget to reap monitor status ring */
#define DP_MON_REAP_BUDGET 1024
#define MON_BUF_MIN_ENTRIES 64

#define mon_rx_warn(params...) QDF_TRACE_WARN(QDF_MODULE_ID_DP_RX, params)

#ifndef WLAN_TX_PKT_CAPTURE_ENH
struct dp_pdev_tx_capture {
};

struct dp_peer_tx_capture {
};
#endif

struct dp_mon_ops {
	QDF_STATUS (*mon_soc_cfg_init)(struct dp_soc *soc);
	QDF_STATUS (*mon_pdev_attach)(struct dp_pdev *pdev);
	QDF_STATUS (*mon_pdev_detach)(struct dp_pdev *pdev);
	QDF_STATUS (*mon_pdev_init)(struct dp_pdev *pdev);
	QDF_STATUS (*mon_pdev_deinit)(struct dp_pdev *pdev);
	QDF_STATUS (*mon_vdev_attach)(struct dp_vdev *vdev);
	QDF_STATUS (*mon_vdev_detach)(struct dp_vdev *vdev);
	QDF_STATUS (*mon_peer_attach)(struct dp_peer *peer);
	QDF_STATUS (*mon_peer_detach)(struct dp_peer *peer);
	QDF_STATUS (*mon_config_debug_sniffer)(struct dp_pdev *pdev, int val);
	void (*mon_flush_rings)(struct dp_soc *soc);
#if !defined(DISABLE_MON_CONFIG)
	QDF_STATUS (*mon_htt_srng_setup)(struct dp_soc *soc,
					 struct dp_pdev *pdev,
					 int mac_id,
					 int mac_for_pdev);
#endif
#if !defined(DISABLE_MON_CONFIG) && defined(MON_ENABLE_DROP_FOR_MAC)
	uint32_t (*mon_drop_packets_for_mac)(struct dp_pdev *pdev,
					     uint32_t mac_id,
					     uint32_t quota);
#endif
#if defined(DP_CON_MON)
	void (*mon_service_rings)(struct  dp_soc *soc, uint32_t quota);
#endif
#ifndef DISABLE_MON_CONFIG
	uint32_t (*mon_process)(struct dp_soc *soc,
				struct dp_intr *int_ctx,
				uint32_t mac_id,
				uint32_t quota);
#endif
	void (*mon_peer_tx_init)(struct dp_pdev *pdev, struct dp_peer *peer);
	void (*mon_peer_tx_cleanup)(struct dp_vdev *vdev,
				    struct dp_peer *peer);
#ifdef WLAN_TX_PKT_CAPTURE_ENH
	void (*mon_peer_tid_peer_id_update)(struct dp_peer *peer,
					    uint16_t peer_id);
	void (*mon_tx_ppdu_stats_attach)(struct dp_pdev *pdev);
	void (*mon_tx_ppdu_stats_detach)(struct dp_pdev *pdev);
	QDF_STATUS (*mon_tx_capture_debugfs_init)(struct dp_pdev *pdev);
	void (*mon_peer_tx_capture_filter_check)(struct dp_pdev *pdev,
						 struct dp_peer *peer);
	QDF_STATUS (*mon_tx_add_to_comp_queue)(struct dp_soc *soc,
					       struct dp_tx_desc_s *desc,
					       struct hal_tx_completion_status *ts,
					       struct dp_peer *peer);
#endif
#if defined(WDI_EVENT_ENABLE) &&\
	(defined(QCA_ENHANCED_STATS_SUPPORT) || !defined(REMOVE_PKT_LOG))
	bool (*mon_ppdu_stats_ind_handler)(struct htt_soc *soc,
					   uint32_t *msg_word,
					   qdf_nbuf_t htt_t2h_msg);
#endif
	QDF_STATUS (*mon_htt_ppdu_stats_attach)(struct dp_pdev *pdev);
	void (*mon_htt_ppdu_stats_detach)(struct dp_pdev *pdev);
	void (*mon_print_pdev_rx_mon_stats)(struct dp_pdev *pdev);

#ifdef WLAN_TX_PKT_CAPTURE_ENH
	void (*mon_print_pdev_tx_capture_stats)(struct dp_pdev *pdev);
	QDF_STATUS (*mon_config_enh_tx_capture)(struct dp_pdev *pdev,
						uint8_t val);
#endif
#ifdef WLAN_RX_PKT_CAPTURE_ENH
	QDF_STATUS (*mon_config_enh_rx_capture)(struct dp_pdev *pdev,
						uint8_t val);
#endif
#ifdef QCA_SUPPORT_BPR
	QDF_STATUS (*mon_set_bpr_enable)(struct dp_pdev *pdev, int val);
#endif
#ifdef ATH_SUPPORT_NAC
	int (*mon_set_filter_neigh_peers)(struct dp_pdev *pdev, bool val);
#endif
#ifdef WLAN_ATF_ENABLE
	void (*mon_set_atf_stats_enable)(struct dp_pdev *pdev, bool value);
#endif
	void (*mon_set_bsscolor)(struct dp_pdev *pdev, uint8_t bsscolor);
	bool (*mon_pdev_get_filter_ucast_data)(struct cdp_pdev *pdev_handle);
	bool (*mon_pdev_get_filter_non_data)(struct cdp_pdev *pdev_handle);
	bool (*mon_pdev_get_filter_mcast_data)(struct cdp_pdev *pdev_handle);
#ifdef WDI_EVENT_ENABLE
	int (*mon_set_pktlog_wifi3)(struct dp_pdev *pdev, uint32_t event,
				    bool enable);
#endif
#if defined(DP_CON_MON) && !defined(REMOVE_PKT_LOG)
	void (*mon_pktlogmod_exit)(struct dp_pdev *pdev);
#endif
	void (*mon_vdev_set_monitor_mode_buf_rings)(struct dp_pdev *pdev);
	void (*mon_neighbour_peers_detach)(struct dp_pdev *pdev);
#ifdef FEATURE_NAC_RSSI
	QDF_STATUS (*mon_filter_neighbour_peer)(struct dp_pdev *pdev,
						uint8_t *rx_pkt_hdr);
#endif
	void (*mon_vdev_timer_init)(struct dp_soc *soc);
	void (*mon_vdev_timer_start)(struct dp_soc *soc);
	bool (*mon_vdev_timer_stop)(struct dp_soc *soc);
	void (*mon_vdev_timer_deinit)(struct dp_soc *soc);
	void (*mon_reap_timer_init)(struct dp_soc *soc);
	void (*mon_reap_timer_start)(struct dp_soc *soc);
	bool (*mon_reap_timer_stop)(struct dp_soc *soc);
	void (*mon_reap_timer_deinit)(struct dp_soc *soc);
#ifdef QCA_MCOPY_SUPPORT
	QDF_STATUS (*mon_mcopy_check_deliver)(struct dp_pdev *pdev,
					      uint16_t peer_id,
					      uint32_t ppdu_id,
					      uint8_t first_msdu);
#endif
	void (*mon_neighbour_peer_add_ast)(struct dp_pdev *pdev,
					   struct dp_peer *ta_peer,
					   uint8_t *mac_addr,
					   qdf_nbuf_t nbuf,
					   uint32_t flags);
};

struct dp_mon_soc {
	/* Holds all monitor related fields extracted from dp_soc */
	/* Holds pointer to monitor ops */
	/* monitor link descriptor pages */
	struct qdf_mem_multi_page_t mon_link_desc_pages[MAX_NUM_LMAC_HW];

	/* total link descriptors for monitor mode for each radio */
	uint32_t total_mon_link_descs[MAX_NUM_LMAC_HW];

	 /* Monitor Link descriptor memory banks */
	struct link_desc_bank
		mon_link_desc_banks[MAX_NUM_LMAC_HW][MAX_MON_LINK_DESC_BANKS];
	uint32_t num_mon_link_desc_banks[MAX_NUM_LMAC_HW];
	/* Smart monitor capability for HKv2 */
	uint8_t hw_nac_monitor_support;

	/* Full monitor mode support */
	bool full_mon_mode;

	/*interrupt timer*/
	qdf_timer_t mon_reap_timer;
	uint8_t reap_timer_init;

	qdf_timer_t mon_vdev_timer;
	uint8_t mon_vdev_timer_state;

	struct dp_mon_ops *mon_ops;
};

struct  dp_mon_pdev {
	/* monitor */
	bool monitor_configured;

	struct dp_mon_filter **filter;	/* Monitor Filter pointer */

	/* advance filter mode and type*/
	uint8_t mon_filter_mode;
	uint16_t fp_mgmt_filter;
	uint16_t fp_ctrl_filter;
	uint16_t fp_data_filter;
	uint16_t mo_mgmt_filter;
	uint16_t mo_ctrl_filter;
	uint16_t mo_data_filter;
	uint16_t md_data_filter;

	struct dp_pdev_tx_capture tx_capture;

	/* tx packet capture enhancement */
	enum cdp_tx_enh_capture_mode tx_capture_enabled;
	/* Stuck count on monitor destination ring MPDU process */
	uint32_t mon_dest_ring_stuck_cnt;
	/* monitor mode lock */
	qdf_spinlock_t mon_lock;

	/* Monitor mode operation channel */
	int mon_chan_num;

	/* Monitor mode operation frequency */
	qdf_freq_t mon_chan_freq;

	/* Monitor mode band */
	enum reg_wifi_band mon_chan_band;

	uint32_t mon_ppdu_status;
	/* monitor mode status/destination ring PPDU and MPDU count */
	struct cdp_pdev_mon_stats rx_mon_stats;
	/* Monitor mode interface and status storage */
	struct dp_vdev *mvdev;
	struct cdp_mon_status rx_mon_recv_status;
	/* to track duplicate link descriptor indications by HW for a WAR */
	uint64_t mon_last_linkdesc_paddr;
	/* to track duplicate buffer indications by HW for a WAR */
	uint32_t mon_last_buf_cookie;

#ifdef QCA_SUPPORT_FULL_MON
	/* List to maintain all MPDUs for a PPDU in monitor mode */
	TAILQ_HEAD(, dp_mon_mpdu) mon_mpdu_q;

	/* TODO: define per-user mpdu list
	 * struct dp_mon_mpdu_list mpdu_list[MAX_MU_USERS];
	 */
	struct hal_rx_mon_desc_info *mon_desc;
#endif
	/* Flag to hold on to monitor destination ring */
	bool hold_mon_dest_ring;

	/* Flag to inidicate monitor rings are initialized */
	uint8_t pdev_mon_init;
#ifndef REMOVE_PKT_LOG
	bool pkt_log_init;
	struct pktlog_dev_t *pl_dev; /* Pktlog pdev */
#endif /* #ifndef REMOVE_PKT_LOG */

	/* Smart Mesh */
	bool filter_neighbour_peers;

	/*flag to indicate neighbour_peers_list not empty */
	bool neighbour_peers_added;
	/* smart mesh mutex */
	qdf_spinlock_t neighbour_peer_mutex;
	/* Neighnour peer list */
	TAILQ_HEAD(, dp_neighbour_peer) neighbour_peers_list;
	/* Enhanced Stats is enabled */
	bool enhanced_stats_en;
	qdf_nbuf_queue_t rx_status_q;

	/* 128 bytes mpdu header queue per user for ppdu */
	qdf_nbuf_queue_t mpdu_q[MAX_MU_USERS];

	/* is this a mpdu header TLV and not msdu header TLV */
	bool is_mpdu_hdr[MAX_MU_USERS];

	/* per user 128 bytes msdu header list for MPDU */
	struct msdu_list msdu_list[MAX_MU_USERS];

	/* RX enhanced capture mode */
	uint8_t rx_enh_capture_mode;
	/* Rx per peer enhanced capture mode */
	bool rx_enh_capture_peer;
	struct dp_vdev *rx_enh_monitor_vdev;
	/* RX enhanced capture trailer enable/disable flag */
	bool is_rx_enh_capture_trailer_enabled;
#ifdef WLAN_RX_PKT_CAPTURE_ENH
	/* RX per MPDU/PPDU information */
	struct cdp_rx_indication_mpdu mpdu_ind;
#endif

	/* Packet log mode */
	uint8_t rx_pktlog_mode;
	/* Enable pktlog logging cbf */
	bool rx_pktlog_cbf;

	bool tx_sniffer_enable;
	/* mirror copy mode */
	enum m_copy_mode mcopy_mode;
	bool enable_reap_timer_non_pkt;
	bool bpr_enable;
	/* Pdev level flag to check peer based pktlog enabled or
	 * disabled
	 */
	uint8_t dp_peer_based_pktlog;

#ifdef WLAN_ATF_ENABLE
	/* ATF stats enable */
	bool dp_atf_stats_enable;
#endif

	/* Maintains first status buffer's paddr of a PPDU */
	uint64_t status_buf_addr;
	struct hal_rx_ppdu_info ppdu_info;

	/* ppdu_id of last received HTT TX stats */
	uint32_t last_ppdu_id;
	struct {
		uint8_t last_user;
		qdf_nbuf_t buf;
	} tx_ppdu_info;

	struct {
		uint32_t tx_ppdu_id;
		uint16_t tx_peer_id;
		uint32_t rx_ppdu_id;
	} m_copy_id;

	/* To check if PPDU Tx stats are enabled for Pktlog */
	bool pktlog_ppdu_stats;

#ifdef ATH_SUPPORT_NAC_RSSI
	bool nac_rssi_filtering;
#endif

	/* ppdu_stats lock for queue concurrency between cores*/
	qdf_spinlock_t ppdu_stats_lock;

	/* list of ppdu tlvs */
	TAILQ_HEAD(, ppdu_info) ppdu_info_list;
	TAILQ_HEAD(, ppdu_info) sched_comp_ppdu_list;

	uint32_t sched_comp_list_depth;
	uint16_t delivered_sched_cmdid;
	uint16_t last_sched_cmdid;
	uint32_t tlv_count;
	uint32_t list_depth;

	struct {
		qdf_nbuf_t last_nbuf; /*Ptr to mgmt last buf */
		uint8_t *mgmt_buf; /* Ptr to mgmt. payload in HTT ppdu stats */
		uint32_t mgmt_buf_len; /* Len of mgmt. payload in ppdu stats */
		uint32_t ppdu_id;
	} mgmtctrl_frm_info;
	/* Context of cal client timer */
	struct cdp_cal_client *cal_client_ctx;
	uint32_t *ppdu_tlv_buf; /* Buffer to hold HTT ppdu stats TLVs*/

	qdf_nbuf_t mcopy_status_nbuf;
	bool is_dp_mon_pdev_initialized;
};

struct  dp_mon_vdev {
	/* callback to hand rx monitor 802.11 MPDU to the OS shim */
	ol_txrx_rx_mon_fp osif_rx_mon;
};

struct dp_mon_peer {
	struct dp_peer_tx_capture tx_capture;
#ifdef FEATURE_PERPKT_INFO
	/* delayed ba ppdu stats handling */
	struct cdp_delayed_tx_completion_ppdu_user delayed_ba_ppdu_stats;
	/* delayed ba flag */
	bool last_delayed_ba;
	/* delayed ba ppdu id */
	uint32_t last_delayed_ba_ppduid;
#endif
};

struct mon_ops {
};

#if defined(QCA_TX_CAPTURE_SUPPORT) || defined(QCA_ENHANCED_STATS_SUPPORT)
void dp_deliver_mgmt_frm(struct dp_pdev *pdev, qdf_nbuf_t nbuf);
#else
static inline
void dp_deliver_mgmt_frm(struct dp_pdev *pdev, qdf_nbuf_t nbuf)
{
}
#endif

#if defined(WLAN_SUPPORT_RX_PROTOCOL_TYPE_TAG) ||\
	defined(WLAN_SUPPORT_RX_FLOW_TAG)
/**
 * dp_rx_mon_update_protocol_flow_tag() - Performs necessary checks for monitor
 *                                       mode and then tags appropriate packets
 * @soc: core txrx main context
 * @vdev: pdev on which packet is received
 * @msdu: QDF packet buffer on which the protocol tag should be set
 * @rx_desc: base address where the RX TLVs start
 * Return: void
 */
void dp_rx_mon_update_protocol_flow_tag(struct dp_soc *soc,
					struct dp_pdev *dp_pdev,
					qdf_nbuf_t msdu, void *rx_desc);
#endif /* WLAN_SUPPORT_RX_PROTOCOL_TYPE_TAG || WLAN_SUPPORT_RX_FLOW_TAG */

#if !defined(WLAN_SUPPORT_RX_PROTOCOL_TYPE_TAG) &&\
	!defined(WLAN_SUPPORT_RX_FLOW_TAG)
/**
 * dp_rx_mon_update_protocol_flow_tag() - Performs necessary checks for monitor
 *                                       mode and then tags appropriate packets
 * @soc: core txrx main context
 * @vdev: pdev on which packet is received
 * @msdu: QDF packet buffer on which the protocol tag should be set
 * @rx_desc: base address where the RX TLVs start
 * Return: void
 */
static inline
void dp_rx_mon_update_protocol_flow_tag(struct dp_soc *soc,
					struct dp_pdev *dp_pdev,
					qdf_nbuf_t msdu, void *rx_desc)
{
}
#endif /* WLAN_SUPPORT_RX_PROTOCOL_TYPE_TAG || WLAN_SUPPORT_RX_FLOW_TAG */

#ifndef WLAN_TX_PKT_CAPTURE_ENH
static inline
QDF_STATUS dp_peer_set_tx_capture_enabled(struct dp_pdev *pdev,
					  struct dp_peer *peer_handle,
					  uint8_t value, uint8_t *peer_mac)
{
	return QDF_STATUS_SUCCESS;
}

/**
 * dp_peer_tid_queue_init() – Initialize ppdu stats queue per TID
 * @peer: Datapath peer
 *
 */
static inline void dp_peer_tid_queue_init(struct dp_peer *peer)
{
}

/**
 * dp_peer_tid_queue_cleanup() – remove ppdu stats queue per TID
 * @peer: Datapath peer
 *
 */
static inline void dp_peer_tid_queue_cleanup(struct dp_peer *peer)
{
}

/**
 * dp_peer_update_80211_hdr() – dp peer update 80211 hdr
 * @vdev: Datapath vdev
 * @peer: Datapath peer
 *
 */
static inline void
dp_peer_update_80211_hdr(struct dp_vdev *vdev, struct dp_peer *peer)
{
}

/**
 * dp_tx_ppdu_stats_attach - Initialize Tx PPDU stats and enhanced capture
 * @pdev: DP PDEV
 *
 * Return: none
 */
static inline void dp_tx_ppdu_stats_attach(struct dp_pdev *pdev)
{
}

/**
 * dp_tx_ppdu_stats_detach - Cleanup Tx PPDU stats and enhanced capture
 * @pdev: DP PDEV
 *
 * Return: none
 */
static inline void dp_tx_ppdu_stats_detach(struct dp_pdev *pdev)
{
}

/**
 * dp_tx_add_to_comp_queue() - add completion msdu to queue
 * @soc: DP Soc handle
 * @tx_desc: software Tx descriptor
 * @ts : Tx completion status from HAL/HTT descriptor
 * @peer: DP peer
 *
 * Return: none
 */
static inline
QDF_STATUS dp_tx_add_to_comp_queue(struct dp_soc *soc,
				   struct dp_tx_desc_s *desc,
				   struct hal_tx_completion_status *ts,
				   struct dp_peer *peer)
{
	return QDF_STATUS_E_FAILURE;
}

/*
 * dp_peer_tx_capture_filter_check: check filter is enable for the filter
 * and update tx_cap_enabled flag
 * @pdev: DP PDEV handle
 * @peer: DP PEER handle
 *
 * return: void
 */
static inline
void dp_peer_tx_capture_filter_check(struct dp_pdev *pdev,
				     struct dp_peer *peer)
{
}

/*
 * dp_tx_capture_debugfs_init: tx capture debugfs init
 * @pdev: DP PDEV handle
 *
 * return: QDF_STATUS
 */
static inline
QDF_STATUS dp_tx_capture_debugfs_init(struct dp_pdev *pdev)
{
	return QDF_STATUS_E_FAILURE;
}
#endif

#ifdef WLAN_TX_PKT_CAPTURE_ENH
extern uint8_t
dp_cpu_ring_map[DP_NSS_CPU_RING_MAP_MAX][WLAN_CFG_INT_NUM_CONTEXTS_MAX];
#endif

int
dp_htt_get_ppdu_sniffer_ampdu_tlv_bitmap(uint32_t bitmap);
/**
 * dp_ppdu_desc_user_stats_update(): Function to update TX user stats
 * @pdev: DP pdev handle
 * @ppdu_info: per PPDU TLV descriptor
 *
 * return: void
 */
void
dp_ppdu_desc_user_stats_update(struct dp_pdev *pdev,
			       struct ppdu_info *ppdu_info);

#ifdef WDI_EVENT_ENABLE
void dp_pkt_log_init(struct cdp_soc_t *soc_hdl, uint8_t pdev_id, void *scn);
#else
static inline void
dp_pkt_log_init(struct cdp_soc_t *soc_hdl, uint8_t pdev_id, void *scn)
{
}
#endif

#ifdef WDI_EVENT_ENABLE
QDF_STATUS dp_peer_stats_notify(struct dp_pdev *pdev, struct dp_peer *peer);
#else
static inline QDF_STATUS dp_peer_stats_notify(struct dp_pdev *pdev,
					      struct dp_peer *peer)
{
	return QDF_STATUS_SUCCESS;
}
#endif

#ifndef WLAN_TX_PKT_CAPTURE_ENH
/**
 * dp_tx_ppdu_stats_process - Deferred PPDU stats handler
 * @context: Opaque work context (PDEV)
 *
 * Return: none
 */
static  inline void dp_tx_ppdu_stats_process(void *context)
{
}

/*
 * dp_tx_capture_htt_frame_counter: increment counter for htt_frame_type
 * pdev: DP pdev handle
 * htt_frame_type: htt frame type received from fw
 *
 * return: void
 */
static inline
void dp_tx_capture_htt_frame_counter(struct dp_pdev *pdev,
				     uint32_t htt_frame_type)
{
}

/*
 * dp_tx_cature_stats: print tx capture stats
 * @pdev: DP PDEV handle
 *
 * return: void
 */
static inline
void dp_print_pdev_tx_capture_stats(struct dp_pdev *pdev)
{
}
#endif

/**
 * dp_rx_cookie_2_mon_link_desc_va() - Converts cookie to a virtual address of
 *				   the MSDU Link Descriptor
 * @pdev: core txrx pdev context
 * @buf_info: buf_info includes cookie that used to lookup virtual address of
 * link descriptor. Normally this is just an index into a per pdev array.
 *
 * This is the VA of the link descriptor in monitor mode destination ring,
 * that HAL layer later uses to retrieve the list of MSDU's for a given MPDU.
 *
 * Return: void *: Virtual Address of the Rx descriptor
 */
static inline
void *dp_rx_cookie_2_mon_link_desc_va(struct dp_pdev *pdev,
				      struct hal_buf_info *buf_info,
				      int mac_id)
{
	void *link_desc_va;
	struct qdf_mem_multi_page_t *pages;
	uint16_t page_id = LINK_DESC_COOKIE_PAGE_ID(buf_info->sw_cookie);
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc)
		return NULL;

	pages = &mon_soc->mon_link_desc_pages[mac_id];
	if (!pages)
		return NULL;

	if (qdf_unlikely(page_id >= pages->num_pages))
		return NULL;

	link_desc_va = pages->dma_pages[page_id].page_v_addr_start +
		(buf_info->paddr - pages->dma_pages[page_id].page_p_addr);

	return link_desc_va;
}

/**
 * dp_soc_is_full_mon_enable () - Return if full monitor mode is enabled
 * @soc: DP soc handle
 *
 * Return: Full monitor mode status
 */
static inline bool dp_soc_is_full_mon_enable(struct dp_pdev *pdev)
{
	return (pdev->soc->monitor_soc->full_mon_mode &&
		pdev->monitor_pdev->monitor_configured) ? true : false;
}

/*
 * monitor_is_enable_reap_timer_non_pkt() - check if mon reap timer is
 * enabled by non-pkt log or not
 * @pdev: point to dp pdev
 *
 * Return: true if mon reap timer is enabled by non-pkt log
 */
static inline bool monitor_is_enable_reap_timer_non_pkt(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return false;

	return pdev->monitor_pdev->enable_reap_timer_non_pkt;
}

/*
 * monitor_is_enable_mcopy_mode() - check if mcopy mode is enabled
 * @pdev: point to dp pdev
 *
 * Return: true if mcopy mode is enabled
 */
static inline bool monitor_is_enable_mcopy_mode(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return false;

	return pdev->monitor_pdev->mcopy_mode;
}

/*
 * monitor_is_enable_tx_sniffer() - check if tx sniffer is enabled
 * @pdev: point to dp pdev
 *
 * Return: true if tx sniffer is enabled
 */
static inline bool monitor_is_enable_tx_sniffer(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return false;

	return pdev->monitor_pdev->tx_sniffer_enable;
}

/*
 * monitor_is_set_monitor_configured() - check if monitor configured is set
 * @pdev: point to dp pdev
 *
 * Return: true if monitor configured is set
 */
static inline bool monitor_is_configured(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return false;

	return pdev->monitor_pdev->monitor_configured;
}

static inline QDF_STATUS monitor_check_com_info_ppdu_id(struct dp_pdev *pdev,
							void *rx_desc)
{
	struct cdp_mon_status *rs;
	struct dp_mon_pdev *mon_pdev;
	uint32_t msdu_ppdu_id = 0;

	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return QDF_STATUS_E_FAILURE;

	mon_pdev = pdev->monitor_pdev;
	if (qdf_likely(1 != mon_pdev->ppdu_info.rx_status.rxpcu_filter_pass))
		return QDF_STATUS_E_FAILURE;

	rs = &pdev->monitor_pdev->rx_mon_recv_status;
	if (!rs || rs->cdp_rs_rxdma_err)
		return QDF_STATUS_E_FAILURE;

	msdu_ppdu_id = hal_rx_get_ppdu_id(pdev->soc->hal_soc, rx_desc);
	if (msdu_ppdu_id != mon_pdev->ppdu_info.com_info.ppdu_id) {
		QDF_TRACE(QDF_MODULE_ID_DP,
			  QDF_TRACE_LEVEL_ERROR,
			  "msdu_ppdu_id=%x,com_info.ppdu_id=%x",
			  msdu_ppdu_id,
			  mon_pdev->ppdu_info.com_info.ppdu_id);
		return QDF_STATUS_E_FAILURE;
	}

	return QDF_STATUS_SUCCESS;
}

static inline struct mon_rx_status*
monitor_get_rx_status(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return NULL;

	return &pdev->monitor_pdev->ppdu_info.rx_status;
}

/*
 * monitor_is_chan_band_known() - check if monitor chan band known
 * @pdev: point to dp pdev
 *
 * Return: true if chan band known
 */
static inline bool monitor_is_chan_band_known(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return false;

	if (pdev->monitor_pdev->mon_chan_band != REG_BAND_UNKNOWN)
		return true;

	return false;
}

/*
 * monitor_get_chan_band() - get chan band
 * @pdev: point to dp pdev
 *
 * Return: wifi channel band
 */
static inline enum reg_wifi_band
monitor_get_chan_band(struct dp_pdev *pdev)
{
	return pdev->monitor_pdev->mon_chan_band;
}

/*
 * monitor_print_tx_stats() - print tx stats from monitor pdev
 * @pdev: point to dp pdev
 *
 */
static inline void monitor_print_tx_stats(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	DP_PRINT_STATS("ppdu info schedule completion list depth: %d",
		       pdev->monitor_pdev->sched_comp_list_depth);
	DP_PRINT_STATS("delivered sched cmdid: %d",
		       pdev->monitor_pdev->delivered_sched_cmdid);
	DP_PRINT_STATS("cur sched cmdid: %d",
		       pdev->monitor_pdev->last_sched_cmdid);
	DP_PRINT_STATS("ppdu info list depth: %d",
		       pdev->monitor_pdev->list_depth);
}

/*
 * monitor_is_enable_enhanced_stats() - check if enhanced stats enabled
 * @pdev: point to dp pdev
 *
 * Return: true if enhanced stats is enabled
 */
static inline bool monitor_is_enable_enhanced_stats(struct dp_pdev *pdev)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return false;

	return pdev->monitor_pdev->enhanced_stats_en;
}

/*
 * monitor_set_chan_num() - set channel number
 * @pdev: point to dp pdev
 * @chan_num: channel number
 *
 * Return:
 */
static inline void monitor_set_chan_num(struct dp_pdev *pdev, int chan_num)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	pdev->monitor_pdev->mon_chan_num = chan_num;
}

/*
 * monitor_set_chan_freq() - set channel frequency
 * @pdev: point to dp pdev
 * @chan_freq: channel frequency
 *
 * Return:
 */
static inline void
monitor_set_chan_freq(struct dp_pdev *pdev, qdf_freq_t chan_freq)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	pdev->monitor_pdev->mon_chan_freq = chan_freq;
}

/*
 * monitor_set_chan_band() - set channel band
 * @pdev: point to dp pdev
 * @chan_band: channel band
 *
 * Return:
 */
static inline void
monitor_set_chan_band(struct dp_pdev *pdev, enum reg_wifi_band chan_band)
{
	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	pdev->monitor_pdev->mon_chan_band = chan_band;
}

/*
 * monitor_get_mpdu_status() - get mpdu status
 * @pdev: point to dp pdev
 * @soc: point to dp soc
 *
 */
static inline void monitor_get_mpdu_status(struct dp_pdev *pdev,
					   struct dp_soc *soc,
					   uint8_t *rx_tlv_hdr)
{
	struct dp_mon_pdev *mon_pdev;

	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	mon_pdev = pdev->monitor_pdev;
	hal_rx_mon_hw_desc_get_mpdu_status(soc->hal_soc, rx_tlv_hdr,
					   &mon_pdev->ppdu_info.rx_status);
}

#ifdef FEATURE_NAC_RSSI
static inline QDF_STATUS monitor_drop_inv_peer_pkts(struct dp_vdev *vdev,
						    struct ieee80211_frame *wh)
{
	struct dp_pdev *pdev = vdev->pdev;
	struct dp_soc *soc = pdev->soc;

	if (!soc->monitor_soc)
		return QDF_STATUS_E_FAILURE;

	if (!soc->monitor_soc->hw_nac_monitor_support &&
	    pdev->monitor_pdev->filter_neighbour_peers &&
	    vdev->opmode == wlan_op_mode_sta) {
		mon_rx_warn("%pK: Drop inv peer pkts with STA RA:%pm",
			    soc, wh->i_addr1);
		return QDF_STATUS_SUCCESS;
	}

	return QDF_STATUS_E_FAILURE;
}
#endif

#ifdef FEATURE_PERPKT_INFO
/*
 * dp_peer_ppdu_delayed_ba_init() Initialize ppdu in peer
 * @peer: Datapath peer
 *
 * return: void
 */
static inline void dp_peer_ppdu_delayed_ba_init(struct dp_peer *peer)
{
	struct dp_mon_peer *mon_peer = peer->monitor_peer;

	if (!mon_peer)
		return;

	qdf_mem_zero(&mon_peer->delayed_ba_ppdu_stats,
		     sizeof(struct cdp_delayed_tx_completion_ppdu_user));
	mon_peer->last_delayed_ba = false;
	mon_peer->last_delayed_ba_ppduid = 0;
}
#else
/*
 * dp_peer_ppdu_delayed_ba_init() Initialize ppdu in peer
 * @peer: Datapath peer
 *
 * return: void
 */
static inline void dp_peer_ppdu_delayed_ba_init(struct dp_peer *peer)
{
}
#endif

static inline void monitor_vdev_register_osif(struct dp_vdev *vdev,
					      struct ol_txrx_ops *txrx_ops)
{
	if (!vdev->monitor_vdev)
		return;

	vdev->monitor_vdev->osif_rx_mon = txrx_ops->rx.mon;
}

static inline struct dp_vdev*
monitor_get_monitor_vdev_from_pdev(struct dp_pdev *pdev)
{
	if (!pdev || !pdev->monitor_pdev || !pdev->monitor_pdev->mvdev)
		return NULL;

	return pdev->monitor_pdev->mvdev;
}

static inline bool monitor_is_vdev_timer_running(struct dp_soc *soc)
{
	struct dp_mon_soc *mon_soc;

	if (qdf_unlikely(!soc || !soc->monitor_soc))
		return false;

	mon_soc = soc->monitor_soc;

	return mon_soc->mon_vdev_timer_state & MON_VDEV_TIMER_RUNNING;
}

static inline struct qdf_mem_multi_page_t*
monitor_get_link_desc_pages(struct dp_soc *soc, uint32_t mac_id)
{
	if (qdf_unlikely(!soc || !soc->monitor_soc))
		return NULL;

	return &soc->monitor_soc->mon_link_desc_pages[mac_id];
}

static inline uint32_t *
monitor_get_total_link_descs(struct dp_soc *soc, uint32_t mac_id)
{
	return &soc->monitor_soc->total_mon_link_descs[mac_id];
}

static inline QDF_STATUS monitor_pdev_attach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	/*
	 * mon_soc uninitialized modular support enabled
	 * monitor related attach/detach/init/deinit
	 * will be done while monitor insmod
	 */
	if (!mon_soc)
		return QDF_STATUS_SUCCESS;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_attach) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_pdev_attach(pdev);
}

static inline QDF_STATUS monitor_pdev_detach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	/*
	 * mon_soc uninitialized modular support enabled
	 * monitor related attach/detach/init/deinit
	 * will be done while monitor insmod
	 */
	if (!mon_soc)
		return QDF_STATUS_SUCCESS;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_detach) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_pdev_detach(pdev);
}

static inline QDF_STATUS monitor_vdev_attach(struct dp_vdev *vdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = vdev->pdev->soc->monitor_soc;

	if (!mon_soc)
		return QDF_STATUS_E_FAILURE;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_attach) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_vdev_attach(vdev);
}

static inline QDF_STATUS monitor_vdev_detach(struct dp_vdev *vdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = vdev->pdev->soc->monitor_soc;

	if (!mon_soc)
		return QDF_STATUS_E_FAILURE;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_detach) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_vdev_detach(vdev);
}

static inline QDF_STATUS monitor_peer_attach(struct dp_soc *soc,
					     struct dp_peer *peer)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc)
		return QDF_STATUS_E_FAILURE;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_peer_attach) {
		qdf_print("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_peer_attach(peer);
}

static inline QDF_STATUS monitor_peer_detach(struct dp_soc *soc,
					     struct dp_peer *peer)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc)
		return QDF_STATUS_E_FAILURE;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_peer_detach) {
		qdf_print("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_peer_detach(peer);
}

static inline QDF_STATUS monitor_pdev_init(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	/*
	 * mon_soc uninitialized modular support enabled
	 * monitor related attach/detach/init/deinit
	 * will be done while monitor insmod
	 */
	if (!mon_soc)
		return QDF_STATUS_SUCCESS;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_init) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_pdev_init(pdev);
}

static inline QDF_STATUS monitor_pdev_deinit(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	/*
	 * mon_soc uninitialized modular support enabled
	 * monitor related attach/detach/init/deinit
	 * will be done while monitor insmod
	 */
	if (!mon_soc)
		return QDF_STATUS_SUCCESS;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_deinit) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_pdev_deinit(pdev);
}

static inline QDF_STATUS monitor_soc_cfg_init(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	/*
	 * this API is getting call from dp_soc_init,
	 * mon_soc will be uninitialized for WIN here
	 * So returning QDF_STATUS_SUCCESS.
	 * For WIN, soc cfg init is done while monitor insmod.
	 */
	if (!mon_soc)
		return QDF_STATUS_SUCCESS;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_soc_cfg_init) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_soc_cfg_init(soc);
}

static inline QDF_STATUS monitor_config_debug_sniffer(struct dp_pdev *pdev,
						      int val)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc)
		return QDF_STATUS_E_FAILURE;

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_config_debug_sniffer) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_config_debug_sniffer(pdev, val);
}

static inline void monitor_flush_rings(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_flush_rings) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_flush_rings(soc);
}

#if !defined(DISABLE_MON_CONFIG)
static inline QDF_STATUS monitor_htt_srng_setup(struct dp_soc *soc,
						struct dp_pdev *pdev,
						int mac_id,
						int mac_for_pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_SUCCESS;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_htt_srng_setup) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_htt_srng_setup(soc, pdev, mac_id,
					       mac_for_pdev);
}
#else
static inline QDF_STATUS monitor_htt_srng_setup(struct dp_soc *soc,
						struct dp_pdev *pdev,
						int mac_id,
						int mac_for_pdev)
{
	return QDF_STATUS_SUCCESS;
}
#endif

#if defined(DP_CON_MON)
static inline void monitor_service_mon_rings(struct dp_soc *soc, uint32_t quota)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_service_rings) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_service_rings(soc, quota);
}
#endif

#ifndef DISABLE_MON_CONFIG
static inline
uint32_t monitor_process(struct dp_soc *soc, struct dp_intr *int_ctx,
			 uint32_t mac_id, uint32_t quota)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return 0;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_process) {
		qdf_err("callback not registered");
		return 0;
	}

	return monitor_ops->mon_process(soc, int_ctx, mac_id, quota);
}
#else
static inline
uint32_t monitor_process(struct dp_soc *soc, struct dp_intr *int_ctx,
			 uint32_t mac_id, uint32_t quota)
{
	return 0;
}
#endif

#if !defined(DISABLE_MON_CONFIG) && defined(MON_ENABLE_DROP_FOR_MAC)
static inline
uint32_t monitor_drop_packets_for_mac(struct dp_pdev *pdev,
				      uint32_t mac_id, uint32_t quota)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return 0;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_drop_packets_for_mac) {
		qdf_err("callback not registered");
		return 0;
	}

	return monitor_ops->mon_drop_packets_for_mac(pdev,
						     mac_id, quota);
}
#else
static inline
uint32_t monitor_drop_packets_for_mac(struct dp_pdev *pdev,
				      uint32_t mac_id, uint32_t quota)
{
	return 0;
}
#endif

static inline void monitor_peer_tx_init(struct dp_pdev *pdev,
					struct dp_peer *peer)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_peer_tx_init) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_peer_tx_init(pdev, peer);
}

static inline void monitor_peer_tx_cleanup(struct dp_vdev *vdev,
					   struct dp_peer *peer)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = vdev->pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_peer_tx_cleanup) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_peer_tx_cleanup(vdev, peer);
}

#ifdef WLAN_TX_PKT_CAPTURE_ENH
static inline
void monitor_peer_tid_peer_id_update(struct dp_soc *soc,
				     struct dp_peer *peer,
				     uint16_t peer_id)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_peer_tid_peer_id_update) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_peer_tid_peer_id_update(peer, peer_id);
}

static inline void monitor_tx_ppdu_stats_attach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_tx_ppdu_stats_attach) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_tx_ppdu_stats_attach(pdev);
}

static inline void monitor_tx_ppdu_stats_detach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_tx_ppdu_stats_detach) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_tx_ppdu_stats_detach(pdev);
}

static inline QDF_STATUS monitor_tx_capture_debugfs_init(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_E_FAILURE;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_tx_capture_debugfs_init) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_tx_capture_debugfs_init(pdev);
}

static inline void monitor_peer_tx_capture_filter_check(struct dp_pdev *pdev,
							struct dp_peer *peer)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_peer_tx_capture_filter_check) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_peer_tx_capture_filter_check(pdev, peer);
}

static inline
QDF_STATUS monitor_tx_add_to_comp_queue(struct dp_soc *soc,
					struct dp_tx_desc_s *desc,
					struct hal_tx_completion_status *ts,
					struct dp_peer *peer)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_SUCCESS;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_tx_add_to_comp_queue) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_tx_add_to_comp_queue(soc, desc, ts, peer);
}

#else
static inline void monitor_peer_tid_peer_id_update(struct dp_soc *soc,
						   struct dp_peer *peer,
						   uint16_t peer_id)
{
}

static inline void monitor_tx_ppdu_stats_attach(struct dp_pdev *pdev)
{
}

static inline void monitor_tx_ppdu_stats_detach(struct dp_pdev *pdev)
{
}

static inline QDF_STATUS monitor_tx_capture_debugfs_init(struct dp_pdev *pdev)
{
	return QDF_STATUS_E_FAILURE;
}

static inline void monitor_peer_tx_capture_filter_check(struct dp_pdev *pdev,
							struct dp_peer *peer)
{
}

static inline
QDF_STATUS monitor_tx_add_to_comp_queue(struct dp_soc *soc,
					struct dp_tx_desc_s *desc,
					struct hal_tx_completion_status *ts,
					struct dp_peer *peer)
{
	return QDF_STATUS_E_FAILURE;
}

#endif

#if defined(WDI_EVENT_ENABLE) &&\
	(defined(QCA_ENHANCED_STATS_SUPPORT) || !defined(REMOVE_PKT_LOG))
static inline bool monitor_ppdu_stats_ind_handler(struct htt_soc *soc,
						  uint32_t *msg_word,
						  qdf_nbuf_t htt_t2h_msg)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->dp_soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return true;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_ppdu_stats_ind_handler) {
		qdf_err("callback not registered");
		return true;
	}

	return monitor_ops->mon_ppdu_stats_ind_handler(soc, msg_word,
						       htt_t2h_msg);
}
#else
static inline bool monitor_ppdu_stats_ind_handler(struct htt_soc *soc,
						  uint32_t *msg_word,
						  qdf_nbuf_t htt_t2h_msg)
{
	return true;
}
#endif

static inline QDF_STATUS monitor_htt_ppdu_stats_attach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_SUCCESS;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_htt_ppdu_stats_attach) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_htt_ppdu_stats_attach(pdev);
}

static inline void monitor_htt_ppdu_stats_detach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_htt_ppdu_stats_detach) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_htt_ppdu_stats_detach(pdev);
}

static inline void monitor_print_pdev_rx_mon_stats(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_print_pdev_rx_mon_stats) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_print_pdev_rx_mon_stats(pdev);
}

#ifdef WLAN_TX_PKT_CAPTURE_ENH
static inline void monitor_print_pdev_tx_capture_stats(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_print_pdev_tx_capture_stats) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_print_pdev_tx_capture_stats(pdev);
}

static inline QDF_STATUS monitor_config_enh_tx_capture(struct dp_pdev *pdev,
						       uint32_t val)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_E_FAILURE;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_config_enh_tx_capture) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_config_enh_tx_capture(pdev, val);
}
#else
static inline void monitor_print_pdev_tx_capture_stats(struct dp_pdev *pdev)
{
}

static inline QDF_STATUS monitor_config_enh_tx_capture(struct dp_pdev *pdev,
						       uint32_t val)
{
	return QDF_STATUS_E_INVAL;
}
#endif

#ifdef WLAN_RX_PKT_CAPTURE_ENH
static inline QDF_STATUS monitor_config_enh_rx_capture(struct dp_pdev *pdev,
						       uint32_t val)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_E_FAILURE;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_config_enh_rx_capture) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_config_enh_rx_capture(pdev, val);
}
#else
static inline QDF_STATUS monitor_config_enh_rx_capture(struct dp_pdev *pdev,
						       uint32_t val)
{
	return QDF_STATUS_E_INVAL;
}
#endif

#ifdef QCA_SUPPORT_BPR
static inline QDF_STATUS monitor_set_bpr_enable(struct dp_pdev *pdev,
						uint32_t val)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_E_FAILURE;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_set_bpr_enable) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_set_bpr_enable(pdev, val);
}
#else
static inline QDF_STATUS monitor_set_bpr_enable(struct dp_pdev *pdev,
						uint32_t val)
{
	return QDF_STATUS_E_FAILURE;
}
#endif

#ifdef ATH_SUPPORT_NAC
static inline int monitor_set_filter_neigh_peers(struct dp_pdev *pdev, bool val)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return 0;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_set_filter_neigh_peers) {
		qdf_err("callback not registered");
		return 0;
	}

	return monitor_ops->mon_set_filter_neigh_peers(pdev, val);
}
#else
static inline int monitor_set_filter_neigh_peers(struct dp_pdev *pdev, bool val)
{
	return 0;
}
#endif

#ifdef WLAN_ATF_ENABLE
static inline
void monitor_set_atf_stats_enable(struct dp_pdev *pdev, bool value)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_set_atf_stats_enable) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_set_atf_stats_enable(pdev, value);
}
#else
static inline
void monitor_set_atf_stats_enable(struct dp_pdev *pdev, bool value)
{
}
#endif

static inline
void monitor_set_bsscolor(struct dp_pdev *pdev, uint8_t bsscolor)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_set_bsscolor) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_set_bsscolor(pdev, bsscolor);
}

static inline
bool monitor_pdev_get_filter_mcast_data(struct cdp_pdev *pdev_handle)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_pdev *pdev = (struct dp_pdev *)pdev_handle;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return false;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_get_filter_mcast_data) {
		qdf_err("callback not registered");
		return false;
	}

	return monitor_ops->mon_pdev_get_filter_mcast_data(pdev_handle);
}

static inline
bool monitor_pdev_get_filter_non_data(struct cdp_pdev *pdev_handle)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_pdev *pdev = (struct dp_pdev *)pdev_handle;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return false;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_get_filter_non_data) {
		qdf_err("callback not registered");
		return false;
	}

	return monitor_ops->mon_pdev_get_filter_non_data(pdev_handle);
}

static inline
bool monitor_pdev_get_filter_ucast_data(struct cdp_pdev *pdev_handle)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_pdev *pdev = (struct dp_pdev *)pdev_handle;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return false;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pdev_get_filter_ucast_data) {
		qdf_err("callback not registered");
		return false;
	}

	return monitor_ops->mon_pdev_get_filter_ucast_data(pdev_handle);
}

#ifdef WDI_EVENT_ENABLE
static inline
int monitor_set_pktlog_wifi3(struct dp_pdev *pdev, uint32_t event, bool enable)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return 0;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_set_pktlog_wifi3) {
		qdf_err("callback not registered");
		return 0;
	}

	return monitor_ops->mon_set_pktlog_wifi3(pdev, event, enable);
}
#else
static inline int monitor_set_pktlog_wifi3(struct dp_pdev *pdev, uint32_t event,
					   bool enable)
{
	return 0;
}
#endif

#if defined(DP_CON_MON) && !defined(REMOVE_PKT_LOG)
static inline void monitor_pktlogmod_exit(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_pktlogmod_exit) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_pktlogmod_exit(pdev);
}
#else
static inline void monitor_pktlogmod_exit(struct dp_pdev *pdev) {}
#endif

static inline
void monitor_vdev_set_monitor_mode_buf_rings(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_set_monitor_mode_buf_rings) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_vdev_set_monitor_mode_buf_rings(pdev);
}

static inline
void monitor_neighbour_peers_detach(struct dp_pdev *pdev)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_neighbour_peers_detach) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_neighbour_peers_detach(pdev);
}

#ifdef FEATURE_NAC_RSSI
static inline QDF_STATUS monitor_filter_neighbour_peer(struct dp_pdev *pdev,
						       uint8_t *rx_pkt_hdr)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_E_FAILURE;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_filter_neighbour_peer) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_filter_neighbour_peer(pdev, rx_pkt_hdr);
}
#endif

static inline
void monitor_reap_timer_init(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_reap_timer_init) {
		qdf_err("callback not registered");
		return;
	}

	monitor_ops->mon_reap_timer_init(soc);
}

static inline
void monitor_reap_timer_deinit(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_reap_timer_deinit) {
		qdf_err("callback not registered");
		return;
	}

	monitor_ops->mon_reap_timer_deinit(soc);
}

static inline
void monitor_reap_timer_start(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_reap_timer_start) {
		qdf_err("callback not registered");
		return;
	}

	monitor_ops->mon_reap_timer_start(soc);
}

static inline
bool monitor_reap_timer_stop(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return false;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_reap_timer_stop) {
		qdf_err("callback not registered");
		return false;
	}

	return monitor_ops->mon_reap_timer_stop(soc);
}

static inline
void monitor_vdev_timer_init(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_timer_init) {
		qdf_err("callback not registered");
		return;
	}

	monitor_ops->mon_vdev_timer_init(soc);
}

static inline
void monitor_vdev_timer_deinit(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_timer_deinit) {
		qdf_err("callback not registered");
		return;
	}

	monitor_ops->mon_vdev_timer_deinit(soc);
}

static inline
void monitor_vdev_timer_start(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_timer_start) {
		qdf_err("callback not registered");
		return;
	}

	monitor_ops->mon_vdev_timer_start(soc);
}

static inline
bool monitor_vdev_timer_stop(struct dp_soc *soc)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return false;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_vdev_timer_stop) {
		qdf_err("callback not registered");
		return false;
	}

	return monitor_ops->mon_vdev_timer_stop(soc);
}

#ifdef QCA_MCOPY_SUPPORT
static inline
QDF_STATUS monitor_mcopy_check_deliver(struct dp_pdev *pdev,
				       uint16_t peer_id, uint32_t ppdu_id,
				       uint8_t first_msdu)
{
	struct dp_mon_ops *monitor_ops;
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return QDF_STATUS_E_FAILURE;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_mcopy_check_deliver) {
		qdf_err("callback not registered");
		return QDF_STATUS_E_FAILURE;
	}

	return monitor_ops->mon_mcopy_check_deliver(pdev, peer_id,
						    ppdu_id, first_msdu);
}
#else
static inline
QDF_STATUS monitor_mcopy_check_deliver(struct dp_pdev *pdev,
				       uint16_t peer_id, uint32_t ppdu_id,
				       uint8_t first_msdu)
{
	return QDF_STATUS_SUCCESS;
}
#endif

static inline void monitor_neighbour_peer_add_ast(struct dp_pdev *pdev,
						  struct dp_peer *ta_peer,
						  uint8_t *mac_addr,
						  qdf_nbuf_t nbuf,
						  uint32_t flags)
{
	struct dp_mon_soc *mon_soc = pdev->soc->monitor_soc;
	struct dp_mon_ops *monitor_ops;

	if (!mon_soc) {
		qdf_err("monitor soc is NULL");
		return;
	}

	monitor_ops = mon_soc->mon_ops;
	if (!monitor_ops || !monitor_ops->mon_neighbour_peer_add_ast) {
		qdf_err("callback not registered");
		return;
	}

	return monitor_ops->mon_neighbour_peer_add_ast(pdev, ta_peer, mac_addr,
						       nbuf, flags);
}

static inline void monitor_vdev_delete(struct dp_soc *soc, struct dp_vdev *vdev)
{
	if (soc->intr_mode == DP_INTR_POLL) {
		qdf_timer_sync_cancel(&soc->int_timer);
		monitor_flush_rings(soc);
	} else if (soc->intr_mode == DP_INTR_MSI) {
		if (monitor_vdev_timer_stop(soc))
			monitor_flush_rings(soc);
	}

	monitor_vdev_detach(vdev);
}

#ifdef DP_POWER_SAVE
/*
 * monitor_pktlog_reap_pending_frames() - reap pending frames
 * @pdev: point to dp pdev
 *
 * Return: void
 */
static inline void monitor_pktlog_reap_pending_frames(struct dp_pdev *pdev)
{
	struct dp_soc *soc;

	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	soc = pdev->soc;

	if (((pdev->monitor_pdev->rx_pktlog_mode != DP_RX_PKTLOG_DISABLED) ||
	     monitor_is_enable_reap_timer_non_pkt(pdev))) {
		if (monitor_reap_timer_stop(soc))
			monitor_service_mon_rings(soc, DP_MON_REAP_BUDGET);
	}
}

/*
 * monitor_pktlog_start_reap_timer() - start reap timer
 * @pdev: point to dp pdev
 *
 * Return: void
 */
static inline void monitor_pktlog_start_reap_timer(struct dp_pdev *pdev)
{
	struct dp_soc *soc;

	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	soc = pdev->soc;
	if (((pdev->monitor_pdev->rx_pktlog_mode != DP_RX_PKTLOG_DISABLED) ||
	     monitor_is_enable_reap_timer_non_pkt(pdev)))
		monitor_reap_timer_start(soc);
}
#endif

static inline
void monitor_neighbour_peer_list_remove(struct dp_pdev *pdev,
					struct dp_vdev *vdev,
					struct dp_neighbour_peer *peer)
{
	struct dp_mon_pdev *mon_pdev;
	struct dp_neighbour_peer *temp_peer = NULL;

	if (qdf_unlikely(!pdev || !pdev->monitor_pdev))
		return;

	mon_pdev = pdev->monitor_pdev;
	qdf_spin_lock_bh(&mon_pdev->neighbour_peer_mutex);
	if (!pdev->soc->monitor_soc->hw_nac_monitor_support) {
		TAILQ_FOREACH(peer, &mon_pdev->neighbour_peers_list,
			      neighbour_peer_list_elem) {
				QDF_ASSERT(peer->vdev != vdev);
			}
	} else {
		TAILQ_FOREACH_SAFE(peer, &mon_pdev->neighbour_peers_list,
				   neighbour_peer_list_elem, temp_peer) {
			if (peer->vdev == vdev) {
				TAILQ_REMOVE(&mon_pdev->neighbour_peers_list,
					     peer,
					     neighbour_peer_list_elem);
				qdf_mem_free(peer);
			}
		}
	}
	qdf_spin_unlock_bh(&mon_pdev->neighbour_peer_mutex);
}

static inline
void monitor_pdev_set_mon_vdev(struct dp_vdev *vdev)
{
	struct dp_mon_pdev *mon_pdev = vdev->pdev->monitor_pdev;

	if (!mon_pdev)
		return;

	mon_pdev->mvdev = vdev;
}

QDF_STATUS dp_mon_soc_attach(struct dp_soc *soc);
QDF_STATUS dp_mon_soc_detach(struct dp_soc *soc);
QDF_STATUS dp_mon_pdev_attach(struct dp_pdev *pdev);
QDF_STATUS dp_mon_pdev_detach(struct dp_pdev *pdev);
QDF_STATUS dp_mon_pdev_init(struct dp_pdev *pdev);
QDF_STATUS dp_mon_pdev_deinit(struct dp_pdev *pdev);
QDF_STATUS dp_mon_soc_cfg_init(struct dp_soc *soc);
void dp_mon_cdp_ops_register(struct dp_soc *soc);
void dp_mon_cdp_ops_deregister(struct dp_soc *soc);
void dp_mon_ops_register(struct dp_mon_soc *mon_soc);
QDF_STATUS dp_mon_htt_srng_setup(struct dp_soc *soc,
				 struct dp_pdev *pdev,
				 int mac_id,
				 int mac_for_pdev);
#ifdef QCA_ENHANCED_STATS_SUPPORT
QDF_STATUS dp_peer_qos_stats_notify(struct dp_pdev *dp_pdev,
				    struct cdp_rx_stats_ppdu_user *ppdu_user);
#endif

#endif /* _DP_MON_H_ */