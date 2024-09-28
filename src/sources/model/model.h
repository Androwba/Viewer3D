#ifndef CPP4_3DVIEWER_V2_0_MODEL_MODEL_H_
#define CPP4_3DVIEWER_V2_0_MODEL_MODEL_H_

#include <fstream>
#include <sstream>

#include "tranformation.h"

namespace s21 {
using string = std::string;

enum class Error {
  kOk,
  kFileWrongExtension,
  kFailedToOpen,
  kIncorrectData,
};

class Model {
 public:
  void ParseObj(string filename);
  void Transform(Action& action, double& value, Axis& axis);

  Error GetError();
  std::vector<int>* GetVertexIndex();
  std::vector<double>* GetVertexCoord();
  static Model& GetInstance();

 private:
  void ParseVertex_(const string& line);
  void ParseEdges_(const string& line);
  void Normalize_();
  void CheckFile_(const string& file_name);
  void ClearObj_();

  std::vector<double> vertexCoords_{};
  std::vector<int> vertexIndices_{};
  Error error_code_{};
  Transformer transformer_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_MODEL_MODEL_H_
