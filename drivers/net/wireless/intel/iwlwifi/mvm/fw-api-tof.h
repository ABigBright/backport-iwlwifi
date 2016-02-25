/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2015 - 2016 Intel Deutschland GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110,
 * USA
 *
 * The full GNU General Public License is included in this distribution
 * in the file called COPYING.
 *
 * Contact Information:
 * Intel Linux Wireless <linuxwifi@intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 * BSD LICENSE
 *
 * Copyright(c) 2015 - 2016 Intel Deutschland GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#ifndef __fw_api_tof_h__
#define __fw_api_tof_h__

#include "fw-api.h"

/* ToF sub-group command IDs */
enum iwl_mvm_tof_sub_grp_ids {
	TOF_RANGE_REQ_CMD = 0x1,
	TOF_CONFIG_CMD = 0x2,
	TOF_RANGE_ABORT_CMD = 0x3,
	TOF_RANGE_REQ_EXT_CMD = 0x4,
	TOF_RESPONDER_CONFIG_CMD = 0x5,
	TOF_NW_INITIATED_RES_SEND_CMD = 0x6,
	TOF_NEIGHBOR_REPORT_REQ_CMD = 0x7,
	TOF_RESPONDER_DYN_CONFIG_CMD = 0x8,
	TOF_RESPONDER_STATS = 0x9,
	TOF_NEIGHBOR_REPORT_RSP_NOTIF = 0xFC,
	TOF_NW_INITIATED_REQ_RCVD_NOTIF = 0xFD,
	TOF_RANGE_RESPONSE_NOTIF = 0xFE,
	TOF_MCSI_DEBUG_NOTIF = 0xFB,
};

/**
 * struct iwl_tof_config_cmd - ToF configuration
 * @tof_disabled: 0 enabled, 1 - disabled
 * @one_sided_disabled: 0 enabled, 1 - disabled
 * @is_debug_mode: 1 debug mode, 0 - otherwise
 * @is_buf_required: 1 channel estimation buffer required, 0 - otherwise
 */
struct iwl_tof_config_cmd {
	__le32 sub_grp_cmd_id;
	u8 tof_disabled;
	u8 one_sided_disabled;
	u8 is_debug_mode;
	u8 is_buf_required;
} __packed;

/**
 * enum iwl_tof_bandwidth - values for iwl_tof_range_req_ap_entry.bandwidth
 */
enum iwl_tof_bandwidth {
	IWL_TOF_BW_20_LEGACY,
	IWL_TOF_BW_20_HT,
	IWL_TOF_BW_40,
	IWL_TOF_BW_80,
};

/*
 * enum iwl_tof_algo_type - Algorithym type for range measurement request
 */
enum iwl_tof_algo_type {
	IWL_TOF_ALGO_TYPE_MAX_LIKE	= 0,
	IWL_TOF_ALGO_TYPE_LINEAR_REG	= 1,

	/* Keep last */
	IWL_TOF_ALGO_TYPE_INVALID,
}; /* ALGO_TYPE_E */

/**
 * struct iwl_tof_responder_config_cmd - ToF AP mode (for debug)
 * @burst_period: future use: (currently hard coded in the LMAC)
 *		  The interval between two sequential bursts.
 * @min_delta_ftm: future use: (currently hard coded in the LMAC)
 *		   The minimum delay between two sequential FTM Responses
 *		   in the same burst.
 * @burst_duration: future use: (currently hard coded in the LMAC)
 *		   The total time for all FTMs handshake in the same burst.
 *		   Affect the time events duration in the LMAC.
 * @num_of_burst_exp: future use: (currently hard coded in the LMAC)
 *		   The number of bursts for the current ToF request. Affect
 *		   the number of events allocations in the current iteration.
 * @get_ch_est: for xVT only, NA for driver
 * @abort_responder: when set to '1' - Responder will terminate its activity
 *		     (all other fields in the command are ignored)
 * @recv_sta_req_params: 1 - Responder will ignore the other Responder's
 *			 params and use the recomended Initiator params.
 *			 0 - otherwise
 * @channel_num: current AP Channel
 * @bandwidth: current AP Bandwidth: &enum iwl_tof_bandwidth
 * @rate: current AP rate
 * @ctrl_ch_position: coding of the control channel position relative to
 *	     the center frequency.
 *	     40MHz  0 below center, 1 above center
 *	     80MHz  bits [0..1]: 0  the near 20MHz to the center,
 *				 1  the far  20MHz to the center
 *		    bit[2]  as above 40MHz
 * @ftm_per_burst: FTMs per Burst
 * @ftm_resp_ts_avail: '0' - we don't measure over the Initial FTM Response,
 *		  '1' - we measure over the Initial FTM Response
 * @asap_mode: ASAP / Non ASAP mode for the current WLS station
 * @sta_id: index of the AP STA when in AP mode
 * @tsf_timer_offset_msecs: The dictated time offset (mSec) from the AP's TSF
 * @toa_offset: Artificial addition [pSec] for the ToA - to be used for debug
 *		purposes, simulating station movement by adding various values
 *		to this field
 * @bssid: Current AP BSSID
 * @algo_type: &enum iwl_tof_algo_type
 * @reserved: For alignment and future use
 */
