/**
 * @file   yuv2rgb.h
 * @author Late Lee
 * @date   2012.01.20
 * 
 * @home   https://github.com/latelee/yuv2rgb
 * 
 * @brief  
 *         YUV422P/YUV420P/YUV422SP转RGB24实现代码
 *
 * @note
 *         1、在Windows编译测试
 *         2、代码中使用的YUV420P、YUV422P是平面(planar)格式，不是打包(packed)格式
 *
 *         3、qcif: 176*144
              cif: 352*288
 * @todo  确定好RGB排序到底是什么
 *
 * @log   2013-10-26 参考422p函数，实现422sp转换422p格式函数。将初始化接口隐藏，不对外公开
 *        2014-02-10 封装统一一个函数。
 *
 * 笔记：
            每个Y、U、V、R、G、B均占用1个字节
            YUV422P平面格式
            Y、U、V分开存储，Y占w*h，U、V分别占w*h/2。每行Y有w个，U、V各w/2个。
            两个Y共用一个U和V，两个像素有Y分量2个，U、V分量各一个，共4字节，因此一个像素占2字节。

            内存分布如下：
                    w
            +--------------------+
            |Y0Y1Y2Y3...         |
            |...                 |   h
            |...                 |
            |                    |
            +--------------------+
            |U0U1      |
            |...       |   h
            |...       |
            |          |
            +----------+
            |V0V1      |
            |...       |  h
            |...       |
            |          |
            +----------+
                w/2

            第一个像素：Y0 U0 V0
            第二个像素：Y1 U0 V0
            第三个像素：Y2 U1 V1
            第四个像素：Y3 U1 V1

			每种格式分布见转换函数处
 */

#ifndef _YUV2RGB_H_
#define _YUV2RGB_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  YUV422P转换为RGB24(查表法)
 * 
 * @param  yuvbuffer[IN]    YUV422P平面格式缓冲区
 * @param  rgbbuffer[OUT]   RGB缓冲区
 * @param  width[IN]        图像宽
 * @param  height[IN]       图像高
 * 
 * @return no
 *
 * @note
 *         1.YUV422P格式YUV缓冲区大小为w * h * 2
 *         2.rgbbuffer数据排序为RGB，如保存BMP，需要调整为BGR
 */
void yuv422p_to_rgb24(unsigned char* yuvbuffer, unsigned char* rgbbuffer, int width, int height);

/** 
 * @brief YUV420P转RGB24(查表法)
 * 
 * @param yuvbuffer  YUV420P格式缓冲区
 * @param rgbbuffer  RGB24格式缓冲区
 * @param width      图像宽
 * @param height     图像高
 *
 *
 * @note
 *        1、YUV420P格式YUV缓冲区大小为w * h * 3 / 2
 *        2、rgbbuffer数据排序为RGB，如保存BMP，需要调整为BGR
 */
void yuv420p_to_rgb24(unsigned char* yuvbuffer,unsigned char* rgbbuffer, int width, int height);

/** 
 * @brief YUV422SP转RGB24(查表法)
 * 
 * @param yuvbuffer  YUV422SP格式缓冲区
 * @param rgbbuffer  RGB24格式缓冲区
 * @param width      图像宽
 * @param height     图像高
 *
 *
 * @note
 *        1、YUV422SP格式YUV缓冲区大小为w * h * 2
 *        2、rgbbuffer数据排序为RGB，如保存BMP，需要调整为BGR
 */
void yuv422sp_to_rgb24(unsigned char* yuvbuffer,unsigned char* rgbbuffer, int width, int height);

// TODO：定义个好一点的值
typedef enum
{
    YUV420P = 0xffff0001,
    YUV422P = 0xffff0002,
	YUV422SP = 0xfff0003,
}YUV_TYPE;

/** 
 * @brief YUV转RGB24(查表法)
 * 
 * @param type       YUV格式类型
 * @param yuvbuffer  YUV格式缓冲区
 * @param rgbbuffer  RGB24格式缓冲区
 * @param width      图像宽
 * @param height     图像高
 *
 * @return 0: OK -1: failed
 *
 * @note
 *        1、YUV422 buffer: w * h * 2 YUV420 buffer: w * h * 3 / 2
 *        2、rgbbuffer数据排序为RGB，如保存BMP，需要调整为BGR
 */
int yuv_to_rgb24(YUV_TYPE type, unsigned char* yuvbuffer,unsigned char* rgbbuffer, int width, int height);

////////////////////////////////////////////////////////////////////////////////////////////////////
// todo
void yuv422sp_to_yuv422p(unsigned char* yuv422sp, unsigned char* yuv422p, int width, int height);

void yuv420sp_to_yuv420p(unsigned char* yuv420sp, unsigned char* yuv420p, int width, int height);


/////////////////////////////////////////////////

void yuv420_to_rgb24_1(unsigned char* yuv420, unsigned char* rgb, int width, int height);

void yuv420_to_rgb24_2(unsigned char *yuv420, unsigned char *rgb24, int width, int height) ;

void yuv420_to_rgb24_3(unsigned char* yuv, unsigned char* rgb, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /* _YUV2RGB_H_ */
