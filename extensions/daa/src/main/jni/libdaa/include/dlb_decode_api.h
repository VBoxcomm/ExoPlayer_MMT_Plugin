 /******************************************************************************
*   This program is protected under international and U.S. copyright laws as
*   an unpublished work. This program is confidential and proprietary to the
*   copyright owners. Reproduction or disclosure, in whole or in part, or the
*   production of derivative works therefrom without the express permission
*   of the copyright owners is prohibited.
*
*                Copyright (C) 2013-2019 by Dolby Laboratories.
*                            All rights reserved.
*
*   Module:     Dolby Audio for Applications API
*
*   File:       dlb_decode_api.h
*
\***************************************************************************/

/*! \defgroup decode Dolby Decode

    The Dolby Audio for Applications library allows decoding of
    Dolby Digital, Dolby Digital Plus, Dolby Digital Plus JOC and Dolby AC-4 
    bitstreams to PCM and applies Dolby Audio Processing. 
    The library runs on the Google Android or Apple iOS platform.​
*/

/*! \addtogroup decode */
/*! \ingroup decode */

/*!
*  \file dlb_decode_api.h
*
*   \brief Dolby Audio for Applications API header file.
*
*/

#if !defined(DLB_DECODE_API_H)
#define DLB_DECODE_API_H

#include "dlb_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief API equates and enumerations for use with dlb_decode_query_info() and dlb_decode_query_memory()
 */
#define DLB_DECODE_MAX_PCMOUT_CHANNELS               (2)         /*!< Maximum number of PCM output channels supported */

#define DLB_DECODE_TIMESCALE_MIN                     (1)         /*!< Minumum timescale */
#define DLB_DECODE_TIMESCALE_MAX                     (1e9)       /*!< Maximum timescale*/
#define DLB_DECODE_TIMESCALE_DEFAULT                 (48000)     /*!< Default timescale */

/**
 * @brief API equates and enumerations for use with dlb_decode_setparam() and dlb_decode_getparam()
 */
#define DLB_DECODE_DIALOG_ENHANCEMENT_GAIN_MIN      (0)         /*!< Iinternally for 0, dialog enhancer is turned OFF */
#define DLB_DECODE_DIALOG_ENHANCEMENT_GAIN_MAX      (9)         /*!< Maximum dialog enhancement gain */   
#define DLB_DECODE_DIALOG_ENHANCEMENT_GAIN_DEFAULT  (6)         /*!< Default dialog enhancement gain */

#define DLB_DECODE_OUTPUT_REFERENCE_LEVEL_MIN       (-23)       /*!< Minumum output reference level */
#define DLB_DECODE_OUTPUT_REFERENCE_LEVEL_MAX       (-11)       /*!< Maximum output reference level */
#define DLB_DECODE_OUTPUT_REFERENCE_LEVEL_DEFAULT   (-14)       /*!< Default output reference level */

#define DLB_DECODE_PRESENTATION_MIN                 (0)         /*!< Minumum presentation id */
#define DLB_DECODE_PRESENTATION_MAX                 (510)       /*!< Maximum presentation id */
#define DLB_DECODE_PRESENTATION_DEFAULT             (0xffff)    /*!< Default presentation di */

#define DLB_DECODE_MAIN_ONLY_MIXING_LEVEL           (-32)       /*!< Minumum mixing preference of main and associated */
#define DLB_DECODE_ASSOCIATED_ONLY_MIXING_LEVEL     (32)        /*!< Maximum mixing preference of main and associated */
#define DLB_DECODE_MAIN_ASSO_PREF_DEFAULT           (-32)       /*!< Default mixing preference of main and associated */

/**
 * @brief Control parameter identification for use with dlb_decode_setparam() and dlb_decode_getparam()
 */
