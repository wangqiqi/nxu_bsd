/************************************************************************
*        Copyright (C), 2023 NXU Network Tech. Co., Ltd.
*                         LET AI CONNECT FUTURE
*************************************************************************
*
* File Name   : nxu_common.h
* Version     : 1.0.0
* Author      : 007 @ TECH TEAM (zhou24388@163.com)
* Created     : 2023/06/12
* Description : The common data type defination
******************************************************************************/
#ifndef __NXU_COMMON_H__
#define __NXU_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

    #ifndef NULL
    #define NULL            0L
    #endif
    #define NXU_NULL        0L
    #define NXU_NULL_PTR    0L

    #define NXU_MAX(a, b) ((a) > (b) ? (a) : (b))
    #define NXU_MIN(a, b) ((a) < (b) ? (a) : (b))
    #define NXU_ABS(a)    ((a) >= 0 ? (a) : (-(a)))
    /* Clip Data between min to max*/
    #define NXU_CLIP(val, min_val, max_val)  ((val) >= (min_val) ? ((val) <= (max_val) ? (val) : (max_val)) : (min_val))

#define NXU_ASSERT(a) assert(a)

    /** NXU_EXPORTS */
#if defined(NXU_API_EXPORTS)
    #define NXU_EXPORTS __declspec(dllexport)
#elif defined(NXU_API_IMPORTS)
    #define NXU_EXPORTS __declspec(dllimport)
#else
    #define NXU_EXPORTS extern
#endif

    /** NXU_INLINE */
#ifndef NXU_INLINE
#if defined __cplusplus
    #define NXU_INLINE inline
#elif (defined WIN32 || defined WIN64) && !defined __GNUC__
    #define NXU_INLINE __inline
#else
    #define NXU_INLINE static
#endif
#endif

    /* META Type Definitions */
    typedef unsigned char      NXU_U8;
    typedef unsigned char      NXU_UCHAR;
    typedef unsigned short     NXU_U16;
    typedef unsigned int       NXU_U32;

    typedef char               NXU_S8;
    typedef char               NXU_CHAR;
    typedef char *             NXU_PCHAR;
    typedef short              NXU_S16;
    typedef int                NXU_S32;

#ifndef _M_IX86
    typedef unsigned long long NXU_U64;
    typedef long long          NXU_S64;
#else
    typedef __int64            NXU_U64;
    typedef __int64            NXU_S64;
