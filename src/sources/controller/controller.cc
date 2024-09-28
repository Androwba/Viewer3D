#include "controller.h"

namespace s21 {

void Controller::SetFileName_(const QString& file_name) {
  model_->ParseObj(file_name.toStdString());

  if (model_->GetError() == Error::kOk) {
    emit SolutionReady(GetVertexIndex(), GetVertexCoord());
  } else {
    emit ErrorHasOccured();
  }
}

std::vector<int>* Controller::GetVertexIndex() {
  return model_->GetVertexIndex();
}

std::vector<double>* Controller::GetVertexCoord() {
  return model_->GetVertexCoord();
}

void Controller::Transform_(Action action, double value, Axis axis) {
  model_->Transform(action, value, axis);
}

}  // namespace s21
