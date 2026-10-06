typedef unsigned int u32;

typedef struct QueueReadState {
    u32 buffer_offset;
    u32 completed_count;
    unsigned char *read_cursor;
} QueueReadState;

void QueuePeekReadDone(QueueReadState *queue) {
    unsigned char *next_cursor;
    u32 buffer_end_offset;
    queue->completed_count -= 1;
    next_cursor = queue->read_cursor + 1;
    queue->read_cursor = next_cursor;
    buffer_end_offset = queue->buffer_offset + 16;
    if (next_cursor == (unsigned char *)queue + buffer_end_offset) {
        queue->read_cursor = (unsigned char *)queue + 16;
    }
}
