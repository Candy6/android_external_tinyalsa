/* asoundlib.h
**
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

#ifndef ASOUNDLIB_H
#define ASOUNDLIB_H


/*
 * PCM API
 */

struct pcm;

#define PCM_OUT        0x00000000
#define PCM_IN         0x10000000

/* Bit formats */
enum pcm_format {
    PCM_FORMAT_S16_LE = 0,
    PCM_FORMAT_S32_LE,

    PCM_FORMAT_MAX,
};

/* Configuration for a stream */
struct pcm_config {
    unsigned int channels;
    unsigned int rate;
    unsigned int period_size;
    unsigned int period_count;
    enum pcm_format format;
};

/* Mixer control types */
enum mixer_ctl_type {
    MIXER_CTL_TYPE_BOOL,
    MIXER_CTL_TYPE_INT,
    MIXER_CTL_TYPE_ENUM,
    MIXER_CTL_TYPE_BYTE,
    MIXER_CTL_TYPE_IEC958,
    MIXER_CTL_TYPE_INT64,
    MIXER_CTL_TYPE_UNKNOWN,

    MIXER_CTL_TYPE_MAX,
};

/* Open and close a stream */
struct pcm *pcm_open(unsigned int card, unsigned int device,
                     unsigned int flags, struct pcm_config *config);
int pcm_close(struct pcm *pcm);
int pcm_is_ready(struct pcm *pcm);

/* Set and get config */
int pcm_get_config(struct pcm *pcm, struct pcm_config *config);
int pcm_set_config(struct pcm *pcm, struct pcm_config *config);

/* Returns a human readable reason for the last error */
const char *pcm_get_error(struct pcm *pcm);

/* Returns the buffer size (int bytes) that should be used for pcm_write.
 * This will be 1/2 of the actual fifo size.
 */
unsigned int pcm_get_buffer_size(struct pcm *pcm);

/* Returns the pcm latency in ms */
unsigned int pcm_get_latency(struct pcm *pcm);

/* Write data to the fifo.
 * Will start playback on the first write or on a write that
 * occurs after a fifo underrun.
 */
int pcm_write(struct pcm *pcm, void *data, unsigned int count);
int pcm_read(struct pcm *pcm, void *data, unsigned int count);


/*
 * MIXER API
 */

struct mixer;
struct mixer_ctl;

/* Open and close a mixer */
struct mixer *mixer_open(unsigned int card);
void mixer_close(struct mixer *mixer);

/* Obtain mixer controls */
unsigned int mixer_get_num_ctls(struct mixer *mixer);
struct mixer_ctl *mixer_get_ctl(struct mixer *mixer, unsigned int id);
struct mixer_ctl *mixer_get_ctl_by_name(struct mixer *mixer, const char *name);

/* Get info about mixer controls */
int mixer_ctl_get_name(struct mixer_ctl *ctl, char *name, unsigned int size);
enum mixer_ctl_type mixer_ctl_get_type(struct mixer_ctl *ctl);
const char *mixer_ctl_get_type_string(struct mixer_ctl *ctl);
unsigned int mixer_ctl_get_num_values(struct mixer_ctl *ctl);
unsigned int mixer_ctl_get_num_enums(struct mixer_ctl *ctl);
int mixer_ctl_get_enum_string(struct mixer_ctl *ctl, unsigned int enum_id,
                              char *string, unsigned int size);

/* Set and get mixer controls */
int mixer_ctl_get_percent(struct mixer_ctl *ctl, unsigned int id);
int mixer_ctl_set_percent(struct mixer_ctl *ctl, unsigned int id, int percent);

int mixer_ctl_get_value(struct mixer_ctl *ctl, unsigned int id);
int mixer_ctl_set_value(struct mixer_ctl *ctl, unsigned int id, int value);
int mixer_ctl_set_enum_by_string(struct mixer_ctl *ctl, const char *string);

/* Determe range of integer mixer controls */
int mixer_ctl_get_range_min(struct mixer_ctl *ctl);
int mixer_ctl_get_range_max(struct mixer_ctl *ctl);

#endif