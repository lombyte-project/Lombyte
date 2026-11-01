# Help messages and tutorial records

The help window has one RC1 layout, `HelpWindowState`, shared by the boot
window code and the Novalis, Kerwan and shared request code. Its storage
remains signed 32-bit words except the two halfwords at 0x38 and 0x3A.
Byte views in the drawing and menu-loader code are intentional: changing
how the compiler obtains a global's address can change the retail code.

## Window and message fields

`link_localized_display_text` (`FUN_001fdd58`) measures the text and sets
`target_half_width` and `target_half_height` at 0x08 and 0x0C. The following
words are `center_x`, `center_y`, `frame_half_width` and `frame_half_height`.
`draw_help` (`FUN_001fe980`) uses those same words to animate the frame;
`help_draw_prompt` (`FUN_001fe898`) sets its half extents to 32. The shared
`HelpWindowPhase` names describe the visible stages used by drawing,
updating and dismissal. They do not describe the separate audio-stream states.

`message_index` at 0x20 indexes the loaded message table;
`pending_message_id` at 0x24 is a request ID, with -1 meaning no request.
`help_record_index` at 0x28 indexes persistent records, and `message_count`
at 0x2C bounds the loaded table. These are different index spaces.
`update_help_state` (`FUN_001fde90`) also establishes the input-start delay
at 0x30/0x34 and the queued-message fields at 0x38..0x3C.

A `HelpMessageEntry` is 16 bytes on EE. Its first word is a text pointer
once relocated; archive loaders still use an integer offset view before
relocation. `find_help_message_index` (`FUN_001fdca0`) compares the ID at
0x04. The word at 0x08 is a different value: `update_help_state` adds 30000
to it for the secondary audio stream, with -1 meaning no audio. The final
word is still unknown. The lookup's message ID must not be substituted for
the audio-track offset.

## Records and time

Both `HelpRecord` and `UsageStat` have 8-byte records: a 16-bit count, a
16-bit `latest_time_bucket`, and 32-bit level/status flags. Writers retain
the maximum of `scale_game_frames(play_time) / 600`. The field is a coarse
play-time bucket, not a best score or a countdown. Readers multiply by 600
when testing elapsed time. The division, narrowing and original comparisons
are unchanged.

The two types deliberately retain different signedness for their flag word.
`UsageStat.level_mask` is signed: Novalis and Kerwan test it against zero
to inspect bit 31. `HelpRecord.level_flags` is unsigned. Bits for visited
levels coexist with bit 31, which `clear_help_record_high_bits`
(`FUN_L00_00203a28`) clears. It is therefore not an immutable "ever shown"
flag. `help_request` (`FUN_L00_00203908`) rejects a count of 0xFFFF;
other writers saturate the count there.

The `help_message_owner`, `help_message_request_state` and
`current_help_message_id` names retain their original D_0015F5D4,
D_0015F5D0 and D_00161288 bindings. Existing register-live calls in
`force_help_message` and `get_help_message_text` keep their original
argument declarations; naming cleanup must not silently change their ABI.

## Reference evidence

RC1 functions and their callers establish these field meanings.
`rac1-decomp` at `05da3217a4046ac01dec3ac74f051a348f063155`
corroborates window geometry and request offsets in `src/game/help.c` and
`src/overlays/shared/help_00203E98.c`. Its help-update code derives from
Lombyte and is not independent evidence. No PAL addresses or bodies were imported.

The help window and record layout checks in `src/check/` verify EE offsets
and sizes; boot and overlay byte checks remain the acceptance gates.
