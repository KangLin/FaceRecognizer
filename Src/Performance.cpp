#include "Performance.h"
#include <QDebug>

CPerformance::CPerformance(const QString &szName, QObject *parent)
    : QObject(parent),
      m_szName(szName)
{
    m_StartTime = QTime::currentTime();
    StartTime();
}

CPerformance::~CPerformance()
{
    m_szMessage = m_szName
            + "[total use time: "
            + QString::number(m_StartTime.msecsTo(QTime::currentTime()))
            + "ms]:\n" + m_szMessage;
    qDebug().noquote() << m_szMessage;
}

int CPerformance::StartTime()
{
    m_Time = QTime::currentTime();
    return 0;
}

int CPerformance::AddTime(const QString &szFile,
                          int nLine,
                          const QString &szFunction,
                          const QString &szName)
{
    int nUse = m_Time.msecsTo(QTime::currentTime());
    m_szMessage += "  ";
    if(!szName.isEmpty())
        m_szMessage += szName +  ": ";
    else
        m_szMessage += "Use time: ";
    m_szMessage += QString::number(nUse) + "ms" + GetColor(ForegroundPurple)
            + " in [" + szFunction + "]" + szFile + "(" + QString::number(nLine) + ")"
            + GetColor(ForegroundReset) + "\n";
    m_Time = QTime::currentTime();
    return 0;
}

/*
  qt中如何用qDebug输出彩色调试信息: https://blog.csdn.net/wangxudongx/article/details/88178112
  
    常用的ANSI控制码如下：
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
QString CPerformance::GetColor(COLOR color)
{
    return "\033[" + QString::number(color) + "m";
}
