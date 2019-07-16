
#pragma once
#include <rapidjson/document.h>

#include "bl2_save_editor_exports.hpp"

void BORDERLANDS2_SAVE_EDITOR_API print(rapidjson::Document &json);

rapidjson::Document BORDERLANDS2_SAVE_EDITOR_API getDocumentFromPath(const std::string &path);

void BORDERLANDS2_SAVE_EDITOR_API printDocumentToPath(const std::string &path, rapidjson::Document &json);
