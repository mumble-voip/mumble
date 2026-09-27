# data/hrtf/default.sofa — Provenance

## Source

`default.sofa` is the CIPIC subject 124 HRTF, copied verbatim from the
**Valve Steam Audio** open-source SDK repository:

- Repository: https://github.com/ValveSoftware/steam-audio
- File path: `core/data/hrtf/cipic_124.sofa`
- Commit: `f88bd4e443ffdec9ec14ec52d2702de9702411a2`
  ("Merge from Perforce 2024-02-19 14:28:05.862022")

No processing was applied by Steam Audio beyond what is in the original CIPIC
distribution — Steam Audio loads the file as-is using `mysofa_open_no_norm()`.

## Original dataset

CIPIC subject 124 is from the CIPIC Interface Laboratory HRTF Database
(UC Davis, Center for Image Processing and Integrated Computing).

The original CIPIC website (https://www.ece.ucdavis.edu/cipic/spatial-sound/hrtf-data/)
is no longer reachable. Mirrors and references:

- GitHub mirror of the dataset: https://github.com/amini-allight/cipic-hrtf-database
- Web Archive snapshot of the original page:
  https://web.archive.org/web/20170916053150/http://interface.cipic.ucdavis.edu/sound/hrtf.html

## Why file choice matters

Raw HRTF measurements typically have strong spectral coloration — the frequency
response at front incidence may vary by 20 dB or more across the audible range.
When used directly for binaural rendering this colours speech noticeably (a
"telephone / band-pass" quality). A well-behaved HRTF for voice communication
should have a relatively flat front-incidence response (within ±10 dB from 100 Hz
to 8 kHz), with spectral shaping appearing primarily as pinna notches above 8 kHz
where it contributes to the spatial illusion rather than colouring speech.

The cipic_124.sofa file meets this criterion: its front-incidence response is flat
within ±5 dB from 100 Hz to 8 kHz. Whether this flatness is inherent to subject 124's
measurements or was applied as diffuse-field equalization upstream is not documented
by Steam Audio.

## License

Copyright (c) 2001 The Regents of the University of California. All Rights Reserved.

The CIPIC database is made available for educational, research, and commercial use
with an acknowledgment request (see Steam Audio's `core/THIRDPARTY.md` for the full
license text). Steam Audio redistributes it under their Apache 2.0 SDK licence.

## Updating

To replace this file with a different HRTF:

1. Obtain a SOFA file in the `SimpleFreeFieldHRIR` convention (44100 or 48000 Hz)
2. Prefer a file with a flat front-incidence response (diffuse-field equalized)
3. Verify with `ffmpeg -i test_impulse.wav -af "sofalizer=sofa=<file>:type=freq:rotation=0:elevation=0" ir_0deg.wav`
   and inspect the spectrum — it should be within ±10 dB from 100 Hz to 8 kHz at 0°