#endif

    typedef float              NXU_FLOAT;
    typedef double             NXU_DOUBLE;
    typedef unsigned long      NXU_SIZE_T;
    typedef unsigned long      NXU_LENGTH_T;

    /* Handle */
    typedef void               NXU_VOID;
    typedef void *             NXU_HANDLE;
    typedef void *             NXU_PVOID;

    /* ------------------ Fixed Point Type ------------------------*/
    /* u8bit */
    typedef unsigned char      NXU_U0Q8;
    typedef unsigned char      NXU_U1Q7;
    typedef unsigned char      NXU_U5Q3;

    /* u16bit */
    typedef unsigned short     NXU_U0Q16;
    typedef unsigned short     NXU_U4Q12;
    typedef unsigned short     NXU_U5Q11;
    typedef unsigned short     NXU_U6Q9;
    typedef unsigned short     NXU_U8Q8;
    typedef unsigned short     NXU_U9Q7;
    typedef unsigned short     NXU_U12Q4;
    typedef unsigned short     NXU_U14Q2;

    /* s16bit */
    typedef short              NXU_S9Q7;
    typedef short              NXU_S14Q2;
    typedef short              NXU_S1Q15;

    /* u32bit */
    typedef unsigned int       NXU_U22Q10;
    typedef unsigned int       NXU_U25Q7;
    typedef unsigned int       NXU_U21Q11;

    /* s32bit */
    typedef int                NXU_S25Q7;
    typedef int                NXU_S16Q16;

    /* Boolean */
    typedef enum _nxubool_e_
    {
        NXU_FALSE = 0,
        NXU_TRUE = 1,
    } NXU_BOOL;

    /* Shape Enum */
    typedef enum _nxushape_e_
    {
        NXU_SHAPE_POINT    = 0,     /* Point */
        NXU_SHAPE_LINE     = 1,     /* Line */
        NXU_SHAPE_RECT     = 2,     /* Rect */
        NXU_SHAPE_CIRCLE   = 3,     /* Circle */
        NXU_SHAPE_ELLIPSE  = 4,     /* Ellipse */
        NXU_SHAPE_POLYGON  = 5,     /* Polygon */
        NXU_SHAPE_POLYLINE = 6,     /* Polyline */
        NXU_SHAPE_FREEHAND = 7      /* Free */
    } NXU_SHAPE_E;


    /************************************************************************/
    /* Point definition                                                     */
    /************************************************************************/
    /* 2D Point Definition */
    typedef struct _nxu2dpoint_s_
    {
        NXU_S32 s32X; /* X */
        NXU_S32 s32Y; /* Y */
    } NXU_POINT_S, NXU_2DPOINT_S;

    /* 2D Point with float data */
    typedef struct _nxu2dpoint32f_s_
    {
        NXU_FLOAT f32X; /* X */
        NXU_FLOAT f32Y; /* Y */
    } NXU_POINT32F_S, NXU_2DPOINT32F_S;

    /* 3D Point */
    typedef struct _nxu3dpoint_s_
    {
        NXU_S32 s32X; /* X */
        NXU_S32 s32Y; /* Y */
        NXU_S32 s32Z; /* Z */
    } NXU_3DPOINT_S;

    /* 3D Point with float data */
    typedef struct _nxu3dpoint32f_s_
    {
        NXU_FLOAT f32X; /* X */
        NXU_FLOAT f32Y; /* Y */
        NXU_FLOAT f32Z; /* Z */
    } NXU_3DPOINT32F_S;

    /**
     * @brief Multi-Points Definition.
     *
     * NXU_POLYGON_S: adjacent point order loop, first->two->three....->last->first,
     *      closed plane area, polygon shape.
     * NXU_POLYLINE_S: adjacent point order, first->two->three....->last, open no loop
     * NXU_POINTS_SET_S: adjacent point order, first, two, three, ...., last, points set
     * NXU_FREEHAND_S: just points set, no order, no loop
     */
    typedef struct _nxumultipoints_s_
    {
        NXU_S32      s32Num;           /* Points Num */
        NXU_POINT_S* pstPoints;        /* Points set */
    } NXU_POINTS_SET_S, NXU_POLYGON_S, NXU_POLYLINE_S, NXU_FREEHAND_S;

    /* 2D Points Set with float data */
    typedef struct _nxu2dpointsset32f_s_
    {
        NXU_S32         s32PtNum;      /* Points Num */
        NXU_POINT32F_S* pstPoints;     /* Points Set */
    } NXU_POINTS32F_SET_S;

    /* Polygon, normally used in the config para */
