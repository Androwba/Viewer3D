#include "tranformation.h"

namespace s21 {

void Move::Transform(std::vector<double> &vertex_coords, double &step,
                     Axis &axis) {
  for (std::size_t i = static_cast<std::size_t>(axis); i < vertex_coords.size();
       i += 3) {
    vertex_coords[i] += step;
  }
}

void Rotate::Transform(std::vector<double> &vertex_coords, double &angle,
                       Axis &axis) {
  double tempAngle = angle * M_PI / 180;
  double cosValue = cos(tempAngle);
  double sinValue = sin(tempAngle);

  for (std::size_t i = (axis == Axis::kX) ? 1 : 0; i < vertex_coords.size();
       i += 3) {
    double coord = vertex_coords[i];

    if (axis == Axis::kX) {
      vertex_coords[i] = cosValue * coord - sinValue * vertex_coords[i + 1];
      vertex_coords[i + 1] = sinValue * coord + cosValue * vertex_coords[i + 1];
    } else if (axis == Axis::kY) {
      vertex_coords[i] = cosValue * coord + sinValue * vertex_coords[i + 2];
      vertex_coords[i + 2] =
          -sinValue * coord + cosValue * vertex_coords[i + 2];
    } else if (axis == Axis::kZ) {
      vertex_coords[i] = cosValue * coord + sinValue * vertex_coords[i + 1];
      vertex_coords[i + 1] =
          -sinValue * coord + cosValue * vertex_coords[i + 1];
    }
  }
}

void Scale::Transform(std::vector<double> &vertex_coords, double &scale,
                      Axis &) {
  for (double &i : vertex_coords) {
    i *= scale;
  }
}

Transformer *Transformer::Action(enum Action action) {
  action_ = action;
  return this;
}

void Transformer::Transform(std::vector<double> &vertex_coords, double &value,
                            Axis &axis) {
  switch (action_) {
    case Action::move:
      move_.Transform(vertex_coords, value, axis);
      break;
    case Action::rotate:
      rotate_.Transform(vertex_coords, value, axis);
      break;
    case Action::scale:
      scale_.Transform(vertex_coords, value, axis);
      break;
  }
}

}  // namespace s21
