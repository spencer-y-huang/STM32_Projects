#ifndef RINGBUF_H
#define RINGBUF_H

typedef struct {
	int len;
	volatile char* buf;
	volatile int pos;
	volatile int ext;
} ringbuf;

static inline void ringbuf_write(ringbuf* rb, char x) {
	rb->buf[rb->ext] = x;
	if ((rb->ext + 1) >= rb->len) { 
		rb->ext = 0; 
	} else {
		rb->ext++;
	}
}

static inline char ringbuf_read(ringbuf* rb) {
	if (rb->pos == rb->ext) {
		return '\0';
	}
	
	char read = rb->buf[rb->pos];
	rb->pos = (rb->pos < (rb->len - 1)) ? (rb->pos + 1) : 0;
	return read;
}

#endif
