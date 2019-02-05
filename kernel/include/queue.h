/*
   Implementation of FIFO queue
*/

#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    unsigned char * _buffer;
    int in_ptr,out_ptr,size;
} fifo_queue_t;

#define DECLARE_FIFO_QUEUE(_name,_size) \
    static unsigned char ____fifo_data_##name [_size]; \
    fifo_queue_t _name = { \
        ._buffer = ____fifo_data_##name, \
        .in_ptr = 0, \
        .out_ptr = 0, \
        .size = (_size), \
    }

static inline int fifo_insert(fifo_queue_t *fifo, unsigned char key)
{
    int ptr = fifo->in_ptr + 1;
    if(ptr >= fifo->size) ptr = 0;
    /* Check if queue is full */
    if(ptr == fifo->out_ptr) return -1;
    fifo->_buffer[fifo->in_ptr] = key;
    fifo->in_ptr = ptr;
    return 0;
}

static inline int fifo_get(fifo_queue_t *fifo, unsigned char *key)
{
    /* Check if FIFO is empty */
    if(fifo->in_ptr == fifo->out_ptr) return -1;
    *key = fifo->_buffer[fifo->out_ptr];
    fifo->out_ptr++;
    if(fifo->out_ptr >= fifo->size)
    fifo->out_ptr = 0;
    return 0;
}

static inline int fifo_empty(fifo_queue_t *fifo)
{
    return (fifo->in_ptr == fifo->out_ptr) ? 1 : 0;
}

#endif
