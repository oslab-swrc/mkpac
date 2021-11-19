/* SPDX-License-Identifier: GPL-2.0-only */
/* Portions Copyright (c) 2021 Electronics and Telecommunications Research Institute */

/*******************************************************************************
 *
 * This is an addon for Intel I40E Device Driver in order to support 
 * pre-allocated buffers in an attempt to optimize its run-time performance
 * especially to support near-wirespeed high Packet Frame Rates.  
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 *
 ******************************************************************************/

#ifndef _I40E_PREALLOC_H_
#define _I40E_PREALLOC_H_

#include "i40e_txrx.h"
//header file for pre-alloc APIs-mKPAC
typedef struct i40e_ring_rx_bi_prealloc
{   struct i40e_rx_buffer *rx_bi;
    int enable;
} i40e_ring_rx_bi_prealloc_t;

typedef struct i40e_ring_tx_bi_prealloc
{   struct i40e_tx_buffer *tx_bi;
    int enable;
} i40e_ring_tx_bi_prealloc_t;

#define I40E_RING_RX_BI_PREALLOC_MAX	2500
#define I40E_RING_TX_BI_PREALLOC_MAX	2500
#define I40E_DEBUG 0
extern i40e_ring_rx_bi_prealloc_t *i40e_ring_rx_bi_prealloc;
extern i40e_ring_tx_bi_prealloc_t *i40e_ring_tx_bi_prealloc;  
extern int i40e_rings_prealloc_status;


void i40e_rings_prealloc_init(void);
void i40e_rings_prealloc_uninit(void);

void i40e_ring_rx_bi_prealloc_free(struct i40e_ring *rx_ring);
void i40e_ring_rx_bi_prealloc_alloc(struct i40e_ring *rx_ring);

void i40e_ring_tx_bi_prealloc_free(struct i40e_ring *tx_ring);
void i40e_ring_tx_bi_prealloc_alloc(struct i40e_ring *tx_ring);

#endif /* _I40E_PREALLOC_H_ */

