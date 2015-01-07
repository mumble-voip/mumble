#ifndef MUMBLE_OVERLAY_OPENGLGLREPLICATION_H_
#define MUMBLE_OVERLAY_OPENGLGLREPLICATION_H_

// TODO: Merely including gl.h does not provide all of the data below. If
//       possible, we should use standard libs though.
// Included before gl.h, see https://www.opengl.org/archives/resources/faq/technical/gettingstarted.htm#0020
//#include <windows.h>
//#include <GL/gl.h>

typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef void            GLvoid;
typedef int             GLint;
typedef unsigned char   GLubyte;
typedef unsigned int    GLuint;
typedef int             GLsizei;
typedef float           GLfloat;
typedef double          GLdouble;

#define GL_ONE                                     0x1
#define GL_TEXTURE_2D                           0x0DE1
#define GL_UNSIGNED_BYTE                        0x1401
#define GL_QUADS                                0x0007
#define GL_RGBA                                 0x1908
#define GL_BGRA                                 0x80E1
#define GL_TEXTURE_WRAP_S                       0x2802
#define GL_TEXTURE_WRAP_T                       0x2803
#define GL_CLAMP_TO_EDGE                        0x812F
#define GL_TEXTURE_MAG_FILTER                   0x2800
#define GL_TEXTURE_MIN_FILTER                   0x2801
#define GL_NEAREST                              0x2600
#define GL_LINEAR                               0x2601
#define GL_FRONT_AND_BACK                       0x0408
#define GL_AMBIENT_AND_DIFFUSE                  0x1602
#define GL_SRC_ALPHA                            0x0302
#define GL_ONE_MINUS_SRC_ALPHA                  0x0303
#define GL_TEXTURE_GEN_S                        0x0C60
#define GL_TEXTURE_GEN_T                        0x0C61
#define GL_TEXTURE_GEN_R                        0x0C62
#define GL_TEXTURE_GEN_Q                        0x0C63
#define GL_STENCIL_TEST                         0x0B90
#define GL_SCISSOR_TEST                         0x0C11
#define GL_SEPARABLE_2D                         0x8012
#define GL_MINMAX                               0x802E
#define GL_LIGHTING                             0x0B50
#define GL_INDEX_LOGIC_OP                       0x0BF1
#define GL_HISTOGRAM                            0x8024
#define GL_FOG                                  0x0B60
#define GL_DITHER                               0x0BD0
#define GL_DEPTH_TEST                           0x0B71
#define GL_CULL_FACE                            0x0B44
#define GL_CONVOLUTION_1D                       0x8010
#define GL_CONVOLUTION_2D                       0x8011
#define GL_COLOR_TABLE                          0x80D0
#define GL_COLOR_MATERIAL                       0x0B57
#define GL_AUTO_NORMAL                          0x0D80
#define GL_ALPHA_TEST                           0x0BC0
#define GL_COLOR_LOGIC_OP                       0x0BF2
#define GL_MATRIX_MODE                          0x0BA0
#define GL_MODELVIEW                            0x1700
#define GL_PROJECTION                           0x1701
#define GL_BLEND                                0x0BE2
#define GL_TEXTURE_ENV                          0x2300
#define GL_TEXTURE_ENV_MODE                     0x2200
#define GL_REPLACE                              0x1E01
#define GL_PACK_ROW_LENGTH                      0x0D02
#define GL_UNPACK_ROW_LENGTH                    0x0CF2

#endif