struct iwl_tof_responder_config_cmd {
	__le32 sub_grp_cmd_id;
	__le16 burst_period;
	u8 min_delta_ftm;
	u8 burst_duration;
	u8 num_of_burst_exp;
	u8 get_ch_est;
	u8 abort_responder;
	u8 recv_sta_req_params;
	u8 channel_num;
	u8 bandwidth;
	u8 rate;
	u8 ctrl_ch_position;
	u8 ftm_per_burst;
	u8 ftm_resp_ts_avail;
	u8 asap_mode;
	u8 sta_id;
	__le16 tsf_timer_offset_msecs;
	__le16 toa_offset;
	u8 bssid[ETH_ALEN];
	u8 algo_type;
	u8 reserved;
} __packed; /* TOF_RESPONDER_CONFIG_CMD_API_S_VER_4 */

/**
 * struct iwl_tof_responder_dyn_config_cmd - Dynamic responder settings
 * @lci_len: The length of the 1st (LCI) part in the @lci_civic buffer
 * @civic_len: The length of the 2nd (CIVIC) part in the @lci_civic buffer
 * @lci_civic: The LCI/CIVIC buffer. LCI data (if exists) comes first, then, if
 *	needed, 0-padding such that the next part is dword-aligned, then CIVIC
 *	data (if exists) follows, and then 0-padding again to complete a
 *	4-multiple long buffer.
 */
struct iwl_tof_responder_dyn_config_cmd {
	__le32 sub_grp_cmd_id;
	__le32 lci_len;
	__le32 civic_len;
	u8 lci_civic[];
} __packed;

/**
 * struct iwl_tof_range_request_ext_cmd - extended range req for WLS
 * @tsf_timer_offset_msec: the recommended time offset (mSec) from the AP's TSF
 * @min_delta_ftm: Minimal time between two consecutive measurements,
 *		   in units of 100us. 0 means no preference by station
 * @ftm_format_and_bw20M: FTM Channel Spacing/Format for 20MHz: recommended
 *			value be sent to the AP
 * @ftm_format_and_bw40M: FTM Channel Spacing/Format for 40MHz: recommended
 *			value to be sent to the AP
 * @ftm_format_and_bw80M: FTM Channel Spacing/Format for 80MHz: recommended
 *			value to be sent to the AP
 */
struct iwl_tof_range_req_ext_cmd {
	__le32 sub_grp_cmd_id;
	__le16 tsf_timer_offset_msec;
	__le16 reserved;
	u8 min_delta_ftm;
	u8 ftm_format_and_bw20M;
	u8 ftm_format_and_bw40M;
	u8 ftm_format_and_bw80M;
} __packed;

#define IWL_MVM_TOF_MAX_APS 21
#define IWL_MVM_TOF_MAX_TWO_SIDED_APS 5

/**
 * enum iwl_tof_location_query - values for query bitmap
 */
