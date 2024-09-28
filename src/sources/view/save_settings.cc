#include "view.h"

namespace s21 {

void View::SaveSettings_() {
  last_settings_->setValue("background_color", settings_.bg_color.name());
  last_settings_->setValue("edge_color", settings_.edge_color.name());
  last_settings_->setValue("vertex_color", settings_.vertex_color.name());

  if (ui_->comboBox_disp_method_2->currentText() == "Parallel") {
    last_settings_->setValue("projection_type", kParallel);
  } else {
    last_settings_->setValue("projection_type", kCentral);
  }

  last_settings_->setValue("edge_thickness",
                           ui_->horizontalSlider_edges_thick->value());
  if (ui_->comboBox->currentText() == "Solid") {
    last_settings_->setValue("edge_type", kSolid);
  } else {
    last_settings_->setValue("edge_type", kDashed);
  }

  last_settings_->setValue("vertex_size",
                           ui_->horizontalSlider_vert_size->value());
  if (ui_->comboBox_disp_method->currentText() == "Circle") {
    last_settings_->setValue("vertex_type", kCircle);
  } else if (ui_->comboBox_disp_method->currentText() == "Square") {
    last_settings_->setValue("vertex_type", kSquare);
  } else {
    last_settings_->setValue("vertex_type", kNoVertex);
  }
}

void View::RestoreSettings_() {
  QString tempBackgroundColor =
      last_settings_->value("background_color", -1).toString();
  QString tempEdgeColor = last_settings_->value("edge_color", -1).toString();
  QString tempVertexColor =
      last_settings_->value("vertex_color", -1).toString();
  int tempEdgeThickness = last_settings_->value("edge_thickness", -1).toInt();
  int tempVertexSize = last_settings_->value("vertex_size", -1).toInt();

  QString prev_style(
      "border-radius: 3px;\nborder: 1px solid rgb(110, 105, 157);\n");
  if (tempBackgroundColor != "-1") {
    settings_.bg_color.setNamedColor(tempBackgroundColor);
    QColor color(tempBackgroundColor);
    ui_->pushButton_bg_color->setStyleSheet(
        QString(prev_style + "background-color: %1").arg(color.name()));
  }
  if (tempEdgeColor != "-1") {
    settings_.edge_color.setNamedColor(tempEdgeColor);
    QColor color(tempEdgeColor);
    ui_->pushButton_edges_color->setStyleSheet(
        QString(prev_style + "background-color: %1").arg(color.name()));
  }
  if (tempVertexColor != "-1") {
    settings_.vertex_color.setNamedColor(tempVertexColor);
    QColor color(tempVertexColor);
    ui_->pushButton_vertex_color->setStyleSheet(
        QString(prev_style + "background-color: %1").arg(color.name()));
  }

  if (last_settings_->value("projection_type").toInt() == kParallel)
    ui_->comboBox_disp_method_2->setCurrentIndex(0);
  if (last_settings_->value("projection_type").toInt() == kCentral)
    ui_->comboBox_disp_method_2->setCurrentIndex(1);

  if (tempEdgeThickness != -1) {
    ui_->horizontalSlider_edges_thick->setValue(tempEdgeThickness);
  }
  if (last_settings_->value("edge_type").toInt() == kSolid) {
    ui_->comboBox->setCurrentIndex(0);
  }
  if (last_settings_->value("edge_type").toInt() == kDashed) {
    ui_->comboBox->setCurrentIndex(1);
  }

  if (tempVertexSize != -1) {
    ui_->horizontalSlider_vert_size->setValue(tempVertexSize);
  }
  if (last_settings_->value("vertex_type").toInt() == kCircle) {
    ui_->comboBox_disp_method->setCurrentIndex(0);
  }
  if (last_settings_->value("vertex_type").toInt() == kSquare) {
    ui_->comboBox_disp_method->setCurrentIndex(1);
  }
  if (last_settings_->value("vertex_type").toInt() == kNoVertex) {
    ui_->comboBox_disp_method->setCurrentIndex(2);
  }
}

}  // namespace s21
