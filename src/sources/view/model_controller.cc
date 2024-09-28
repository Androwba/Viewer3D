#include "model_controller.h"

namespace s21 {

void ModelController::MoveModel(QString &slider_name) {
  double rate = 0;
  if (slider_name == "horizontalSlider_move_x") {
    rate = ui_->horizontalSlider_move_x->value() / 30.0;
    emit SetTransform(s21::Action::move, rate - transform_info_.move_x,
                      Axis::kX);
    transform_info_.move_x = rate;
  } else if (slider_name == "horizontalSlider_move_y") {
    rate = ui_->horizontalSlider_move_y->value() / 30.0;
    emit SetTransform(Action::move, rate - transform_info_.move_y, Axis::kY);
    transform_info_.move_y = rate;
  } else if (slider_name == "horizontalSlider_move_z") {
    rate = ui_->horizontalSlider_move_z->value() / 30.0;
    emit SetTransform(Action::move, rate - transform_info_.move_z, Axis::kZ);
    transform_info_.move_z = rate;
  }
}

void ModelController::RotateModel(QString &slider_name) {
  if (slider_name == "horizontalSlider_rotate_x") {
    emit SetTransform(
        Action::rotate,
        ui_->horizontalSlider_rotate_x->value() - transform_info_.rotate_x,
        Axis::kX);
    transform_info_.rotate_x = ui_->horizontalSlider_rotate_x->value();
  } else if (slider_name == "horizontalSlider_rotate_y") {
    emit SetTransform(
        Action::rotate,
        ui_->horizontalSlider_rotate_y->value() - transform_info_.rotate_y,
        Axis::kY);
    transform_info_.rotate_y = ui_->horizontalSlider_rotate_y->value();
  } else if (slider_name == "horizontalSlider_rotate_z") {
    emit SetTransform(
        Action::rotate,
        ui_->horizontalSlider_rotate_z->value() - transform_info_.rotate_z,
        Axis::kZ);
    transform_info_.rotate_z = ui_->horizontalSlider_rotate_z->value();
  }
}

void ModelController::ScaleModel(QString &slider_name) {
  if (slider_name == "horizontalSlider_scale") {
    double rate = pow(10, ui_->horizontalSlider_scale->value() / 60.0) / 100;
    if (transform_info_.scale == 0) {
      transform_info_.scale = rate;
    }
    emit SetTransform(Action::scale, rate / transform_info_.scale, Axis::kX);
    transform_info_.scale = rate;
  }
}

void ModelController::Transform(QSlider *slider) {
  QString slider_name = slider->objectName();

  MoveModel(slider_name);
  RotateModel(slider_name);
  ScaleModel(slider_name);
}

void ModelController::ResetTransformation() {
  QSlider *sliders[] = {
      ui_->horizontalSlider_move_x,   ui_->horizontalSlider_move_y,
      ui_->horizontalSlider_move_z,   ui_->horizontalSlider_rotate_x,
      ui_->horizontalSlider_rotate_y, ui_->horizontalSlider_rotate_z,
      ui_->horizontalSlider_scale};
  const int defaultValues[] = {0, 0, 0, 0, 0, 0, 100};
  for (int i = 0; i < sizeof(sliders) / sizeof(sliders[0]); ++i) {
    sliders[i]->setValue(defaultValues[i]);
  }
  for (int i = 0; i < 3; ++i) {
    emit SetTransform(Action::move, -transform_info_.move_x,
                      static_cast<Axis>(i));
    emit SetTransform(Action::rotate, -transform_info_.rotate_x,
                      static_cast<Axis>(i));
  }
  double scaleReset = 1.0 / transform_info_.scale;
  emit SetTransform(Action::scale, scaleReset, Axis::kX);
  transform_info_ = {0, 0, 0, 0, 0, 0, 1.0};
}

}  // namespace s21