enum iwl_tof_location_query {
	IWL_TOF_LOC_LCI = 0x01,
	IWL_TOF_LOC_CIVIC = 0x02,
};

 /**
 * struct iwl_tof_range_req_ap_entry - AP configuration parameters
 * @channel_num: Current AP Channel
 * @bandwidth: Current AP Bandwidth. One of iwl_tof_bandwidth.
 * @tsf_delta_direction: TSF relatively to the subject AP
 * @ctrl_ch_position: Coding of the control channel position relative to the
 *	     center frequency.
 *	     40MHz  0 below center, 1 above center
 *	     80MHz  bits [0..1]: 0  the near 20MHz to the center,
 *				 1  the far  20MHz to the center
 *		    bit[2]  as above 40MHz
 * @bssid: AP's bss id
 * @measure_type: Measurement type: 0 - two sided, 1 - One sided
 * @num_of_bursts: Recommended value to be sent to the AP.  2s Exponent of the
 *		   number of measurement iterations (min 2^0 = 1, max 2^14)
 * @burst_period: Recommended value to be sent to the AP. Measurement
 *		  periodicity In units of 100ms. ignored if num_of_bursts = 0
 * @samples_per_burst: 2-sided: the number of FTMs pairs in single Burst (1-31)
 *		       1-sided: how many rts/cts pairs should be used per burst.
 * @retries_per_sample: Max number of retries that the LMAC should send
 *			in case of no replies by the AP.
 * @tsf_delta: TSF Delta in units of microseconds.
 *	       The difference between the AP TSF and the device local clock.
 * @location_req: Location Request Bit[0] LCI should be sent in the FTMR
 *			      Bit[1] Civic should be sent in the FTMR
 * @asap_mode: 0 - non asap mode, 1 - asap mode (not relevant for one sided)
 * @enable_dyn_ack: Enable Dynamic ACK BW.
 *	    0  Initiator interact with regular AP
 *	    1  Initiator interact with Responder machine: need to send the
 *	    Initiator Acks with HT 40MHz / 80MHz, since the Responder should
 *	    use it for its ch est measurement (this flag will be set when we
 *	    configure the opposite machine to be Responder).
 * @rssi: Last received value
 *	  leagal values: -128-0 (0x7f). above 0x0 indicating an invalid value.
 * @algo_type: &enum iwl_tof_algo_type
 * @reserved: For alignment and future use
 */
struct iwl_tof_range_req_ap_entry {
	u8 channel_num;
	u8 bandwidth;
	u8 tsf_delta_direction;
	u8 ctrl_ch_position;
	u8 bssid[ETH_ALEN];
	u8 measure_type;
	u8 num_of_bursts;
	__le16 burst_period;
	u8 samples_per_burst;
	u8 retries_per_sample;
	__le32 tsf_delta;
	u8 location_req;
	u8 asap_mode;
	u8 enable_dyn_ack;
	s8 rssi;
	u8 algo_type;
	u8 reserved[3];
} __packed; /* LOCATION_RANGE_REQ_AP_ENTRY_CMD_API_S_VER_2 */

/**
 * enum iwl_tof_response_mode
 * @IWL_MVM_TOF_RESPONSE_ASAP: report each AP measurement separately as soon as
 *			       possible (not supported for this release)
 * @IWL_MVM_TOF_RESPONSE_TIMEOUT: report all AP measurements as a batch upon
 *				  timeout expiration
 * @IWL_MVM_TOF_RESPONSE_COMPLETE: report all AP measurements as a batch at the
 *				   earlier of: measurements completion / timeout
 *				   expiration.
 */
enum iwl_tof_response_mode {
	IWL_MVM_TOF_RESPONSE_ASAP,
	IWL_MVM_TOF_RESPONSE_TIMEOUT,
	IWL_MVM_TOF_RESPONSE_COMPLETE,
};

/**
 * struct iwl_tof_range_req_cmd - start measurement cmd
 * @request_id: A Token incremented per request. The same Token will be
 *		sent back in the range response
 * @initiator: 0- NW initiated,  1 - Client Initiated
 * @one_sided_los_disable: '0'- run ML-Algo for both ToF/OneSided,
 *			   '1' - run ML-Algo for ToF only
 * @req_timeout: Requested timeout of the response in units of 100ms.
 *	     This is equivalent to the session time configured to the
 *	     LMAC in Initiator Request
 * @report_policy: Supported partially for this release: For current release -
 *		   the range report will be uploaded as a batch when ready or
 *		   when the session is done (successfully / partially).
 *		   one of iwl_tof_response_mode.
 * @num_of_ap: Number of APs to measure (error if > IWL_MVM_TOF_MAX_APS)
 * @macaddr_random: '0' Use default source MAC address (i.e. p2_p),
 *	            '1' Use MAC Address randomization according to the below
 * @macaddr_mask: Bits set to 0 shall be copied from the MAC address template.
 *		  Bits set to 1 shall be randomized by the UMAC
 */
struct iwl_tof_range_req_cmd {
	__le32 sub_grp_cmd_id;
	u8 request_id;
	u8 initiator;
	u8 one_sided_los_disable;
	u8 req_timeout;
	u8 report_policy;
	u8 los_det_disable;
	u8 num_of_ap;
	u8 macaddr_random;
	u8 range_req_bssid[ETH_ALEN];
	u8 macaddr_template[ETH_ALEN];
	u8 macaddr_mask[ETH_ALEN];
	u16 reserved;
	struct iwl_tof_range_req_ap_entry ap[IWL_MVM_TOF_MAX_APS];
} __packed;

