#ifndef MUMBLE_OPUS_H
#define MUMBLE_OPUS_H

#include "opus.h"

#ifdef __cplusplus
extern "C" {
#endif

OPUS_EXPORT int mumble_opus_decode_float(OpusDecoder *st, const unsigned char *data,
        int len, float *pcm, int frame_size, int decode_fec, int *packet_offset);

OPUS_EXPORT int mumble_opus_packet_parse(const unsigned char *data, int len,
      unsigned char *out_toc, const unsigned char *frames[48],
      short size[48], int *payload_offset);

#ifdef __cplusplus
}
#endif

#endif