#define DLB_DECODE_CTL_DAP_ONOFF_ID                 (0)         /*!< Enable or disable all postprocessing features for playback. 
                                                                     Values is defined in #DLB_DECODE_DAP_ONOFF:
                                                                     #DLB_DECODE_DAP_ON : enable the postprocessing features with pre-defined values
                                                                        DLB_DECODE_CTL_VIRTUALIZER_ID: #DLB_DECODE_VIRTUALIZER_ON
                                                                        DLB_DECODE_CTL_DIALOG_ENHANCEMENT_ID: 6
                                                                        DLB_DECODE_CTL_OUTPUT_REFERENCE_LEVEL_ID: -14
                                                                     #DLB_DECODE_DAP_OFF : disable the postprocessing features with pre-defined values
                                                                        DLB_DECODE_CTL_VIRTUALIZER_ID: #DLB_DECODE_VIRTUALIZER_OFF
                                                                        DLB_DECODE_CTL_DIALOG_ENHANCEMENT_ID: 0
                                                                        DLB_DECODE_CTL_OUTPUT_REFERENCE_LEVEL_ID: -17
                                                                     Default value is #DLB_DECODE_DAP_ON. */
#define DLB_DECODE_CTL_ENDPOINT_ID                  (1)         /*!< Endpoint setting. 
                                                                     Value is defined in #DLB_DECODE_ENDPOINT:
                                                                     #DLB_DECODE_ENDP_STEREO_SPEAKER:    stereo speaker endpoint 
                                                                     #DLB_DECODE_ENDP_STEREO_HEADPHONE:  stereo headphone endpoint 
                                                                     Default value is #DLB_DECODE_ENDP_STEREO_HEADPHONE. */
#define DLB_DECODE_CTL_VIRTUALIZER_ID               (100)       /*!< Enable or disable virtualizer for playback endpoint.
                                                                     Values defined in #DLB_DECODE_VIRTUALIZER_ONOFF:
                                                                     #DLB_DECODE_VIRTUALIZER_ON:     enable virtualizer
                                                                     #DLB_DECODE_VIRTUALIZER_OFF:    disable virtualizer
                                                                     Default value is #DLB_DECODE_VIRTUALIZER_ON. */
