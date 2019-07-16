#include <iostream>
#include <fstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "library.hpp"

void print(rapidjson::Document &json) {
	using namespace rapidjson;

	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	json.Accept(writer);
	auto str = sb.GetString();
	printf("%s\n", str);
}

rapidjson::Document getDocumentFromPath(const std::string &path) {
	std::ifstream myfile(path);
	rapidjson::IStreamWrapper isw(myfile);

	rapidjson::Document document;
	document.ParseStream(isw);

	return document;
}

void printDocumentToPath(const std::string &path, rapidjson::Document &document) {
	std::ofstream o_file(path);
	rapidjson::OStreamWrapper osw(o_file);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	document.Accept(writer);
}