/**
 * struct iwl_tof_gen_resp_cmd - generic ToF response
 */
struct iwl_tof_gen_resp_cmd {
	__le32 sub_grp_cmd_id;
	u8 data[];
} __packed;

/**
 * enum iwl_tof_entry_status
 *
 * @IWL_TOF_ENTRY_SUCCESS: successful measurement.
 * @IWL_TOF_ENTRY_NOT_MEASURED: not measured due to timeout/abort.
 * @IWL_TOF_ENTRY_UNAVAILABLE: peer is unavailable.
 * @IWL_TOF_ENTRY_PROTOCOL_ERR: fail due to protocol error.
 * @IWL_TOF_ENTRY_INTERNAL_ERR: fail due to internal error.
 * @IWL_TOF_ENTRY_INVALID: request received in an invalid state.
 */
enum iwl_tof_entry_status {
	IWL_TOF_ENTRY_SUCCESS,
	IWL_TOF_ENTRY_NOT_MEASURED,
	IWL_TOF_ENTRY_UNAVAILABLE,
	IWL_TOF_ENTRY_PROTOCOL_ERR,
	IWL_TOF_ENTRY_INTERNAL_ERR,
	IWL_TOF_ENTRY_INVALID = 0xff,
}; /* LOCATION_MEASUREMENT_STATUS */

/**
 * struct iwl_tof_range_rsp_ap_entry_ntfy - AP parameters (response)
 * @measure_status: current APs measurement status, one of
 *	%enum iwl_tof_entry_status.
 * @measure_bw: Current AP Bandwidth: 0  20MHz, 1  40MHz, 2  80MHz
 * @rtt: The Round Trip Time that took for the last measurement for
 *	 current AP [pSec]
 * @rtt_variance: The Variance of the RTT values measured for current AP
 * @rtt_spread: The Difference between the maximum and the minimum RTT
 *	       values measured for current AP in the current session [pSec]
 * @rssi: RSSI as uploaded in the Channel Estimation notification
 * @rssi_spread: The Difference between the maximum and the minimum RSSI values
 *	        measured for current AP in the current session
 * @range: Measured range [cm]
 * @range_variance: Measured range variance [cm]
 * @timestamp: The GP2 Clock [usec] where Channel Estimation notification was
 *	       uploaded by the LMAC
 */
struct iwl_tof_range_rsp_ap_entry_ntfy {
	u8 bssid[ETH_ALEN];
	u8 measure_status;
	u8 measure_bw;
	__le32 rtt;
	__le32 rtt_variance;
	__le32 rtt_spread;
	s8 rssi;
	u8 rssi_spread;
	__le16 reserved;
	__le32 range;
	__le32 range_variance;
	__le32 timestamp;
} __packed;

/**
 * enum iwl_tof_response_status - tof response status
 *
 * @IWL_TOF_RESPONSE_SUCCESS: successful response.
 * @IWL_TOF_RESPONSE_TIMEOUT: request aborted due to timeout expiration.
 *	partial result of ranges done so far is included in the response.
 * @IWL_TOF_RESPONSE_ABORTED: Aborted due to tof service unavailable.
 *	In case of one-sided, this also the status in case a nested request was
 *	sent.
 */
enum iwl_tof_response_status {
	IWL_TOF_RESPONSE_SUCCESS,
	IWL_TOF_RESPONSE_TIMEOUT,
	IWL_TOF_RESPONSE_ABORTED,
}; /* LOCATION_RNG_RSP_STATUS */

/**
 * struct iwl_tof_range_rsp_ntfy -
 * @request_id: A Token ID of the corresponding Range request
 * @request_status: status of current measurement session, one of
 *	@enum iwl_tof_response_status.
 * @last_in_batch: reprot policy (when not all responses are uploaded at once)
 * @num_of_aps: Number of APs to measure (error if > IWL_MVM_TOF_MAX_APS)
 */
struct iwl_tof_range_rsp_ntfy {
	u8 request_id;
	u8 request_status;
	u8 last_in_batch;
	u8 num_of_aps;
	struct iwl_tof_range_rsp_ap_entry_ntfy ap[IWL_MVM_TOF_MAX_APS];
} __packed;

