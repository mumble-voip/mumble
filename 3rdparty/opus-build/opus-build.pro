include(../../qmake/compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-build,-src)

!win32 {
  VERSION=0 #$$ Fool packaging script
}

!exists(../$$SOURCEDIR/COPYING) {
  message("The $$SOURCEDIR/ directory was not found. Please update your submodules (git submodule update --init).")
  error("Aborting configuration")
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += no_include_pwd
VPATH = ../$$SOURCEDIR
TARGET = opus
DEFINES += HAVE_CONFIG_H

CONFIG(static) {
  CONFIG -= static
  CONFIG += shared
}

!CONFIG(third-party-warnings) {
  # We ignore warnings in third party builds. We won't actually look
  # at them and they clutter out our warnings.
  CONFIG -= warn_on
  CONFIG += warn_off
}

QMAKE_CFLAGS -= -fPIE -pie

win32 {
  DEFINES += WIN32 _WIN32
  INCLUDEPATH += ../$$BUILDDIR/win32

  CONFIG(sse2) {
    TARGET_VERSION_EXT = .sse2
  } else {
    QMAKE_CFLAGS_RELEASE -= -arch:SSE
    QMAKE_CFLAGS_DEBUG -= -arch:SSE
  }
}

unix {
  contains(QMAKE_CFLAGS, -ffast-math) {
    DEFINES += FLOAT_APPROX
  }

  INCLUDEPATH += ../$$BUILDDIR
}

INCLUDEPATH *= \
../$$SOURCEDIR \
../$$SOURCEDIR/celt \
../$$SOURCEDIR/celt/x86 \
../$$SOURCEDIR/include \
../$$SOURCEDIR/silk \
../$$SOURCEDIR/silk/x86 \
../$$SOURCEDIR/silk/float

win32-msvc* {
  CONFIG *= opus-sse-sources
  CONFIG *= opus-sse2-sources
  CONFIG *= opus-sse41-sources
}

macx:!CONFIG(universal) {
  CONFIG *= opus-sse-sources
  CONFIG *= opus-sse2-sources
}

# celt_sources.mk: CELT_SOURCES
SOURCES *= \
celt/bands.c \
celt/celt.c \
celt/celt_encoder.c \
celt/celt_decoder.c \
celt/cwrs.c \
celt/entcode.c \
celt/entdec.c \
celt/entenc.c \
celt/kiss_fft.c \
celt/laplace.c \
celt/mathops.c \
celt/mdct.c \
celt/modes.c \
celt/pitch.c \
celt/celt_lpc.c \
celt/quant_bands.c \
celt/rate.c \
celt/vq.c

CONFIG(opus-sse-sources) {
  # celt_sources.mk: CELT_SOURCES_SSE
  SOURCES *= \
  celt/x86/x86cpu.c \
  celt/x86/x86_celt_map.c \
  celt/x86/pitch_sse.c
}

CONFIG(opus-sse2-sources) {
  # celt_sources.mk: CELT_SOURCES_SSE2
  SOURCES *= \
  celt/x86/pitch_sse2.c \
  celt/x86/vq_sse2.c
}

CONFIG(opus-sse41-sources) {
  # celt_sources.mk: CELT_SOURCES_SSE4_1
  SOURCES *= \
  celt/x86/celt_lpc_sse4_1.c \
  celt/x86/pitch_sse4_1.c
}

# silk_sources.mk: SILK_SOURCES + SILK_SOURCES_FLOAT
SOURCES *= \
silk/CNG.c \
silk/code_signs.c \
silk/init_decoder.c \
silk/decode_core.c \
silk/decode_frame.c \
silk/decode_parameters.c \
silk/decode_indices.c \
silk/decode_pulses.c \
silk/decoder_set_fs.c \
silk/dec_API.c \
silk/enc_API.c \
silk/encode_indices.c \
silk/encode_pulses.c \
silk/gain_quant.c \
silk/interpolate.c \
silk/LP_variable_cutoff.c \
silk/NLSF_decode.c \
silk/NSQ.c \
silk/NSQ_del_dec.c \
silk/PLC.c \
silk/shell_coder.c \
silk/tables_gain.c \
silk/tables_LTP.c \
silk/tables_NLSF_CB_NB_MB.c \
silk/tables_NLSF_CB_WB.c \
silk/tables_other.c \
silk/tables_pitch_lag.c \
silk/tables_pulses_per_block.c \
silk/VAD.c \
silk/control_audio_bandwidth.c \
silk/quant_LTP_gains.c \
silk/VQ_WMat_EC.c \
silk/HP_variable_cutoff.c \
silk/NLSF_encode.c \
silk/NLSF_VQ.c \
silk/NLSF_unpack.c \
silk/NLSF_del_dec_quant.c \
silk/process_NLSFs.c \
silk/stereo_LR_to_MS.c \
silk/stereo_MS_to_LR.c \
silk/check_control_input.c \
silk/control_SNR.c \
silk/init_encoder.c \
silk/control_codec.c \
silk/A2NLSF.c \
silk/ana_filt_bank_1.c \
silk/biquad_alt.c \
silk/bwexpander_32.c \
silk/bwexpander.c \
silk/debug.c \
silk/decode_pitch.c \
silk/inner_prod_aligned.c \
silk/lin2log.c \
silk/log2lin.c \
silk/LPC_analysis_filter.c \
silk/LPC_inv_pred_gain.c \
silk/table_LSF_cos.c \
silk/NLSF2A.c \
silk/NLSF_stabilize.c \
silk/NLSF_VQ_weights_laroia.c \
silk/pitch_est_tables.c \
silk/resampler.c \
silk/resampler_down2_3.c \
silk/resampler_down2.c \
silk/resampler_private_AR2.c \
silk/resampler_private_down_FIR.c \
silk/resampler_private_IIR_FIR.c \
silk/resampler_private_up2_HQ.c \
silk/resampler_rom.c \
silk/sigm_Q15.c \
silk/sort.c \
silk/sum_sqr_shift.c \
silk/stereo_decode_pred.c \
silk/stereo_encode_pred.c \
silk/stereo_find_predictor.c \
silk/stereo_quant_pred.c \
silk/LPC_fit.c \
silk/float/apply_sine_window_FLP.c \
silk/float/corrMatrix_FLP.c \
silk/float/encode_frame_FLP.c \
silk/float/find_LPC_FLP.c \
silk/float/find_LTP_FLP.c \
silk/float/find_pitch_lags_FLP.c \
silk/float/find_pred_coefs_FLP.c \
silk/float/LPC_analysis_filter_FLP.c \
silk/float/LTP_analysis_filter_FLP.c \
silk/float/LTP_scale_ctrl_FLP.c \
silk/float/noise_shape_analysis_FLP.c \
silk/float/process_gains_FLP.c \
silk/float/regularize_correlations_FLP.c \
silk/float/residual_energy_FLP.c \
silk/float/warped_autocorrelation_FLP.c \
silk/float/wrappers_FLP.c \
silk/float/autocorrelation_FLP.c \
silk/float/burg_modified_FLP.c \
silk/float/bwexpander_FLP.c \
silk/float/energy_FLP.c \
silk/float/inner_product_FLP.c \
silk/float/k2a_FLP.c \
silk/float/LPC_inv_pred_gain_FLP.c \
silk/float/pitch_analysis_core_FLP.c \
silk/float/scale_copy_vector_FLP.c \
silk/float/scale_vector_FLP.c \
silk/float/schur_FLP.c \
silk/float/sort_FLP.c

CONFIG(opus-sse41-sources) {
  # silk_sources.mk: SILK_SOURCES_SSE4_1
  SOURCES *= \
  silk/x86/NSQ_sse4_1.c \
  silk/x86/NSQ_del_dec_sse4_1.c \
  silk/x86/x86_silk_map.c \
  silk/x86/VAD_sse4_1.c \
  silk/x86/VQ_WMat_EC_sse4_1.c
}

# opus_sources.mk: OPUS_SOURCES + OPUS_SOURCES_FLOAT
SOURCES *= \
src/opus.c \
src/opus_decoder.c \
src/opus_encoder.c \
src/opus_multistream.c \
src/opus_multistream_encoder.c \
src/opus_multistream_decoder.c \
src/repacketizer.c \
src/opus_projection_encoder.c \
src/opus_projection_decoder.c \
src/mapping_matrix.c \
src/analysis.c \
src/mlp.c \
src/mlp_data.c

include(../../qmake/symbols.pri)
