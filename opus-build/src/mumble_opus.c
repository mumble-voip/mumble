#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "celt.h"
#include "mumble_opus.h"

#include "opus_private.h"

int mumble_opus_decode_float(OpusDecoder *st, const unsigned char *data,
        int len, float *pcm, int frame_size, int decode_fec, int *packet_offset)
{
   return opus_decode_native(st, data, len, pcm, frame_size, decode_fec, 1, packet_offset);
}

