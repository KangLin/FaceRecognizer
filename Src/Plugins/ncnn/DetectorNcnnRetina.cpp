#include "DetectorNcnnRetina.h"
#include "Log.h"
#include "Performance.h"
#include <QDir>
#include <QtGlobal>

CDetectorNcnnRetina::CDetectorNcnnRetina(CFace *pFace, QObject *parent)
    : CDetector(pFace, parent),
      m_bInit(false),
      m_pNet(nullptr)
{
    m_pNet = new ncnn::Net();
}

int ComputeIOU(const QRect & rect1,
	const QRect & rect2, float * iou,
	const std::string& type) {
    
    QRect intersected = rect1.intersected(rect2);
    float intersectArea = 0;
    if(intersected.isValid())
        intersectArea = intersected.width() * intersected.height();
	if (type == "UNION") {
        if(intersected.isValid())
        {
            *iou = intersectArea / (rect1.width() * rect1.height()
                                    + rect2.width() * rect2.height()
                                    - intersectArea);
        }
	}
	else {
		*iou = intersectArea / qMin(rect1.width() * rect1.height(),
                                    rect2.width() * rect2.height());
	}

	return 0;
}

template <typename T>
int const NMS(const std::vector<T>& inputs, std::vector<T>& result,
	const float& threshold, const std::string& type = "UNION") {
	result.clear();
    if (inputs.size() == 0)
        return -1;
    
    std::vector<T> inputs_tmp;
    inputs_tmp.assign(inputs.begin(), inputs.end());
    std::sort(inputs_tmp.begin(), inputs_tmp.end(),
    [](const T& a, const T& b) {
        return a.score < b.score;
    });

    std::vector<int> indexes(inputs_tmp.size());

    for (int i = 0; i < indexes.size(); i++) {
        indexes[i] = i;
    }

    while (indexes.size() > 0) {
        int good_idx = indexes[0];
        result.push_back(inputs_tmp[good_idx]);
        std::vector<int> tmp_indexes = indexes;
        indexes.clear();
        for (int i = 1; i < tmp_indexes.size(); i++) {
            int tmp_i = tmp_indexes[i];
            float iou = 0.0f;
            ComputeIOU(inputs_tmp[good_idx].location, inputs_tmp[tmp_i].location, &iou, type);
            if (iou <= threshold) {
                indexes.push_back(tmp_i);
            }
        }
    }
}

int CDetectorNcnnRetina::UpdateParameter()
{
    m_bInit = false;

    if(m_pNet)
        m_pNet->clear();
    else
        m_pNet = new ncnn::Net();
    
    QString szPath = getModelPath() + QDir::separator() + "ncnn";
    QDir d;
    if(!d.exists(szPath)) szPath = getModelPath();
    QString szFileParam = szPath + QDir::separator() + "fd.param";
    QString szFileBin = szPath + QDir::separator() + "fd.bin";
    try {
        m_pNet->clear();
        if (m_pNet->load_param(szFileParam.toStdString().c_str()) == -1 ||
            m_pNet->load_model(szFileBin.toStdString().c_str()) == -1) {
            LOG_MODEL_ERROR("CDetectorNcnnRetina", "load face detect model failed.");
            return -1;
        }
        
        // generate anchors
        ANCHORS anchors;
        GenerateAnchors(16, { 1.0f }, { 32, 16 }, &anchors);
        m_Anchors.push_back(anchors);
        GenerateAnchors(16, { 1.0f }, { 8, 4 }, &anchors);
        m_Anchors.push_back(anchors);
        GenerateAnchors(16, { 1.0f }, { 2, 1 }, &anchors);
        m_Anchors.push_back(anchors);

    } catch (...) {
        QString szErr = "Load model fail:" + szFileParam;
        LOG_MODEL_ERROR("CDetectorNcnnRetina", szErr.toStdString().c_str());
        return -2;
    }
    m_bInit = true;
    return 0;
}

