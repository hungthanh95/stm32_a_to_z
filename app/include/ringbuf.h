#ifndef __RINGBUF_H
#define __RINGBUF_H

typedef struct
{
    int len;
    volatile char* data;
    volatile int head;
    volatile int tail;
} ringbuf;

// macro to write buffer
#define ringbuf_write(rb, x) \
    rb.data[ rb.tail ] = x; \
    if ( ( rb.tail + 1) > rb.len ) { rb.tail = 0; } \
    else { rb.tail = rb.tail + 1; }

// macro to read buffer
static inline char ringbuf_read(ringbuf* rb) {
    if ( rb->head == rb->tail ) { return '\0'; }
    char read = rb->data[rb->head];
    rb->head = ( rb->head < (rb->len - 1 ) ) ? ( rb->head + 1 ) : 0;
    return read;
}

#endif /* __RINGBUF_H */