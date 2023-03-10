/*
 * Copyright (c) 2019-2020 The Linux Foundation. All rights reserved.
 * Copyright (c) 2021-2022 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/**
 * DOC: Target interface file for denylist manager component to
 * Implement api's which shall be used by denylist manager component
 * in target if internally.
 */

#include <target_if_dlm.h>
#include "target_if.h"

#if defined(WLAN_FEATURE_ROAM_OFFLOAD)
QDF_STATUS
target_if_dlm_send_reject_ap_list(struct wlan_objmgr_pdev *pdev,
				  struct reject_ap_params *reject_params)
{
	struct wmi_unified *wmi_handle;

	wmi_handle = get_wmi_unified_hdl_from_pdev(pdev);
	if (!wmi_handle) {
		target_if_err("Invalid wmi handle");
		return QDF_STATUS_E_INVAL;
	}

	return wmi_unified_send_reject_ap_list(wmi_handle, reject_params);
}

void target_if_dlm_register_tx_ops(struct wlan_dlm_tx_ops *dlm_tx_ops)
{
	if (!dlm_tx_ops) {
		target_if_err("dlm_tx_ops is null");
		return;
	}

	dlm_tx_ops->dlm_send_reject_ap_list = target_if_dlm_send_reject_ap_list;
}
#endif
