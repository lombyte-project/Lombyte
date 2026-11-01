# Help messages and tutorial records

`HelpState` in `rnc/ui/help/help_state.h` is the shared boot help-window
layout. Its geometry comes from `link_localized_display_text` and is consumed
by `draw_help`: `half_width`/`half_height` are the target size, while
`frame_half_width`/`frame_half_height` describe the animated frame.

## Message fields

`msg` indexes the loaded message table; `pending` is a request ID, with -1
meaning no request. `entry` indexes persistent records, and `count` bounds
the loaded table. These are different index spaces.

A `HelpMessageEntry` is 16 bytes on EE. Its first word is a text pointer
after relocation; archive loaders retain their integer-offset view before
relocation. `find_help_message_index` compares `message_id` at 0x04.
`update_help_state` adds 30000 to `audio_track_offset` at 0x08 for the
secondary audio stream, with -1 meaning no audio. The final word remains
unknown. Message IDs and audio-track offsets are not interchangeable.

## Records and time

`HelpRecord` and `UsageStat` are 8-byte records: a 16-bit count, a 16-bit
`latest_time_bucket`, and 32-bit level/status flags. Writers retain the
maximum of `scale_game_frames(play_time) / 600`; readers multiply by 600
when testing elapsed time. This is a coarse play-time bucket, not a best
score or countdown. Original narrowing and comparisons are preserved.

`UsageStat.level_mask` is signed: Novalis and Kerwan test it against zero
to inspect bit 31. `HelpRecord.level_flags` remains unsigned. Visited-level
bits coexist with bit 31, which `clear_help_record_high_bits` clears, so
it is not an immutable "ever shown" flag. `help_request` rejects a count
of 0xFFFF; other writers saturate the count there.

## Reference evidence

RC1 functions and callers establish these meanings. `rac1-decomp` at
`05da3217a4046ac01dec3ac74f051a348f063155` corroborates geometry and request
offsets in `src/game/help.c` and `src/overlays/shared/help_00203E98.c`.
Its help-update code derives from Lombyte and is not independent evidence.
No PAL addresses or implementation bodies were imported.

The help window and record layout checks in `src/check/` verify EE offsets
and sizes; boot and overlay byte checks remain the acceptance gates.
