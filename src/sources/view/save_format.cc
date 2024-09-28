#include "view.h"

namespace s21 {

void View::SaveGif_() {
  ui_->tabWidget_save_format->setCurrentIndex(0);
  frames_ = 0;
  gif_ = new QGifImage;
  timer_ = new QTimer(this);
  gif_->setDefaultDelay(100);
  connect(timer_, SIGNAL(timeout()), this, SLOT(SaveImage_()));
  timer_->start(100);
}

void View::SaveImage_() {
  QPixmap pix = QPixmap::fromImage(grabFramebuffer());
  QPixmap scaledPix = pix.scaled(QSize(640, 480), Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);
  gif_->addFrame(scaledPix.toImage());

  if (frames_ == 50) {
    timer_->stop();
    QString pathGIF = QFileDialog::getSaveFileName(
        this, ("Save as GIF"), "image.gif", "GIF Image Files (*.gif)");
    gif_->save(pathGIF);
    delete timer_;
    delete gif_;
  }
  frames_++;
}

void View::SaveBmp_() {
  ui_->tabWidget_save_format->setCurrentIndex(0);
  QString pathScreenshot = QFileDialog::getSaveFileName(
      this, ("Save as BMP"), "image.bmp", "BMP Image Files (*.bmp)");

  grabFramebuffer().save(pathScreenshot, "bmp");
}

void View::SaveJpeg_() {
  ui_->tabWidget_save_format->setCurrentIndex(0);
  QString pathScreenshot = QFileDialog::getSaveFileName(
      this, ("Save as JPEG"), "image.jpeg", "JPEG Image Files (*.jpeg)");

  grabFramebuffer().save(pathScreenshot, "jpeg");
}

}  // namespace s21
