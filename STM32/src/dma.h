#ifndef _DMA_H_
#define _DMA_H_

struct dma_reg {
	volatile unsigned long interrupt_status;		// DMA_ISR
	volatile unsigned long interrupt_flag_clear;	// DMA_IFCR
};

#define DMA_BASE_ADDR	0x40020000
#define DMA_1_ADDR		(struct dma_reg *) 0x40020000
#define DMA_2_ADDR		(struct dma_reg *) 0x40020400
#define DMA_CHANNEL_CONFIG_REG(dma_channel, channel)	(volatile unsigned long *) (DMA_BASE_ADDR + ((dma_channel - 1) * 0x400) + 0x08 + 20 * (channel - 1))
#define DMA_NUMBER_OF_DATA_REG(dma_channel, channel)	(volatile unsigned long *) (DMA_BASE_ADDR + ((dma_channel - 1) * 0x400) + 0x0C + 20 * (channel - 1))
#define DMA_PERIPHERAL_ADDR_REG(dma_channel, channel)	(volatile unsigned long *) (DMA_BASE_ADDR + ((dma_channel - 1) * 0x400) + 0x10 + 20 * (channel - 1))
#define DMA_MEMORY_ADDR_REG(dma_channel, channel)		(volatile unsigned long *) (DMA_BASE_ADDR + ((dma_channel - 1) * 0x400) + 0x14 + 20 * (channel - 1))

// DMA interrupt status register bits
// Reference manual - page 284
#define DMA_CHANNEL_TRANSFER_ERROR_FLAG(channel)		(1 << 3 + ((channel - 1) * 4))	// TEIFx: Channel x transfer error flag
#define DMA_CHANNEL_HALF_TRANSFER_FLAG(channel)			(1 << 2 + ((channel - 1) * 4))	// HTIFx: Channel x half transfer flag
#define DMA_CHANNEL_TRANSFER_COMPLETE_FLAG(channel)		(1 << 1 + ((channel - 1) * 4))	// TCIFx: Channel x transfer complete flag
#define DMA_CHANNEL_GLOBAL_INTERRUPT_FLAG(channel)		(1 << 0 + ((channel - 1) * 4))	// GIF1: Channel 1 global interrupt flag

// DMA interrupt flag clear register bits
// Reference manual - page 285
#define DMA_CHANNEL_TRANSFER_ERROR_CLEAR(channel)		(1 << 3 + ((channel - 1) * 4))	// TEIFx: Channel x transfer error clear
#define DMA_CHANNEL_HALF_TRANSFER_CLEAR(channel)		(1 << 2 + ((channel - 1) * 4))	// HTIFx: Channel x half transfer clear
#define DMA_CHANNEL_TRANSFER_COMPLETE_CLEAR(channel)	(1 << 1 + ((channel - 1) * 4))	// TCIFx: Channel x transfer complete clear
#define DMA_CHANNEL_GLOBAL_INTERRUPT_CLEAR(channel)		(1 << 0 + ((channel - 1) * 4))	// GIFx: Channel x global interrupt clear

// DMA channel x configuration register bits
// Reference manual - page 286
#define DMA_MEM2MEM	14	// MEM2MEM: Memory to memory mode
#define DMA_PL		12	// PL[1:0]: Channel priority level
#define DMA_MSIZE	10	// MSIZE[1:0]: Memory size
#define DMA_PSIZE	8	// PSIZE[1:0]: Peripheral size
#define DMA_MINC	7	// MINC: Memory increment mode
#define DMA_PINC	6	// PINC: Peripheral increment mode
#define DMA_CIRC	5	// CIRC: Circular mode
#define DMA_DIR		4	// DIR: Data transfer direction
#define DMA_TEIE	3	// TEIE: Transfer error interrupt enable
#define DMA_HTIE	2	// HTIE: Half transfer interrupt enable
#define DMA_TCIE	1	// TCIE: Transfer complete interrupt enable
#define DMA_EN		0	// EN: Channel enable



#endif