#define NXU_MAX_VERTEX_NUM 16          /* max vertex num of the polygon */
    typedef struct _nxupolygon_s_
    {
        NXU_U8      u8VertexNum;                    /* Points Num */
        NXU_POINT_S astVertex[NXU_MAX_VERTEX_NUM];  /* Points Array */
    } NXU_Polygon_S;

    // 3D Points Set
    typedef struct _nxu3dpointsset_s_
    {
        NXU_S32        s32PtNum;       /* Points Num */
        NXU_3DPOINT_S* pstPoints;      /* Points Set */
    } NXU_3DPOINTS_SET_S;

    // 3D Points Set with float data
    typedef struct _nxu3dpintsset32f_s_
    {
        NXU_S32           s32PtNum;    /* Points Num */
        NXU_3DPOINT32F_S* pstPoints;   /* Points Set */
    } NXU_3DPOINTS32F_SET_S;


    /************************************************************************/
    /* Line definition                                                      */
    /************************************************************************/
    /* Line */
    /**
     * @brief Multi-lines definition.
     *
     * maybe used for key-points set, or line-sets, or curve-sets, or ...
     * bIsDirect: Defaults to NXU_FALSE.
     *     if true, the line is direct from start to end,
     *    otherwise, the line is not direct
     */
    typedef struct _nxuline_s_
    {
        NXU_BOOL    bIsDirect;         /* Direct or Not */
        NXU_POINT_S stStart;           /* Start Point */
        NXU_POINT_S stEnd;             /* End   Point */
    } NXU_LINE_S;

    /* Line with float data */
    typedef struct _nxuline32f_s_
    {
        NXU_BOOL       bIsDirect;      /* Direct or Not */
        NXU_POINT32F_S stStart;        /* Start Point */
        NXU_POINT32F_S stEnd;          /* End   Point */
    } NXU_LINE32F_S;

    /* Lines Set with default data */
    typedef struct _nxu2dlineset_s_
    {
        NXU_S32     s32LineNum;        /* Lines Num */
        NXU_LINE_S* pstLines;          /* Lines Set */
    } NXU_LINES_SET_S;

    /* Lines Set with float data */
    typedef struct _nxu2Dlinesets32f_s_
    {
        NXU_S32        s32LineNum;     /* Lines Num */
        NXU_LINE32F_S* pstLineSet;     /* Lines Set */
    } NXU_2DLINES32F_SET_S;


    /************************************************************************/
    /* Shape Definition: Rect, Circle, Ellipse                              */
    /************************************************************************/
    /* Rect */
    typedef struct _nxurect2p_s_
    {
        NXU_POINT_S stTopLeft;         /* Top    Left  Point */
        NXU_POINT_S stBottomRight;     /* Bottom Right Point */
    } NXU_RECT_S;

    /* Rect use width and height */
    typedef struct _nuxrectwh_s_
    {
        NXU_S32 s32X1;                 /* Top left x */
        NXU_S32 s32Y1;                 /* Top left y */
        NXU_S32 s32Width;              /* Rect Width */
        NXU_S32 s32Height;             /* Rect Height */
    }NXU_Rect_S;

    /* Rect with float data */
    typedef struct _nxurect32f2p_s_
    {
        NXU_POINT32F_S stTopLeft;      /* Top Left Point */
        NXU_POINT32F_S stBottomRight;  /* Bottom Right Point */
    } NXU_RECT32F_S;

    /* Rect with float data use width and height */
    typedef struct _nuxrect32fwh_s_
    {
        NXU_FLOAT      f32X1;          /* Top left x */
        NXU_FLOAT      f32Y1;          /* Top left y */
        NXU_FLOAT      f32Width;       /* Rect Width */
        NXU_FLOAT      f32Height;      /* Rect Height */
    } NXU_Rect32F_S;

    /* Rotate Rect */
    typedef struct _nxurotate_rect_s_
    {
        NXU_Rect32F_S  stRect;         /* Rectangle */
        NXU_FLOAT      f32Angle;       /* Angle */
    } NXU_ROTATE_Rect_S;

    /* Circle */
    typedef struct _nxucircle_s_
    {
        NXU_POINT_S    stCenter;       /* Center Point */
        NXU_S32        s32Radius;      /* Radius */
    } NXU_CIRCLE_S;

    /* Circle with float data */
    typedef struct _nxucircle32f_s_
    {
        NXU_POINT32F_S stCenter;       /* Center Point */
        NXU_FLOAT      f32Radius;      /* Radius */
    } NXU_CIRCLE32F_S;

    /* Ellipse */
    typedef struct _nxuellipse_s_
    {
        NXU_POINT_S    stCenter;       /* Center Point */
        NXU_S32        s32RadiusX;     /* Radius X */
        NXU_S32        s32RadiusY;     /* Radius Y */
    } NXU_ELLIPSE_S;

    /* Ellipse with float data */
    typedef struct _nxuellipse32f_s_
    {
        NXU_POINT32F_S stCenter;       /* Center Point */
        NXU_FLOAT      f32RadiusX;     /* Radius X */
        NXU_FLOAT      f32RadiusY;     /* Radius Y */
    } NXU_ELLIPSE32F_S;



    /************************************************************************/
    /* STATUS_CODE                                                          */
    /************************************************************************/
    /**
     * @brief all zero is ok, good status
     * we can use the 'binary shift' and 'binary xor' or 'binary or' to get the status
     */
    typedef enum _nxustatuscode_e_
    {
        NXU_SUCCESS               = 0x00000000, /* Success */
        NXU_FAILURE               = 0x80000000, /* Failure */

        /* memory failure */
        NXU_MEM_FAIL              = 0x80000001, /* memory failure */
        NXU_MEM_FAIL_MALLOC       = 0x80000002, /* memory allocation failure */
        NXU_MEM_FAIL_CALLOC       = 0x80000003, /* calloc */
        NXU_MEM_FAIL_REALLOC      = 0x80000004, /* realloc failure */
        NXU_MEM_FAIL_FREE         = 0x80000005, /* free memory failure */

        /* image failure */
        NXU_IMAGE_FAIL            = 0x80000010, /* image failure */
        NXU_IMAGE_FAIL_SIZE       = 0x80000020, /* image size error */
        NXU_IMAGE_FAIL_FORMAT     = 0x80000030, /* image format error */
        NXU_IMAGE_FAIL_READ       = 0x80000040, /* read image file failure */
        NXU_IMAGE_FAIL_WRITE      = 0x80000050, /* write failure */

        /* file failure */
        NXU_FILE_FAIL             = 0x80000100, /* file operation failure */
        NXU_FILE_FAIL_READ        = 0x80000200, /* read file failure */
        NXU_FILE_FAIL_WRITE       = 0x80000300, /* write file failure */

        /* para failure */
        NXU_PARA_FAIL             = 0x80001000, /* parameter error */
        NXU_PARA_FAIL_CONFIG      = 0x80002000, /* config file failure */
        NXU_PARA_FAIL_INVALID     = 0x80003000, /* invalid parameter */
        NXU_PARA_FAIL_UNSUPPORT   = 0x80004000, /* not support */

        /* license failure */
        NXU_LICENSE_FAIL          = 0x80010000, /* license failure */
        NXU_LICENSE_FAIL_TIMEOUT  = 0x80020000, /* license timeout */
        NXU_LICENSE_FAIL_READ_MAC = 0x80030000, /* read mac failure */
        NXU_LICENSE_FAIL_CHECK    = 0x80040000, /* license check failure */
        NXU_LICENSE_FAIL_INVALID  = 0x80050000, /* license invalid */

        /* net failure */
        NXU_NETWORK_FAIL          = 0x80100000, /* network failure */
        NXU_NETWORK_CONNECT       = 0x80200000, /* network connect failure */
        NXU_NETWORK_PUSH          = 0x80300000, /* network push failure */
        NXU_NETWORK_GET           = 0x80400000, /* network get failure */
    } NXU_STATUS_CODE_E;

    /**
     * @brief if f32Real = 2, f32Imag = 3, then the complex number is 2+3i
     * maybe used in Fourier
     */
    typedef struct _nxucomplexdata_s_
    {
        NXU_FLOAT f32Real;
        NXU_FLOAT f32Imag;
    } NXU_Complex_S;

    /************************************************************************/
    /* Image and Matrix Definition                                          */
    /************************************************************************/
    /**
     * @brief the packed pixel, maybe rgb, yuv, hsv, gray, ...
     */
    typedef struct _nxupackedpixel_s_
    {
        NXU_U8 u8ColorA;
        NXU_U8 u8ColorB;
        NXU_U8 u8ColorC;
        NXU_U8 u8ColorReserved;
    } NXU_PACKED_PIXEL_S;

    /* BGR pixel value */
    typedef struct _nxupixel_s_
    {
        NXU_U8 u8B; /* B */
        NXU_U8 u8G; /* G */
        NXU_U8 u8R; /* R */
    } NXU_PIXEL_S, NXU_BGR_PIXEL_S;

    /* HSV pixel value */
    typedef struct _nxuhsvpixel_s_
    {
        NXU_FLOAT f32H; /* H */
        NXU_FLOAT f32S; /* S */
        NXU_FLOAT f32V; /* V */
    } NXU_HSV_PIXEL_S;

    /* HSL pixel value */
    typedef struct _nxuhslpixel_s_
    {
        NXU_FLOAT f32H; /* H */
        NXU_FLOAT f32S; /* S */
        NXU_FLOAT f32L; /* L */
    } NXU_HSL_PIXEL_S;

    /**
     * @brief Usual image format
     *  U(Cb), V(Cr)
     */
    typedef enum _nxuimageformat_e_
    {
        NXU_IMAGE_FORMAT_UNKNOWN = 0,  /* Unknown */
        NXU_IMAGE_FORMAT_GRAY,         /* GRAY GRAY GRAY GRAY */
        NXU_IMAGE_FORMAT_RGB_PACKED,   /* RGB RGB RGB RGB RGB RGB RGB RGB */
        NXU_IMAGE_FORMAT_RGB_PLANAR,   /* RRRRRRRR GGGGGGGG BBBBBBBBB */
        NXU_IMAGE_FORMAT_BGR_PACKED,   /* BGR BGR BGR BGR BGR BGR BGR BGR */
        NXU_IMAGE_FORMAT_BGR_PLANAR,   /* BBBBBBBBB GGGGGGGG RRRRRRRR */
        NXU_IMAGE_FORMAT_HSV,          /* hsv, but not often used */
        NXU_IMAGE_FORMAT_HLS,          /* hls, but not often used */
        NXU_IMAGE_FORMAT_YUV444P,      /* YYYYYYYY VVVVVVVV UUUUUUU */
        NXU_IMAGE_FORMAT_YUV422P,      /* YYYYYYYY VVVV UUUU */
        NXU_IMAGE_FORMAT_YUV422_YUYV,  /* YUYV YUYV YUYV YUYV */
        NXU_IMAGE_FORMAT_YUV422_UYVY,  /* UYVY UYVY UYVY UYVY */
        NXU_IMAGE_FORMAT_YUV420p_YV12, /* YYYYYYYY VV UU */
        NXU_IMAGE_FORMAT_YUV420p_I420, /* YYYYYYYY UU VV */
        NXU_IMAGE_FORMAT_YUV420sp,     /* YYYYYYYY UVUV, default */
        NXU_IMAGE_FORMAT_YUV420_NV12,  /* YYYYYYYY UVUV */
        NXU_IMAGE_FORMAT_YUV420_NV21,  /* YYYYYYYY VUVU */
        NXU_IMAGE_FORMAT_YUV400,       /* YYYYYYYY, only y */
        NXU_IMAGE_FORMAT_BayerRGGB,    /* RGGB RGGB RGGB RGGB RGGB RGGB */
        NXU_IMAGE_FORMAT_BayerGRBG,    /* GRBG GRBG GRBG GRBG GRBG GRBG */
        NXU_IMAGE_FORMAT_BayerGBRG,    /* GBRG GBRG GBRG GBRG GBRG GBRG */
        NXU_IMAGE_FORMAT_BayerBGGR,    /* BGGR BGGR BGGR BGGR BGGR BGGR */
        NXU_IMAGE_FORMAT_BayerGR,      /* GBRG GBRG GBRG GBRG GBRG GBRG */
        NXU_IMAGE_FORMAT_BayerRG,      /* RGGB RGGB RGGB RGGB RGGB RGGB */
        NXU_IMAGE_FORMAT_BayerGB,      /* GBRG GBRG GBRG GBRG GBRG GBRG */
        NXU_IMAGE_FORMAT_BayerBG,      /* BGGR BGGR BGGR BGGR BGGR BGGR */
        NXU_IMAGE_FORMAT_UNSUPPORTED,  /* unsupported format */
    } NXU_IMAGE_FORMAT_E;

    /**
     * @brief
     * DO REMEMBER: That the image is a matrix, not a Vector
     * DO REMEMBER: FREE the memory after Use
     * DO REMEMBER: How to store the data in the memory accord to the image format
     *
     */
    typedef struct _nxuimage_s_
    {
        NXU_S32            s32W;       /* Width */
        NXU_S32            s32H;       /* Height */
        NXU_U8*            pData;      /* Image Data */
        NXU_IMAGE_FORMAT_E eFormat;    /* Image Format */
    } NXU_IMAGE_S;

    /* 3-ch uchar Image Definition */
    typedef struct _nxuimages3_s_
    {
        NXU_S32 s32W;                  /* Image width  */
        NXU_S32 s32H;                  /* Image height */
        NXU_U8 *pu8D1;                 /* Channel 1 data pointer */
        NXU_U8 *pu8D2;                 /* Channel 2 data pointer */
        NXU_U8 *pu8D3;                 /* Channel 3 data pointer */
        NXU_U32 u32Time;               /* Time tag */
        NXU_IMAGE_FORMAT_E enFormat;   /* image format pointer */
        NXU_U32 u32Flag;               /* Reserved flag */
    } NXU_IMAGE3_S;

    /* Matrix with uchar data */
    typedef struct _nxudatamat8u_s_
    {
        NXU_S32 s32Rows;               /* Rows */
        NXU_S32 s32Cols;               /* Columns */
        NXU_S32 s32Chns;               /* Channels */
        NXU_U8 *pu8Data;               /* Data Buffer */
    } NXU_MAT_8U_S;

    /* Matrix with short data */
    typedef struct _nxudatamat16s_s_
    {
        NXU_S32  s32Rows;              /* Rows */
        NXU_S32  s32Cols;              /* Columns */
        NXU_S32  s32Chns;              /* Channels */
        NXU_S16* ps16Data;             /* Data Buffer */
    } NXU_MAT_16S_S;

    /* Matrix with int data */
    typedef struct _nxudatamat32s_s_
    {
        NXU_S32 s32Rows;               /* data rows */
        NXU_S32 s32Cols;               /* Columns */
        NXU_S32 s32Chns;               /* Channels */
        NXU_S32* ps32Data;             /* Data Buffer */
    } NXU_MAT_32S_S;

    /* Matrix with float data */
    typedef struct _nxudatamat32f_s_
    {
        NXU_S32    s32Rows;            /* data rows */
        NXU_S32    s32Cols;            /* Columns */
        NXU_S32    s32Chns;            /* Channels */
        NXU_FLOAT* pf32Data;           /* Data Buffer */
    } NXU_MAT_32F_S;

    /**
     * @brief
     * Do REMEMBER: How to store the data in the memory.
     *       if the mat store the image, do remember the data organization
     * DO REMEMBER: Free the memory after use the matrix or image.
     */
    typedef struct _nxumat_s_
    {
        NXU_S32   s32C;                /* Channel */
        NXU_S32   s32W;                /* Width */
        NXU_S32   s32H;                /* Height */
        NXU_VOID* pData;               /* Data */
    } NXU_EXIMAGE_S, NXU_MAT_S, NXU_MATRIX_S;

    /* Image in RGB format */
    typedef struct _nxurgbimage_s_
    {
        NXU_S32 s32W;                  /* Image width  */
        NXU_S32 s32H;                  /* Image height */
        NXU_U8* pu8Data;               /* Image data buffer */
        NXU_U32 u32Time;               /* Time tag */
    } NXU_RGB_IMAGE_S;

    /* Image in HSV format */
    typedef struct _nxuhsvimage_s_
    {
        NXU_S32 s32W;                  /* Image width  */
        NXU_S32 s32H;                  /* Image height */
        NXU_FLOAT *pf32Data;           /* Image data buffer */
        NXU_U32 u32Time;               /* Time tag */
    } NXU_HSV_IMAGE_S;

    /* Gray image with uchar data */
    typedef struct _nxugrayimage_s_
    {
        NXU_S32 s32W;                  /* Image width  */
        NXU_S32 s32H;                  /* Image height */
        NXU_U8* pu8Data;               /* Image data buffer */
    } NXU_GRAY_IMAGE_S;

    /* Gray image with short data, */
    typedef struct _nxugrayimage16s_s_
    {
        NXU_S32  s32W;                 /* Image width  */
        NXU_S32  s32H;                 /* Image height */
        NXU_S16* ps16Data;             /* Image data buffer */
    } NXU_GRAY_IMAGE16_S;

    /* Gray Image with int data, maybe used in intergral image */
    typedef struct _nxugrayimage32s_s_
    {
        NXU_S32  s32W;                 /* Image width  */
        NXU_S32  s32H;                 /* Image height */
        NXU_S32* ps32Data;             /* Image data buffer */
    } NXU_GRAY_IMAGE32_S;

    /************************************************************************/
    /* Deep learning or Machine Learning object.                            */
    /************************************************************************/
    typedef struct _nxudlmltgt_s_
    {
        NXU_S32    s32LabelID;         /* Label ID, name accord to project */
        NXU_RECT_S stRect;             /* Target Bounding Box */
        NXU_FLOAT  f32Prob;            /* Probability */
    } NXU_Target_S;

#ifdef __cplusplus
}
#endif /* end of __cplusplus*/

#endif /* end of file*/