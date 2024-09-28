#include "view.h"

#include <QMessageBox>
#include <QSize>

namespace s21 {

View::View(QWidget *parent)
    : QOpenGLWidget(parent),
      ui_(new Ui::View),
      model_controller(new ModelController(ui_)) {
  ui_->setupUi(this);
  this->resize(1200, 670);
  ui_->tabWidget_save_format->tabBar()->hide();
  setAcceptDrops(true);
  InitDefaultSettings_();
  ConnectSlotSignals_();
  last_settings_ = new QSettings("SAVE_3DVIEWER", "3DViewer", this);
  RestoreSettings_();
  expandAnimation_ = new QPropertyAnimation(this, "size");
  expandAnimation_->setDuration(500);
}

View::~View() {
  SaveSettings_();
  delete last_settings_;
  delete model_controller;
  delete ui_;
}

void View::ConnectSlotSignals_() {
  connect(ui_->pushButton_open_file, SIGNAL(clicked()), this,
          SLOT(OpenFileBtnClicked_()));
  connect(ui_->saveAsButton, SIGNAL(clicked()), this,
          SLOT(ChangeSaveFormatTab_()));
  connect(ui_->buttonGroupImg, SIGNAL(buttonClicked(QAbstractButton *)), this,
          SLOT(Record_(QAbstractButton *)));
  std::vector<QSlider *> transform_sliders = {
      ui_->horizontalSlider_move_x,   ui_->horizontalSlider_move_y,
      ui_->horizontalSlider_move_z,   ui_->horizontalSlider_rotate_x,
      ui_->horizontalSlider_rotate_y, ui_->horizontalSlider_rotate_z,
      ui_->horizontalSlider_scale};
  for (auto slider : transform_sliders) {
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(TransformModel_()));
  }
  connect(ui_->horizontalSlider_vert_size, SIGNAL(valueChanged(int)), this,
          SLOT(update()));
  connect(ui_->horizontalSlider_edges_thick, SIGNAL(valueChanged(int)), this,
          SLOT(update()));
  connect(ui_->comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(update()));
  connect(ui_->comboBox_disp_method, SIGNAL(currentIndexChanged(int)), this,
          SLOT(update()));
  connect(ui_->comboBox_disp_method_2, SIGNAL(currentIndexChanged(int)), this,
          SLOT(update()));
  connect(ui_->buttonGroupColors, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(SetColor_(QAbstractButton *)));
  connect(ui_->expandButton, SIGNAL(clicked()), this, SLOT(ExpandView()));
  connect(ui_->centerModel, SIGNAL(clicked()), this, SLOT(centerModel()));
}

void View::InitDefaultSettings_() {
  QColor defaultBgColor(Qt::black);
  QColor defaultEdgeColor(Qt::blue);
  QColor defaultVertexColor(Qt::white);
  settings_.bg_color.setNamedColor(defaultBgColor.name());
  settings_.edge_color.setNamedColor(defaultEdgeColor.name());
  settings_.vertex_color.setNamedColor(defaultVertexColor.name());
  ui_->tabWidget->setCurrentIndex(0);
}

void View::SetColor_(QAbstractButton *button) {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    QString prev_style(
        "border-radius: 3px;\nborder: 1px solid rgb(0, 128, 0);\n");
    if (button == ui_->pushButton_bg_color) {
      settings_.bg_color = color;
      ui_->pushButton_bg_color->setStyleSheet(
          QString(prev_style + "background-color: %1")
              .arg(settings_.bg_color.name()));
    } else if (button == ui_->pushButton_edges_color) {
      settings_.edge_color = color;
      ui_->pushButton_edges_color->setStyleSheet(
          QString(prev_style + "background-color: %1")
              .arg(settings_.edge_color.name()));
    } else if (button == ui_->pushButton_vertex_color) {
      settings_.vertex_color = color;
      ui_->pushButton_vertex_color->setStyleSheet(
          QString(prev_style + "background-color: %1")
              .arg(settings_.vertex_color.name()));
    }
    update();
  }
}

void View::ChangeSaveFormatTab_() {
  ui_->tabWidget_save_format->setCurrentIndex(1);
}

void View::ClearSliders_() {
  QSlider *sliders[] = {
      ui_->horizontalSlider_move_x,   ui_->horizontalSlider_move_y,
      ui_->horizontalSlider_move_z,   ui_->horizontalSlider_rotate_x,
      ui_->horizontalSlider_rotate_y, ui_->horizontalSlider_rotate_z,
  };
  for (auto slider : sliders) {
    slider->setValue(0);
  }
  ui_->horizontalSlider_scale->setValue(100);
}

void View::OpenFileBtnClicked_() {
  QString sourceCodeDir = QCoreApplication::applicationDirPath();
#ifdef Q_OS_MACOS
  QDir modelsDir(sourceCodeDir + "/../../../../models");
#else
  QDir modelsDir(sourceCodeDir + "/../models");
#endif
  QString filepath = QFileDialog::getOpenFileName(
      this, ("Select Model"), modelsDir.path(), "3D Image Files (*.obj)");
  if (!filepath.isEmpty()) {
    ClearSliders_();
    emit SetModel(filepath);
    ui_->label_filename->setText(QFileInfo(filepath).fileName());
  }
}

void View::HandleSolution_(std::vector<int> *vertex_index,
                           std::vector<double> *vertex_coord) {
  vertex_index_ = vertex_index->data();
  vertex_coord_ = vertex_coord->data();
  count_vertex_index_ = static_cast<int>(vertex_index->size());
  count_vertex_coord_ = static_cast<int>(vertex_coord->size());
  QString info = "Vertices: %1\nEdges: %2";
  ui_->label_file_info->setText(
      info.arg(count_vertex_coord_ / 3).arg(count_vertex_index_ / 4));
  update();
}

void View::ExpandView() {
#ifdef Q_OS_MACOS
  QSize targetSize(1200, 800);
  QSize currentSize = size();
  if (currentSize == targetSize) {
    expandAnimation_->setStartValue(targetSize);
    expandAnimation_->setEndValue(QSize(1200, 670));
  } else {
    expandAnimation_->setStartValue(currentSize);
    expandAnimation_->setEndValue(targetSize);
  }
  expandAnimation_->start();
#else
  static bool expanded = false;
  if (expanded) {
    setFixedSize(QSize(1200, 670));
  } else {
    setFixedSize(QSize(1200, 800));
  }
  expanded = !expanded;
#endif
}

void View::centerModel() { model_controller->ResetTransformation(); }

void View::HandleError_() {
  vertex_index_ = nullptr;
  vertex_coord_ = nullptr;
  count_vertex_index_ = 0;
  count_vertex_coord_ = 0;
  update();
  QMessageBox::critical(nullptr, "Error", "Invalid Model");
}

void View::TransformModel_() {
  auto *slider = qobject_cast<QSlider *>(sender());
  model_controller->Transform(slider);
  update();
}

void View::Record_(QAbstractButton *button) {
  if (button == ui_->pushButton_bmp)
    SaveBmp_();
  else if (button == ui_->pushButton_jpeg)
    SaveJpeg_();
  else if (button == ui_->pushButton_gif)
    SaveGif_();
}

}  // namespace s21