#define DLB_DECODE_CTL_DIALOG_ENHANCEMENT_ID        (101)       /*!< Dialog Enhancer control parameter.
                                                                     Values in [0,9].
                                                                     0 :     Turns OFF Dialog Enhancer
                                                                     [1,9] : Enables Dialog enhancer internally and set corresponding gain level
                                                                     Default value (#DLB_DECODE_DIALOG_ENHANCEMENT_GAIN_DEFAULT) is 6. */
#define DLB_DECODE_CTL_OUTPUT_REFERENCE_LEVEL_ID    (102)       /*!< Indicate the preferred Output reference level in the unit of dB.
                                                                     Values in [-23,-11].
                                                                     Default value (#DLB_DECODE_OUTPUT_REFERENCE_LEVEL_DEFAULT) is -14. */
#define DLB_DECODE_CTL_PRESENTATION_ID              (200)       /*!< Indicated the presentation group index to select AC4 substream. 
                                                                     Only valid when #DLB_DECODE_INPUT_FORMAT_AC4 is set.
                                                                     Values in [0,510].
                                                                     Default value (#DLB_DECODE_PRESENTATION_DEFAULT) selects first decodable presentation */
#define DLB_DECODE_CTL_MAIN_ASSO_PREF_ID            (201)       /*!< Indicate the preferred mixing ratio between main and associated audio programs.
                                                                     Only valid when #DLB_DECODE_INPUT_FORMAT_AC4 is set.
                                                                     -32 :       associated fully muted
                                                                     [-1,-31] :  dB to favor main program (attenuate associated)
                                                                     0   :       neutral (no balance adjustment)
                                                                     [+1,+31] :  dB to favor associated program (attenuate main)
                                                                     32  :       main fully muted */

/**
 * @brief Error codes definitions
 */
typedef enum
{
    DLB_DECODE_ERR_NO_ERROR                          = 0,       /*!< Represents no error case*/
    DLB_DECODE_ERR_GENERIC                           = 1000,    /*!< Generic error */
    DLB_DECODE_ERR_INVALID_PARAM,                               /*!< Invalid parameter */
    DLB_DECODE_ERR_INVALID_PARAM_SIZE,                          /*!< Invalid size of Parameter */
    DLB_DECODE_ERR_UNSUPPORTED_PARAM,                           /*!< Unsupported Parameter */
    DLB_DECODE_ERR_INTERNAL_BUFFER,                             /*!< Internal error on buffer management */
    DLB_DECODE_ERR_FRAME_NOT_VALID,                             /*!< Attempt to prepare a valid frame for decoding */
    DLB_DECODE_ERR_FRAME_NEED_MORE_DATA,                        /*!< Provide more bitstream data to the framer in order to
                                                                     extract further frames.*/
    DLB_DECODE_ERR_PROCESSING,                                  /*!< Error processing */
    DLB_DECODE_ERR_UNSUPPORTED_OUTPUT_DATATYPE,                 /*!< Unsupported output datatype format */
    DLB_DECODE_ERR_UNSUPPORTED_BITSTREAM_FORMAT,                /*!< Unsupported bitstream format */
    DLB_DECODE_ERR_UNSUPPORTED_BITSTREAM_TYPE,                  /*!< Unsupported AC-4 bitstream type */
    DLB_DECODE_ERR_PROGRAM_CHANGE,                              /*!< Internal error handling AC-4 presentation change */
    DLB_DECODE_ERR_PROGRAM_MIXING                               /*!< Internal error handling AC-4 mixing between main and associated programs */
} dlb_decode_error_codes;

/**
 * @brief Input bitstream format definitions
 */
typedef enum
{
    DLB_DECODE_INPUT_FORMAT_UNDEFINED                  = -1,    /*!< Undefined bitstream format */
    DLB_DECODE_INPUT_FORMAT_DDP                        = 0,     /*!< Input bitstream is DD, DDP or DDP-JOC bitstream */
    DLB_DECODE_INPUT_FORMAT_AC4                        = 1      /*!< Input bitstream is AC-4 bitstream */
} dlb_decode_input_format;


/**
 * @brief Input bitstream type definitions if input bitstream format is AC-4
 */
typedef enum
{
    DLB_DECODE_INPUT_TYPE_UNDEFINED                    = -1,    /*!< Undefined AC-4 bitstream type */
    DLB_DECODE_INPUT_TYPE_AC4_SIMPLE_TRANSPORT         = 0,     /*!< Input AC-4 bitstream is simple transport stream  */
    DLB_DECODE_INPUT_TYPE_AC4_RAW_FRAME                = 1      /*!< Input AC-4 bitstream is Raw frame */
} dlb_decode_input_type;

/**
 * @brief Postprocessing on/off definitions
 */
typedef enum
{
    DLB_DECODE_DAP_OFF                                 = 0,    /*!< Internally for 0, postprocessing is turned off */
    DLB_DECODE_DAP_ON                                          /*!< Postprocessing is turned on */
} dlb_decode_dap_onoff;

/**
 * @brief Decoder audio end-point definitions
 */
typedef enum
{
    DLB_DECODE_ENDP_STEREO_SPEAKER                     = 1,    /*!< The audio end point is stereo speakers */
    DLB_DECODE_ENDP_STEREO_HEADPHONE                           /*!< The audio end point is stereo headphones */
} dlb_decode_endpoint;

/**
 * @brief Virtualizer on/off definitions
 */
typedef enum
{
    DLB_DECODE_VIRTUALIZER_OFF                         = 0,    /*!< Internally for 0, virtualizer is turned off */
    DLB_DECODE_VIRTUALIZER_ON                                  /*!< virtualizer is turned on */
} dlb_decode_virtualizer_onoff;

/**
 * @brief Structure used in dlb_decode_query_memory() and dlb_decode_open()
 */
typedef struct dlb_decode_query_ip_s
{
    int                         output_datatype;                /*!< Data type of output samples, set to #DLB_BUFFER_SHORT_16,
                                                                    #DLB_BUFFER_LONG_32,
                                                                    or #DLB_BUFFER_FLOAT */
    dlb_decode_input_format     input_bitstream_format;         /*!< Input bitstream format as defined in #dlb_decode_input_format.
                                                                    In case of decoding DD, DDP or DDP-JOC bitstream, the value should be 
                                                                    #DLB_DECODE_INPUT_FORMAT_DDP.
                                                                    In case of decoding AC-4 bitsteram, the value should be
                                                                    #DLB_DECODE_INPUT_FORMAT_AC4 */
    dlb_decode_input_type       input_bitstream_type;           /*!< Input bitstream type only valid for AC-4 bitstream.
                                                                    Defined in #dlb_decode_input_type.
                                                                    In case of ISO base media file format i.e. MP4, the value should be 
                                                                    #DLB_DECODE_INPUT_TYPE_AC4_RAW_FRAME.
                                                                    In case of transport streams, the value should be
                                                                    #DLB_DECODE_INPUT_TYPE_AC4_SIMPLE_TRANSPORT */
    unsigned long               timescale;                      /*!< Timescale in units of ticks per second. */
} dlb_decode_query_ip;

/**
 * @brief Structure with library version information parameters used in dlb_decode_query_info()
 */
typedef struct dlb_decode_query_info_op_s
{
    char                       daa_version[20];                 /*!< Dolby Audio for Applications library version */
    char                       daa_api_version;                 /*!< Dolby Audio for Applications library API version */
    char                       core_decoder_version[10];        /*!< Dolby DDP or AC-4 decoder version */
    char                       dap_version[10];                 /*!< Dolby postprocessing version. Only valid for DDP bitstream. */
} dlb_decode_query_info_op;

/**
 * @brief Structure with library memory information parameters used in dlb_decode_query_memory()
 */
typedef struct dlb_decode_query_mem_op_s
{
    size_t                      decoder_size;                   /*!< Size of the decoder structure in bytes */
    size_t                      output_buffer_size;             /*!< Buffer size (in bytes) for all the PCM output buffers */
} dlb_decode_query_mem_op;

/**
 * @brief Decode structure with input and output parameters for the dlb_decode_process() function
 */
typedef struct dlb_decode_io_params_s
{
    dlb_buffer                  *pcm_output_buf;                /*!< PCM output buffer (in/out) */
    unsigned long               output_sample_rate;             /*!< Sample rate of output PCM  in Hz */
    unsigned int                output_channels_num;            /*!< Number of output PCM channels */
    unsigned int                output_samples_num;             /*!< Number of samples of each PCM channel */
    unsigned int                delay_samples_num;              /*!< Delayed samples of output */
    signed long long            output_timestamp;               /*!< Output timestamp. This can be negative, 
                                                                    e.g. when input timestamp is 0. Timestamp values are defined
                                                                    based on timescale. Default timescale value is 48000 */
} dlb_decode_io_params;

/**************************************************************************//**
@addtogroup decode
@{
******************************************************************************/

/**
 * @brief Get the static parameters of the decoder
 *
 * @param[in]   p_inparams      Input parameters
 * @param[out]  p_outparams     Query output
 * @return      Error code:     Typical return values are
 *                              DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                              DLB_DECODE_ERR_INVALID_PARAM if the input parameter is not valid
 *                              DLB_DECODE_ERR_GENERIC for other error cases
 */
int
dlb_decode_query_info
(const dlb_decode_query_ip *p_inparams
, dlb_decode_query_info_op *p_outparams
);

/**
 * @brief Get the memory requirements of the decoder
 *
 * @param[in]   p_inparams      Input parameters
 * @param[out]  p_outparams     Query output
 * @return      Error code:     Typical return values are
 *                              DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                              DLB_DECODE_ERR_INVALID_PARAM if the input parameter is not valid
 *                              DLB_DECODE_ERR_GENERIC for other error cases
 */
int
dlb_decode_query_memory
(const dlb_decode_query_ip *p_inparams
 , dlb_decode_query_mem_op *p_outparams
);

/**
 * @brief Initialize the decoder memory.
 *
 * This includes initializing buffer pointers to point to allocated
 * buffers and setting control parameters to default values.
 * This will also open the decoder and postprocessing.
 * It is called once at system startup.
 *
 * @param[in]       p_inparams      Input parameters
 * @param[in,out]   p_decode_hdl    Pointer to decoder memory
 * @return          Error code:     Typical return values are
 *                                  DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                                  DLB_DECODE_ERR_INVALID_PARAM if the input parameter is invalid
 *                                  DLB_DECODE_ERR_GENERIC for other error cases
 */
int
dlb_decode_open
(const dlb_decode_query_ip *p_inparams
 , void *p_decode_hdl
);

/**
 * @brief Add bytes to the input of the decoder.
 *
 * Once the decoder has accumulated a frame worth of bytes,
 * the function dlb_decode_process() can be invoked.
 *
 * @param[in,out]   p_decode_hdl        Pointer to decoder memory
 * @param[in]       p_buffer            Pointer to buffer that will be processed
 * @param[in]       buflen              Buffered bytes available in p_buffer
 * @param[in]       input_timestamp     Input timestamp. It can be 0 in which case,
                                        output timestamp will be negative due to decoder latency.
                                        Timestamp values are defined based on timescale.
                                        Default timescale value is 48000
 * @param[out]      p_bytesconsumed     Number of bytes consumed out of p_buffer
 * @param[out]      p_frame_complete    Set to 1 if the decoder receives one complete frame
 * @return          Error code:         Typical return values are
 *                                      DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                                      DLB_DECODE_ERR_INVALID_PARAM if the input parameter is invalid
 *                                      DLB_DECODE_ERR_FRAME_NEED_MORE_DATA if Buffered bytes is not enough
 *                                      DLB_DECODE_ERR_FRAME_NOT_VALID if the frame is not completed
 *                                      DLB_DECODE_ERR_GENERIC for other error cases
 */
int
dlb_decode_addbytes
(void * const p_decode_hdl
 , const char *p_buffer
 , const unsigned int buflen
 , const signed long long input_timestamp
 , unsigned int *p_bytesconsumed
 , int * const p_frame_complete
);

/**
 * @brief Decode one complete audio frame.
 *
 * This processes an audio frame including performing postprocessing if it is enabled.
 *
 * @param[in,out]   p_decode_hdl    Pointer to decoder memory
 * @param[in,out]   p_params        Data structure with the input and output parameters
 * @return          Error code:     Typical return values are
 *                                  DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                                  DLB_DECODE_ERR_INVALID_PARAM if the input parameter is invalid
 *                                  DLB_DECODE_ERR_FRAME_NOT_VALID if the input frame is not completed
 *                                  DLB_DECODE_ERR_GENERIC for generic processing error
 */
int
dlb_decode_process
(void * const p_decode_hdl
 , dlb_decode_io_params *p_params
);

/**
 * @brief Set the specified decoder control parameter.
 *
 * The change takes effect on the next call to
 * dlb_decode_process(), when the new parameter value is
 * loaded for use by the decoder.
 *
 * Note: Do not call this function before dlb_decode_open() is called
 *
 * @param[in,out]   p_decode_hdl    Pointer to decoder memory
 * @param[in]       paramid         Control parameter identification
 * @param[in]       p_paramval      Pointer to parameter value
 * @param[in]       paramsize       Size of the parameter value in bytes
 * @returns         Error code:     Typical return values are
 *                                  DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                                  DLB_DECODE_ERR_INVALID_PARAM if the input parameter is invalid
 *                                  DLB_DECODE_ERR_UNSUPPORTED_PARAM if the input parameter is not supported
 */
int
dlb_decode_setparam
(void * const p_decode_hdl
 , const int paramid
 , void * const p_paramval
 , const int paramsize
);

/**
 * @brief Get the current value of the specified decoder control parameter.
 *
 * Note: Do not call this function before dlb_decode_open() is called
 *
 * @param[in]       p_decode_hdl    Pointer to decoder memory
 * @param[in]       paramid         Control parameter identification
 * @param[out]      p_paramval      Pointer to parameter value
 * @param[out]      p_paramsize     Size of the parameter value in bytes
 * @return          Error code:     Typical return values are
 *                                  DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                                  DLB_DECODE_ERR_UNSUPPORTED_PARAM if the input parameter is not supported
 */
int
dlb_decode_getparam
(const void *p_decode_hdl
 , const int  paramid
 , void * const p_paramval
 , int * const p_paramsize
);

/**
 * @brief Perform all clean up necessary to close the decoder.
 *
 * @param[in,out]   p_decode_hdl    Pointer to decoder memory
 * @return          Error code:     Typical return values are
 *                                  DLB_DECODE_ERR_NO_ERROR for normal operation (no error)
 *                                  DLB_DECODE_ERR_GENERIC for other errors
 */
int
dlb_decode_close
(void *p_decode_hdl
);


#ifdef __cplusplus
}
#endif

/* @} */

#endif /* !defined(DLB_DECODE_API_H) */