#define IWL_MVM_TOF_MCSI_BUF_SIZE  (245)
/**
 * struct iwl_tof_mcsi_notif - used for debug
 * @token: token ID for the current session
 * @role: '0' - initiator, '1' - responder
 * @initiator_bssid: initiator machine
 * @responder_bssid: responder machine
 * @mcsi_buffer: debug data
 */
struct iwl_tof_mcsi_notif {
	u8 token;
	u8 role;
	__le16 reserved;
	u8 initiator_bssid[ETH_ALEN];
	u8 responder_bssid[ETH_ALEN];
	u8 mcsi_buffer[IWL_MVM_TOF_MCSI_BUF_SIZE * 4];
} __packed;

/**
 * struct iwl_tof_neighbor_report_notif
 * @bssid: BSSID of the AP which sent the report
 * @request_token: same token as the corresponding request
 * @status:
 * @report_ie_len: the length of the response frame starting from the Element ID
 * @data: the IEs
 */
struct iwl_tof_neighbor_report {
	u8 bssid[ETH_ALEN];
	u8 request_token;
	u8 status;
	__le16 report_ie_len;
	u8 data[];
} __packed;

/**
 * struct iwl_tof_range_abort_cmd
 * @request_id: corresponds to a range request
 */
struct iwl_tof_range_abort_cmd {
	__le32 sub_grp_cmd_id;
	u8 request_id;
	u8 reserved[3];
} __packed;

enum ftm_responder_stats_flags {
	FTM_RESP_STAT_NON_ASAP_STARTED = BIT(0),
	FTM_RESP_STAT_NON_ASAP_IN_WIN = BIT(1),
	FTM_RESP_STAT_NON_ASAP_OUT_WIN = BIT(2),
	FTM_RESP_STAT_TRIGGER_DUP = BIT(3),
	FTM_RESP_STAT_DUP = BIT(4),
	FTM_RESP_STAT_DUP_IN_WIN = BIT(5),
	FTM_RESP_STAT_DUP_OUT_WIN = BIT(6),
	FTM_RESP_STAT_SCHED_SUCCESS = BIT(7),
	FTM_RESP_STAT_ASAP_REQ = BIT(8),
	FTM_RESP_STAT_NON_ASAP_REQ = BIT(9),
	FTM_RESP_STAT_ASAP_RESP = BIT(10),
	FTM_RESP_STAT_NON_ASAP_RESP = BIT(11),
	FTM_RESP_STAT_FAIL_INITIATOR_INACTIVE = BIT(12),
	FTM_RESP_STAT_FAIL_INITIATOR_OUT_WIN = BIT(13),
	FTM_RESP_STAT_FAIL_INITIATOR_RETRY_LIM = BIT(14),
	FTM_RESP_STAT_FAIL_NEXT_SERVED = BIT(15),
	FTM_RESP_STAT_FAIL_TRIGGER_ERR = BIT(16),
	FTM_RESP_STAT_FAIL_GC = BIT(17),
	FTM_RESP_STAT_SUCCESS = BIT(18),
	FTM_RESP_STAT_INTEL_IE = BIT(19),
	FTM_RESP_STAT_TRIGGER_UNKNOWN = BIT(20),
	FTM_RESP_STAT_PROCESS_FAIL = BIT(21),
	FTM_RESP_STAT_ACK = BIT(22),
	FTM_RESP_STAT_NACK = BIT(23),
	FTM_RESP_STAT_INVALID_INITIATOR_ID = BIT(24),
	FTM_RESP_STAT_TIMER_MIN_DELTA = BIT(25),
	FTM_RESP_STAT_INITIATOR_REMOVED = BIT(26),
	FTM_RESP_STAT_INITIATOR_ADDED = BIT(27),
}; /* RESP_IND_E */

/**
 * struct iwl_tof_responder_stats - FTM responder statistics
 * @addr: initiator address
 * @success_ftm: number of successful ftm frames
 * @ftm_per_burst: num of FTM frames that were received
 * @flags: &enum ftm_responder_stats_flags
 * @duration: actual duration of FTM
 * @allocated_duration: time that was allocated for this FTM session
 * @bw: FTM request bandwidth
 * @rate: FTM request rate
 * @reserved: for alingment and future use
 */
struct iwl_tof_responder_stats {
	u8 addr[ETH_ALEN];
	u8 success_ftm;
	u8 ftm_per_burst;
	__le32 flags;
	__le32 duration;
	__le32 allocated_duration;
	u8 bw;
	u8 rate;
	__le16 reserved;
} __packed; /* TOF_RESPONDER_STATISTICS_NTFY_S_VER_1 */

#endif