int CDetectorNcnnRetina::Detect(const QImage &image, QVector<QRect> &faces)
{
    if(image.isNull()) return -1;
    if(!m_bInit) return -2;
    
    PERFORMANCE(NcnnRetina)
    QImage img = image;
    if(img.format() != QImage::Format_RGB888)
    {
        PERFORMANCE_START(NcnnRetina)
        img = img.convertToFormat(QImage::Format_RGB888);     
        PERFORMANCE_ADD_TIME(NcnnRetina,
                             "conver format, image width:"
                             + QString::number(image.width())
                             + ";Height:"
                             + QString::number(image.height()))
    }
    //img = img.rgbSwapped();
    PERFORMANCE_ADD_TIME(NcnnRetina, "rgbSwapped")
    
    faces.clear();
    PERFORMANCE_START(NcnnRetina)
    
    float factor_x = static_cast<float>(img.width()) / m_InputSize.width();
    float factor_y = static_cast<float>(img.height()) / m_InputSize.height();
    ncnn::Extractor ex = m_pNet->create_extractor();
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(img.bits(),
                                                 ncnn::Mat::PIXEL_RGB,
                                                 img.width(),
                                                 img.height(),
                                                 m_InputSize.width(),
                                                 m_InputSize.height());
    ex.input("data", in);

    std::vector<FaceInfo> faces_tmp;
	for (int i = 0; i < 3; ++i) {
		std::string class_layer_name = "face_rpn_cls_prob_reshape_stride" + std::to_string(m_RPNs[i]);
		std::string bbox_layer_name = "face_rpn_bbox_pred_stride" + std::to_string(m_RPNs[i]);
		std::string landmark_layer_name = "face_rpn_landmark_pred_stride" + std::to_string(m_RPNs[i]);

		ncnn::Mat class_mat, bbox_mat, landmark_mat;
		ex.extract(class_layer_name.c_str(), class_mat);
		ex.extract(bbox_layer_name.c_str(), bbox_mat);
		ex.extract(landmark_layer_name.c_str(), landmark_mat);

		ANCHORS anchors = m_Anchors.at(i);
		int width = class_mat.w;
		int height = class_mat.h;
		int anchor_num = static_cast<int>(anchors.size());
		for (int h = 0; h < height; ++h) {
			for (int w = 0; w < width; ++w) {
				int index = h * width + w;
				for (int a = 0; a < anchor_num; ++a) {
					float score = class_mat.channel(anchor_num + a)[index];
					if (score < m_ScoreThreshold) {
						continue;
					}
					QRect box = QRect(w * m_RPNs[i] + anchors[a].x(),
						h * m_RPNs[i] + anchors[a].y(),
						anchors[a].width(),
						anchors[a].height());

					float delta_x = bbox_mat.channel(a * 4 + 0)[index];
					float delta_y = bbox_mat.channel(a * 4 + 1)[index];
					float delta_w = bbox_mat.channel(a * 4 + 2)[index];
					float delta_h = bbox_mat.channel(a * 4 + 3)[index];
					QPointF center = QPointF(box.x() + box.width() * 0.5f,
						box.y() + box.height() * 0.5f);
					center.setX(center.x() + delta_x * box.width());
					center.setY(center.y() + delta_y * box.height());
					float curr_width = std::exp(delta_w) * (box.width() + 1);
					float curr_height = std::exp(delta_h) * (box.height() + 1);
					QRect curr_box = QRect(center.x() - curr_width * 0.5f,
						center.y() - curr_height * 0.5f, curr_width, curr_height);
					curr_box.setX(qMax(static_cast<int>(curr_box.x() * factor_x), 0));
					curr_box.setY(qMax(static_cast<int>(curr_box.y() * factor_y), 0));
					curr_box.setWidth(qMin(image.width() - curr_box.x(), static_cast<int>(curr_box.width() * factor_x)));
					curr_box.setHeight(qMin(image.height() - curr_box.y(), static_cast<int>(curr_box.height() * factor_y)));

                    FaceInfo face_info;
					memset(&face_info, 0, sizeof(face_info));
					face_info.score = score;
					face_info.location = curr_box;
					faces_tmp.push_back(face_info);
				}
			}
		}
	}

    std::vector<FaceInfo> faces_result;
    NMS(faces_tmp, faces_result, m_IouThreshold);

    foreach (auto f, faces_result) {
        faces.push_back(f.location);
    }
    return 0;
}

int CDetectorNcnnRetina::RatioAnchors(const QRect &anchor,
	const QVector<float>& ratios, 
	QVector<QRect>* anchors) {
	anchors->clear();
	QPoint center = QPoint(anchor.x() + (anchor.width() - 1) * 0.5f,
		anchor.y() + (anchor.height() - 1) * 0.5f);
	float anchor_size = anchor.width() * anchor.height();
//#if defined(_OPENMP)
//#pragma omp parallel for num_threads(threads_num)
//#endif
	for (int i = 0; i < static_cast<int>(ratios.size()); ++i) {
		float ratio = ratios.at(i);
		float anchor_size_ratio = anchor_size / ratio;
		float curr_anchor_width = std::sqrt(anchor_size_ratio);
		float curr_anchor_height = curr_anchor_width * ratio;
		float curr_x = center.x() - (curr_anchor_width - 1) * 0.5f;
		float curr_y = center.y() - (curr_anchor_height - 1) * 0.5f;

		QRect curr_anchor = QRect(curr_x, curr_y,
			curr_anchor_width - 1, curr_anchor_height - 1);
		anchors->push_back(curr_anchor);
	}
	return 0;
}

int CDetectorNcnnRetina::ScaleAnchors(const QVector<QRect>& ratio_anchors,
	const QVector<float>& scales, QVector<QRect>* anchors) {
	anchors->clear();
//#if defined(_OPENMP)
//#pragma omp parallel for num_threads(threads_num)
//#endif
	for (int i = 0; i < static_cast<int>(ratio_anchors.size()); ++i) {
		QRect anchor = ratio_anchors.at(i);
		QPointF center = QPointF(anchor.x() + anchor.width() * 0.5f,
			anchor.y() + anchor.height() * 0.5f);
		for (int j = 0; j < static_cast<int>(scales.size()); ++j) {
			float scale = scales.at(j);
			float curr_width = scale * (anchor.width() + 1);
			float curr_height = scale * (anchor.height() + 1);
			float curr_x = center.x() - curr_width * 0.5f;
			float curr_y = center.y() - curr_height * 0.5f;
			QRect curr_anchor = QRect(curr_x, curr_y,
				curr_width, curr_height);
			anchors->push_back(curr_anchor);
		}
	}

	return 0;
}

int CDetectorNcnnRetina::GenerateAnchors(const int & base_size,
	const QVector<float>& ratios, 
	const QVector<float> scales,
	QVector<QRect>* anchors) {
	anchors->clear();
	QRect anchor = QRect(0, 0, base_size, base_size);
	QVector<QRect> ratio_anchors;
	RatioAnchors(anchor, ratios, &ratio_anchors);
	ScaleAnchors(ratio_anchors, scales, anchors);
	return 0;
}
