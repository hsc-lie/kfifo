/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * A generic kernel FIFO implementation
 *
 * Copyright (C) 2013 Stefani Seibold <stefani@seibold.net>
 */

#ifndef _LINUX_KFIFO_H
#define _LINUX_KFIFO_H

/*
 * How to porting drivers to the new generic FIFO API:
 *
 * - Modify the declaration of the "struct kfifo *" object into a
 *   in-place "struct kfifo" object
 * - Init the in-place object with kfifo_alloc() or kfifo_init()
 *   Note: The address of the in-place "struct kfifo" object must be
 *   passed as the first argument to this functions
 * - Replace the use of __kfifo_put into kfifo_in and __kfifo_get
 *   into kfifo_out
 * - Replace the use of kfifo_put into kfifo_in_spinlocked and kfifo_get
 *   into kfifo_out_spinlocked
 *   Note: the spinlock pointer formerly passed to kfifo_init/kfifo_alloc
 *   must be passed now to the kfifo_in_spinlocked and kfifo_out_spinlocked
 *   as the last parameter
 * - The formerly __kfifo_* functions are renamed into kfifo_*
 */

/*
 * Note about locking: There is no locking required until only one reader
 * and one writer is using the fifo and no kfifo_reset() will be called.
 * kfifo_reset_out() can be safely used, until it will be only called
 * in the reader thread.
 * For multiple writer and one reader there is only a need to lock the writer.
 * And vice versa for only one writer and multiple reader there is only a need
 * to lock the reader.
 */

typedef struct __kfifo {
	unsigned int	in;
	unsigned int	out;
	unsigned int	mask;
	unsigned int	esize;
	void		*data;
}kfifo_t;

extern int kfifo_init(kfifo_t *fifo, void *buffer, unsigned int len, unsigned int esize);
extern unsigned int kfifo_in(kfifo_t *fifo, const void *buf, unsigned int len);
extern unsigned int kfifo_out(kfifo_t *fifo, void *buf, unsigned int len);
extern unsigned int kfifo_out_peek(kfifo_t *fifo, void *buf, unsigned int len);
extern void kfifo_reset(kfifo_t *fifo);
extern void kfifo_reset_out(kfifo_t *fifo);
extern unsigned int kfifo_len(kfifo_t *fifo);
extern unsigned int kfifo_used_len(kfifo_t *fifo);
extern unsigned int kfifo_unused_len(kfifo_t *fifo);
extern unsigned char kfifo_is_empty(kfifo_t *fifo);
extern unsigned char kfifo_is_full(kfifo_t *fifo);

#endif
