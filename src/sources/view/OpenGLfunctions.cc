#include "view.h"

namespace s21 {

void View::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void View::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void View::paintGL() {
  glClearColor(settings_.bg_color.redF(), settings_.bg_color.greenF(),
               settings_.bg_color.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  UpdateValues_();
  if (settings_.projection_type) {
    glVertexPointer(3, GL_DOUBLE, 0, vertex_coord_);
    glEnableClientState(GL_VERTEX_ARRAY);
    SetProjectionType_();

    if (settings_.projection_type == kParallel) {
      glRotated(15, 1, 0, 0);
      glRotated(15, 0, 1, 0);
      glRotated(1, 0, 0, 1);
    }

    SetEdges_();
    if (settings_.point_visibility == kAVertex) SetVertices_();
    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

void View::UpdateValues_() {
  settings_.projection_type =
      ui_->comboBox_disp_method_2->currentText() == "Parallel" ? kParallel
                                                               : kCentral;
  settings_.edge_type =
      ui_->comboBox->currentText() == "Solid" ? kSolid : kDashed;

  settings_.edge_width = ui_->horizontalSlider_edges_thick->value();
  settings_.point_size = ui_->horizontalSlider_vert_size->value();

  if (ui_->comboBox_disp_method->currentText() == "None") {
    settings_.point_visibility = kNoVertex;
  } else {
    settings_.point_visibility = kAVertex;

    settings_.point_type = ui_->comboBox_disp_method->currentText() == "Circle"
                               ? kCircle
                               : kSquare;
  }
}

void View::SetEdges_() {
  if (settings_.edge_type == kDashed) {
    glLineStipple(1, 0x3333);
    glEnable(GL_LINE_STIPPLE);
  } else if (settings_.edge_type == kSolid) {
    glDisable(GL_LINE_STIPPLE);
  }

  glLineWidth(settings_.edge_width);
  glColor3f(settings_.edge_color.redF(), settings_.edge_color.greenF(),
            settings_.edge_color.blueF());
  glDrawElements(GL_LINES, count_vertex_index_, GL_UNSIGNED_INT, vertex_index_);
}

void View::SetVertices_() {
  glPointSize(settings_.point_size);
  glColor3f(settings_.vertex_color.redF(), settings_.vertex_color.greenF(),
            settings_.vertex_color.blueF());

  if (settings_.point_type == kCircle) {
    glEnable(GL_POINT_SMOOTH);
    glDrawArrays(GL_POINTS, 0, count_vertex_coord_ / 3);
    glDisable(GL_POINT_SMOOTH);
  } else {
    glDrawArrays(GL_POINTS, 0, count_vertex_coord_ / 3);
  }
}

void View::SetProjectionType_() {
  if (settings_.projection_type == kParallel) {
    // (left, right, bottom, top, near, far)
    glOrtho(-7, 14.5, -7, 7, -50, 50);
    glTranslated(2, -2, -15);
  } else if (settings_.projection_type == kCentral) {
    glFrustum(2.1, -4, -2, 2, 2, 30);
    glTranslated(-3.5, 0, -10);
  }
}

}  // namespace s21
