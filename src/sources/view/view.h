#ifndef CPP4_3DVIEWER_V2_0_VIEW_VIEW_H_
#define CPP4_3DVIEWER_V2_0_VIEW_VIEW_H_

#include <QButtonGroup>
#include <QColor>
#include <QColorDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPalette>
#include <QPropertyAnimation>
#include <QSettings>
#include <QTimer>
#include <QWheelEvent>
#include <QWidget>

#include "QtGifImage/gifimage/qgifimage.h"
#include "model_controller.h"
#include "ui_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}  // namespace Ui
QT_END_NAMESPACE

namespace s21 {

enum Settings {
  kParallel = 1,
  kCentral = 2,
  kSolid = 1,
  kDashed = 2,
  kCircle = 1,
  kSquare = 2,
  kNoVertex = 3,
  kAVertex = 4
};

struct TransformInfo {
  double move_x;
  double move_y;
  double move_z;
  int rotate_x;
  int rotate_y;
  int rotate_z;
  double scale;
};

struct SettingsInfo {
  int edge_type;
  int point_type;
  float edge_width;
  float point_size;
  int projection_type;
  int point_visibility;
  QColor edge_color;
  QColor vertex_color;
  QColor bg_color;
};

class View : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit View(QWidget* parent = nullptr);
  ~View() override;

 signals:
  void SetModel(QString file_name);

 public slots:
  void HandleSolution_(std::vector<int>* vertex_index_,
                       std::vector<double>* vertex_coord_);
  void HandleError_();
  void ExpandView();

 private slots:
  void OpenFileBtnClicked_();
  void ClearSliders_();
  void TransformModel_();
  void Record_(QAbstractButton* button);
  void SetColor_(QAbstractButton* button);
  void ChangeSaveFormatTab_();
  void SaveImage_();
  void centerModel();

 private:
  Ui::View* ui_;
  int* vertex_index_{nullptr};
  QPropertyAnimation* expandAnimation_;
  double* vertex_coord_{nullptr};
  int count_vertex_index_{};
  int count_vertex_coord_{};

  // Model rendering methods
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void UpdateValues_();
  void SetProjectionType_();
  void SetEdges_();
  void SetVertices_();

  // Methods for saving and restoring settings_
  struct SettingsInfo settings_;
  QSettings* last_settings_;
  void SaveSettings_();
  void RestoreSettings_();

  // Methods for saving images
  void SaveGif_();
  void SaveBmp_();
  void SaveJpeg_();

  // GIF
  QGifImage* gif_;
  QTimer* timer_;
  int frames_;

  // Events
  QPoint click_pos_;
  bool left_btn_pressed_;
  bool right_btn_pressed;
  void mouseMoveEvent(QMouseEvent* cursor_position) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* cursor_position) override;
  void wheelEvent(QWheelEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;

  // Other
  void InitDefaultSettings_();
  void ConnectSlotSignals_();

 public:
  ModelController* model_controller;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_VIEW_H_
