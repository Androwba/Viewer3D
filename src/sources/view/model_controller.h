#ifndef CPP4_3DVIEWER_V2_0_MODEL_CONTROLLER_H_
#define CPP4_3DVIEWER_V2_0_MODEL_CONTROLLER_H_

#include <QObject>
#include <QSlider>
#include <QString>

#include "model/tranformation.h"
#include "ui_view.h"

namespace s21 {

class ModelController : public QObject {
  Q_OBJECT

 public:
  ModelController() : ui_(nullptr) {}
  explicit ModelController(Ui::View* ui) : ui_(ui) {}

  void MoveModel(QString& slider_name);
  void RotateModel(QString& slider_name);
  void ScaleModel(QString& slider_name);
  void Transform(QSlider* slider);

 public slots:
  void ResetTransformation();

 signals:
  void SetTransform(Action action, double value, Axis axis);

 private:
  Ui::View* ui_;
  struct TransformInfo {
    double move_x;
    double move_y;
    double move_z;
    int rotate_x;
    int rotate_y;
    int rotate_z;
    double scale;
  } transform_info_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_MODEL_CONTROLLER_H_
