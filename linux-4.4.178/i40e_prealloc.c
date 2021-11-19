// SPDX-License-Identifier: GPL-2.0-only
// Portions Copyright (c) 2021 Electronics and Telecommunications Research Institute

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

#include <linux/prefetch.h>
#include <net/busy_poll.h>
#include "i40e.h"
#include "i40e_prototype.h"
#include "i40e_txrx.h"
#include "i40e_prealloc.h"
//prealloc APIs-mKPAC
i40e_ring_rx_bi_prealloc_t *i40e_ring_rx_bi_prealloc=NULL;
i40e_ring_tx_bi_prealloc_t *i40e_ring_tx_bi_prealloc=NULL;
int i40e_rings_prealloc_status=0;

void i40e_rings_prealloc_init(void)
{   int i;
    int bi_size;
    
    ///if(I40E_DEBUG) { printk("API: i40e_rings_prealloc_init() \n"); }
    
    if(i40e_rings_prealloc_status)
    { ///if(I40E_DEBUG) { printk("i40e_rings_prealloc_status=%d - already alloc'd exiting ! \n", i40e_rings_prealloc_status); }
      return;
    } 
    

//RX-BI -------------    
    i40e_ring_rx_bi_prealloc = (i40e_ring_rx_bi_prealloc_t *)kmalloc(sizeof(i40e_ring_rx_bi_prealloc_t)*I40E_RING_RX_BI_PREALLOC_MAX, GFP_KERNEL);
	 if(i40e_ring_rx_bi_prealloc==NULL) { return; }
	 
    bi_size = sizeof(struct i40e_rx_buffer) * (1024*30);
    for (i = 0;i < I40E_RING_RX_BI_PREALLOC_MAX;i++)
    {   i40e_ring_rx_bi_prealloc[i].rx_bi=NULL;
    	  i40e_ring_rx_bi_prealloc[i].rx_bi = kzalloc(bi_size, GFP_KERNEL);
        
        if(i40e_ring_rx_bi_prealloc[i].rx_bi==NULL) { printk("Unable to allocate memory rx_bi [%i]\n", i); }
        else
        { 
          printk("i40e_ring_rx_bi_prealloc[%i].rx_bi - alloc'd\n", i);
        }
        
        i40e_ring_rx_bi_prealloc[i].enable=0; //init to 0 -> vacant !
    }
    
//TX-BI -------------
    i40e_ring_tx_bi_prealloc = (i40e_ring_tx_bi_prealloc_t *)kmalloc(sizeof(i40e_ring_tx_bi_prealloc_t)*I40E_RING_TX_BI_PREALLOC_MAX, GFP_KERNEL);
	 if(i40e_ring_tx_bi_prealloc==NULL) { return; }
        
    bi_size = sizeof(struct i40e_tx_buffer) * (1024*30);
    for (i = 0;i < I40E_RING_TX_BI_PREALLOC_MAX;i++)
    {   i40e_ring_tx_bi_prealloc[i].tx_bi=NULL;
    	  i40e_ring_tx_bi_prealloc[i].tx_bi = kzalloc(bi_size, GFP_KERNEL);
        
        if(i40e_ring_tx_bi_prealloc[i].tx_bi==NULL) {} //printk("Unable to allocate memory tx_bi [%i]\n",i);}
        else
        {    
            printk("i40e_ring_tx_bi_prealloc[%i].tx_bi - alloc'd\n", i);
        }
        
        i40e_ring_tx_bi_prealloc[i].enable=0; //init to 0 -> vacant !
    }
    
    
    i40e_rings_prealloc_status=1; //alloc done !
    //printk("i40e_rings_prealloc_status=%d - alloc complete ! \n", i40e_rings_prealloc_status);
}

