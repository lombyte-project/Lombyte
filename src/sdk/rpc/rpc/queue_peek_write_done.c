typedef unsigned int u32;

typedef struct QueueState {
    u32 buffer_offset;
    u32 completed_count;
    u32 reserved;
    unsigned char *write_cursor;
} QueueState;

void QueuePeekWriteDone(QueueState *queue) {
    unsigned char *next_cursor;
    u32 buffer_end_offset;
    queue->completed_count += 1;
    next_cursor = queue->write_cursor + 1;
    queue->write_cursor = next_cursor;
    buffer_end_offset = queue->buffer_offset + 16;
    if (next_cursor == (unsigned char *)queue + buffer_end_offset) {
        queue->write_cursor = (unsigned char *)queue + 16;
    }
}
