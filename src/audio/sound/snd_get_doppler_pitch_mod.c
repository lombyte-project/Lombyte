int snd_get_doppler_pitch_mod(int projected_velocity) {
    return (projected_velocity * 0x5F4) / 0x2E5;
}
