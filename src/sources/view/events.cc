#include "view.h"

namespace s21 {

void View::mouseMoveEvent(QMouseEvent *cursor_position) {
  if (left_btn_pressed_) {
    int rotate_x = (cursor_position->pos().y() - click_pos_.y()) / 3;
    int rotate_y = (cursor_position->pos().x() - click_pos_.x()) / 3;
    ui_->horizontalSlider_rotate_x->setValue(
        ui_->horizontalSlider_rotate_x->value() + rotate_x);
    ui_->horizontalSlider_rotate_y->setValue(
        ui_->horizontalSlider_rotate_y->value() + rotate_y);

  } else if (right_btn_pressed) {
    int move_x = (cursor_position->pos().x() - click_pos_.x()) / 2;
    int move_y = (click_pos_.y() - cursor_position->pos().y()) / 2;
    ui_->horizontalSlider_move_x->setValue(
        ui_->horizontalSlider_move_x->value() + move_x);
    ui_->horizontalSlider_move_y->setValue(
        ui_->horizontalSlider_move_y->value() + move_y);
  }

  click_pos_.setX(cursor_position->pos().x());
  click_pos_.setY(cursor_position->pos().y());
  update();
}

void View::mousePressEvent(QMouseEvent *event) {
  click_pos_ = event->pos();
  if (event->button() == Qt::LeftButton) {
    left_btn_pressed_ = true;
  } else if (event->button() == Qt::RightButton) {
    right_btn_pressed = true;
  }
}

void View::mouseReleaseEvent(QMouseEvent *cursor_position) {
  left_btn_pressed_ = false;
  right_btn_pressed = false;
  click_pos_ = cursor_position->pos();
}

void View::wheelEvent(QWheelEvent *event) {
  int offset = 0;
  if (event->angleDelta().y() > 0) {
    offset = 6;
  } else if (event->angleDelta().y() < 0) {
    offset = -6;
  }
  ui_->horizontalSlider_scale->setValue(ui_->horizontalSlider_scale->value() +
                                        offset);
}

void View::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls()) event->acceptProposedAction();
}

void View::dropEvent(QDropEvent *event) {
  const QMimeData *mime_data = event->mimeData();

  if (mime_data->hasUrls()) {
    QList<QUrl> url_list = mime_data->urls();
    QString filepath = url_list[0].toLocalFile();

    ClearSliders_();
    emit SetModel(filepath);
    ui_->label_filename->setText((QFileInfo(filepath)).fileName());
    event->acceptProposedAction();
  }
}

}  // namespace s21