void i40e_rings_prealloc_uninit(void)
{   int i;
    int bi_size;
    
    ///if(I40E_DEBUG) { printk("API: i40e_rings_prealloc_uninit() \n"); }
    
    if(i40e_rings_prealloc_status)
    { if(I40E_DEBUG) { printk("i40e_rings_prealloc_status=%d - already alloc'd exiting ! \n", i40e_rings_prealloc_status); }
      return;
    } 
    
        
    bi_size = sizeof(struct i40e_rx_buffer) * (4096*3);
    for (i = 0;i < I40E_RING_RX_BI_PREALLOC_MAX;i++)
    {   i40e_ring_rx_bi_prealloc[i].rx_bi=NULL;
    	  i40e_ring_rx_bi_prealloc[i].rx_bi = kzalloc(bi_size, GFP_KERNEL);
        
        if(!i40e_ring_rx_bi_prealloc[i].rx_bi) { printk("Unable to allocate memory rx_bi [%i]\n", i); }
        else
        {    
            printk("i40e_ring_rx_bi_prealloc[%i].rx_bi - alloc'd\n", i);
        }
        
        i40e_ring_rx_bi_prealloc[i].enable=0; //init to 0 -> vacant !
    }
    
    
    i40e_rings_prealloc_status=1; //alloc done !
    //printk("i40e_rings_prealloc_status=%d - alloc complete ! \n", i40e_rings_prealloc_status);
}


void i40e_ring_rx_bi_prealloc_free(struct i40e_ring *rx_ring)
{	if(rx_ring->rx_bi)
   { i40e_ring_rx_bi_prealloc[rx_ring->rx_bi_prealloc_index].enable=0; //free this pre-alloc slot !
     rx_ring->rx_bi = NULL;
   }
}

void i40e_ring_rx_bi_prealloc_alloc(struct i40e_ring *rx_ring)
{	int i;
	int bi_size;
	bi_size = sizeof(struct i40e_rx_buffer) * rx_ring->count;
	 
	if(rx_ring->rx_bi)
   { memset(rx_ring->rx_bi, 0x00, bi_size); //ZERO the pre-alloc buf before use !
     return;
   }	 
	 
	 //if rx_ring->rx_bi == NULL
    for(i=0;i<I40E_RING_RX_BI_PREALLOC_MAX;i++)
    {  if(!i40e_ring_rx_bi_prealloc[i].enable)
	    {   i40e_ring_rx_bi_prealloc[i].enable=1;
	    	  rx_ring->rx_bi = i40e_ring_rx_bi_prealloc[i].rx_bi;
	    	  memset(rx_ring->rx_bi, 0x00, bi_size); //ZERO the pre-alloc buf before use !
      	  rx_ring->rx_bi_prealloc_index = i; //store the prealloc buf slot/index
           ///if(I40E_DEBUG) { printk("setup: i40e_ring_rx_bi_prealloc[i].enable = %d\n", i); }
           return;
       }
    }
}


void i40e_ring_tx_bi_prealloc_free(struct i40e_ring *tx_ring)
{	if(tx_ring->tx_bi)
   { i40e_ring_tx_bi_prealloc[tx_ring->tx_bi_prealloc_index].enable=0; //free this pre-alloc slot !
     tx_ring->tx_bi = NULL;
   }
}


void i40e_ring_tx_bi_prealloc_alloc(struct i40e_ring *tx_ring)
{	int i;
	int bi_size;
	bi_size = sizeof(struct i40e_tx_buffer) * tx_ring->count;
	 
	if(tx_ring->tx_bi)
   { memset(tx_ring->tx_bi, 0x00, bi_size); //ZERO the pre-alloc buf before use !
     return;
   }	 
	 
	 //if tx_ring->tx_bi == NULL
    for(i=0;i<I40E_RING_TX_BI_PREALLOC_MAX;i++)
    {  if(!i40e_ring_tx_bi_prealloc[i].enable)
	    {   i40e_ring_tx_bi_prealloc[i].enable=1;
	    	  tx_ring->tx_bi = i40e_ring_tx_bi_prealloc[i].tx_bi;
	    	  memset(tx_ring->tx_bi, 0x00, bi_size); //ZERO the pre-alloc buf before use !
      	  tx_ring->tx_bi_prealloc_index = i; //store the prealloc buf slot/index
           ///if(I40E_DEBUG) { printk("setup: i40e_ring_tx_bi_prealloc[i].enable = %d\n", i); }
           return;
       }
    }
}


