#ifndef CPERFORMANCE_H
#define CPERFORMANCE_H

#pragma once

#ifndef BUILD_PERFORMANCE
    #define PERFORMANCE(name)
    #define PERFORMANCE_START(name)
    #define PERFORMANCE_ADD_TIME(name, message)
    #define RETURN(value)    
#else
    #define PERFORMANCE(name) \
        CPerformance object##name (#name);
    #define PERFORMANCE_START(name) object##name.StartTime();
    #define PERFORMANCE_ADD_TIME(name, message) \
        object##name.AddTime(__FILE__, __LINE__, __FUNCTION__, message);
    #define RETURN(value)

#include <QObject>
#include <QTime>

#include "facerecognizer_export.h"

class FACERECOGNIZER_EXPORT CPerformance : public QObject
{
    Q_OBJECT

public:
    explicit CPerformance(const QString &szName = QString(),
                          QObject *parent = nullptr);
    virtual ~CPerformance();
    
    int StartTime();
    int AddTime(const QString &szFile,
                int nLine,
                const QString &szFunction = QString(),
                const QString &szName = QString());
    
private:
    QString m_szName;
    QString m_szMessage;
    QTime m_StartTime;
    QTime m_Time;

    /*
     * 常用的ANSI控制码如下：
    \033[0m 关闭所有属性
    \033[1m 高亮
    \033[2m 亮度减半
    \033[3m 斜体
    \033[4m 下划线
    \033[5m 闪烁
    \033[6m 快闪
    \033[7m 反显
    \033[8m 消隐
    \033[9m 中间一道横线
    10-19 关于字体的
    21-29 基本与1-9正好相反
    30-37 设置前景色
    40-47 设置背景色
    30:黑
    31:红
    32:绿
    33:黄
    34:蓝色
    35:紫色
    36:深绿
    37:白色
    38 打开下划线,设置默认前景色
    39 关闭下划线,设置默认前景色
    40 黑色背景
    41 红色背景
    42 绿色背景
    43 棕色背景
    44 蓝色背景
    45 品红背景
    46 孔雀蓝背景
    47 白色背景
    48 不知道什么东西
    49 设置默认背景色
    50-89 没用
    90-109 又是设置前景背景的，比之前的颜色浅
    \033[nA 光标上移n行
    \033[nB 光标下移n行
    \033[nC 光标右移n行
    \033[nD 光标左移n行
    \033[y;xH设置光标位置
    \033[2J 清屏
    \033[K 清除从光标到行尾的内容
    \033[s 保存光标位置
    \033[u 恢复光标位置
    \033[?25l 隐藏光标
    \033[?25h 显示光标
     */
    enum COLOR{
        ForegroundBlack = 30,
        ForegroundRed,
        ForegroundGreen,
        ForegroundYellow,
        ForegroundBlue,
        ForegroundPurple,
        ForegroundDeepGreen,
        ForegroundWhite,
        ForegroundReset = 39,
        
        BackgroundBlack = 40,
        BackgroundRed,
        BackgroundGreen,
        BackgroundYellow,
        BackgroundBlue,
        BackgroundPurple,
        BackgroundDeepGreen,
        BackgroundWhite,
        
        ForegroundLightBlack = 90,
        ForegroundLightRed,
        ForegroundLightGreen,
        ForegroundLightYellow,
        ForegroundLightBlue,
        ForegroundLightPurple,
        ForegroundLightDeepGreen,
        ForegroundLightWhite,
        
        BackgroundLightBlack = 100,
        BackgroundLightRed,
        BackgroundLightGreen,
        BackgroundLightYellow,
        BackgroundLightBlue,
        BackgroundLightPurple,
        BackgroundLightDeepGreen,
        BackgroundLightWhite,
    };
    QString GetColor(COLOR color);
};

#endif //BUILD_PERFORMANCE

#endif // CPERFORMANCE_H
