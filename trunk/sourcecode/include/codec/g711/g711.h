#ifndef _G711_H_
#define _G711_H_

#ifdef __cplusplus
extern "C"
{
#endif

void mulaw_dec( char* mulaw_data /* contains size char */, char* s16_data /* contains size*2 char */, int size );

void mulaw_enc( char* s16_data /* contains 320 char */, char* mulaw_data /* contains 160 char */, int pcm_size );

void alaw_dec( char* alaw_data /* contains size char */, char* s16_data /* contains size*2 char */, int size );

void alaw_enc( char* s16_data /* contains 320 char */, char* alaw_data /* contains 160 char */, int pcm_size );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

