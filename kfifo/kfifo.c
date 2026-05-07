// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * A generic kernel FIFO implementation
 *
 * Copyright (C) 2009/2010 Stefani Seibold <stefani@seibold.net>
 */
#include <kfifo.h>
#include <string.h>

static inline unsigned char is_power_of_2(unsigned long n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

static inline unsigned int min(unsigned int a, unsigned int b)
{
	return (a < b ? a : b);
}

/*
 * internal helper to calculate the unused elements in a fifo
 */
static inline unsigned int kfifo_unused(kfifo_t *fifo)
{
	return (fifo->mask + 1) - (fifo->in - fifo->out);
}

int kfifo_init(kfifo_t *fifo, void *buffer, unsigned int len, unsigned int esize)
{
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;
	fifo->data = buffer;

	if ((0 == is_power_of_2(len)) || (len < 2))
	{
		fifo->mask = 0;
		return -1;
	}

	fifo->mask = len - 1;

	return 0;
}

static void kfifo_copy_in(kfifo_t *fifo, const void *src, unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(fifo->data + off, src, l);
	memcpy(fifo->data, src + l, len - l);
	/*
	 * make sure that the data in the fifo is up to date before
	 * incrementing the fifo->in index counter
	 */
	//smp_wmb();
}

unsigned int kfifo_in(kfifo_t *fifo, const void *buf, unsigned int len)
{
	unsigned int l;

	l = kfifo_unused(fifo);
	if (len > l)
		len = l;

	kfifo_copy_in(fifo, buf, len, fifo->in);
	fifo->in += len;
	return len;
}

static void kfifo_copy_out(kfifo_t *fifo, void *dst, unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(dst, fifo->data + off, l);
	memcpy(dst + l, fifo->data, len - l);
	/*
	 * make sure that the data is copied before
	 * incrementing the fifo->out index counter
	 */
	//smp_wmb();
}

unsigned int kfifo_out_peek(kfifo_t *fifo, void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	kfifo_copy_out(fifo, buf, len, fifo->out);
	return len;
}

unsigned int kfifo_out(kfifo_t *fifo, void *buf, unsigned int len)
{
	len = kfifo_out_peek(fifo, buf, len);
	fifo->out += len;
	return len;
}

void kfifo_reset(kfifo_t *fifo)
{
	fifo->in = fifo->out = 0;
}

void kfifo_reset_out(kfifo_t *fifo)
{
	fifo->out = fifo->in;
}

unsigned int kfifo_len(kfifo_t *fifo)
{
	return fifo->mask+1;
}

unsigned int kfifo_used_len(kfifo_t *fifo)
{
	return fifo->in - fifo->out;
}

unsigned int kfifo_unused_len(kfifo_t *fifo)
{
	return kfifo_unused(fifo);
}

unsigned char kfifo_is_empty(kfifo_t *fifo)
{
	return fifo->in == fifo->mask;
}

unsigned char kfifo_is_full(kfifo_t *fifo)
{
	return kfifo_used_len(fifo) > fifo->mask;
}
