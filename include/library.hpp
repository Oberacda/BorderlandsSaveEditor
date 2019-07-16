
#pragma once
#include <rapidjson/document.h>

#include "bl_save_editor_exports.hpp"

void BORDERLANDS_SAVE_EDITOR_API print(rapidjson::Document &json);

rapidjson::Document BORDERLANDS_SAVE_EDITOR_API getDocumentFromPath(const std::string &path);

void BORDERLANDS_SAVE_EDITOR_API printDocumentToPath(const std::string &path, rapidjson::Document &json);
