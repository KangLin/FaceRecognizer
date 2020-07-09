#ifndef CDetectorNcnnRetina_H
#define CDetectorNcnnRetina_H

#include "Detector.h"
#include "net.h"
#include <vector>

class CDetectorNcnnRetina : public CDetector
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")

public:
    explicit CDetectorNcnnRetina(CFace* pFace = nullptr, QObject *parent = nullptr);
    
    virtual int Detect(const QImage &image,  QVector<QRect> &faces) override;
    
protected:
    virtual int UpdateParameter() override;

private:
    bool m_bInit;
    struct FaceInfo {
        QRect location;
        float score;
        float keypoints[10];
        bool mask;
    };
    
    ncnn::Net* m_pNet;
    using ANCHORS = QVector<QRect>;
    std::vector<ANCHORS> m_Anchors;
    const int m_RPNs[3] = { 32, 16, 8 };
    const QSize m_InputSize = {300, 300};
    const float m_IouThreshold = 0.4f;
	const float m_ScoreThreshold = 0.8f;
    
    int GenerateAnchors(const int & base_size,
                        const QVector<float>& ratios, 
                        const QVector<float> scales,
                        QVector<QRect>* anchors);
    int RatioAnchors(const QRect &anchor,
                     const QVector<float>& ratios, 
                     QVector<QRect>* anchors);
    int ScaleAnchors(const QVector<QRect>& ratio_anchors,
                     const QVector<float>& scales,
                     QVector<QRect>* anchors);
};

#endif // CDetectorNcnnRetina_H
