
#pragma once
#include <rapidjson/document.h>

#include "save_editor_exports.hpp"

void PATHFINDER_SAVE_EDITOR_API print(rapidjson::Document &json);

rapidjson::Document PATHFINDER_SAVE_EDITOR_API getDocumentFromPath(const std::string &path);

void PATHFINDER_SAVE_EDITOR_API printDocumentToPath(const std::string &path, rapidjson::Document &json);
