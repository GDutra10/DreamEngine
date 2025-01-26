#ifndef EDITOR_IMPORTERS_BASE_MODEL_IMPORTER_H_
#define EDITOR_IMPORTERS_BASE_MODEL_IMPORTER_H_

#include <string>
#include "../Models/Model.h"

namespace DreamEngine::Editor::Importers
{
using namespace DreamEngine::Editor::Models;
class BaseModelImporter
{
   public:
    virtual Model& Import(std::string const& path, bool gamma = false) = 0;
};

}  // namespace DreamEngine::Editor::Helpers
#endif