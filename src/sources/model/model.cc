#include "model.h"

#include "tranformation.h"

namespace s21 {

void Model::ParseObj(string filename) {
  CheckFile_(filename);
  if (error_code_ != Error::kFileWrongExtension) {
    ClearObj_();
    std::ifstream file(filename);
    string line;

    if (!file.is_open()) {
      error_code_ = Error::kFailedToOpen;
      return;
    }

    while (error_code_ == Error::kOk && std::getline(file, line)) {
      if (!line.compare(0, 2, "v ")) {
        ParseVertex_(line);
      } else if (!line.compare(0, 2, "f ")) {
        ParseEdges_(string(line.begin() + 2, line.end()));
      }
    }
    Normalize_();
    file.close();
  }
}

void Model::ParseVertex_(const string& line) {
  double d1 = 0, d2 = 0, d3 = 0;
  char j = 0;
  if (sscanf(line.c_str(), "%c %lf %lf %lf %c", &j, &d1, &d2, &d3, &j) == 4) {
    vertexCoords_.insert(vertexCoords_.end(), {d1, d2, d3});
  } else {
    error_code_ = Error::kIncorrectData;
  }
}

void Model::ParseEdges_(const string& line) {
  std::istringstream iss(line);
  std::string token;
  int first = 0, first_index = 0, index = 0;

  while (std::getline(iss, token, ' ')) {
    sscanf(token.c_str(), "%d", &index);

    if (index <= 0 || token.find('\r') != std::string::npos) {
      continue;
    }
    --index;

    if (first == 0) {
      vertexIndices_.push_back(index);
      first_index = index;
      first = 1;
    } else {
      vertexIndices_.insert(vertexIndices_.end(), {index, index});
    }
  }
  vertexIndices_.push_back(first_index);
}

void Model::CheckFile_(const string& file_name) {
  if (file_name.size() <= 3 ||
      file_name.compare(file_name.size() - 4, 4, ".obj")) {
    error_code_ = Error::kFileWrongExtension;
  } else {
    error_code_ = Error::kOk;
  }
}

void Model::ClearObj_() {
  vertexCoords_.clear();
  vertexIndices_.clear();
}

Error Model::GetError() { return error_code_; }

std::vector<int>* Model::GetVertexIndex() { return &vertexIndices_; }

std::vector<double>* Model::GetVertexCoord() { return &vertexCoords_; }

void Model::Transform(Action& action, double& value, Axis& axis) {
  transformer_.Action(action)->Transform(vertexCoords_, value, axis);
}

void Model::Normalize_() {
  double max_value = 0;
  for (double value : vertexCoords_) {
    if (value > max_value) max_value = value;
  }
  if (max_value > 10) {
    for (double& value : vertexCoords_) {
      value = value / max_value;
    }
  }
}

Model& Model::GetInstance() {
  static Model instance;
  return instance;
}

}  // namespace s21
