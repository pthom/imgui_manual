#include "source_parse/ImGuiDemoParser.h"
#include <fplus/fplus.hpp>
#include <iostream>

// make_example_apps is an exe called by cmake that  will populate
// the folder playground/ExampleApps with the example apps from imgui_demo.cpp
// it will thus create 13 cpp files there
using namespace SourceParse;
using namespace std::literals;

std::string APPS_FOLDER = std::string(IMGUI_MANUAL_SOURCE_DIR) + "/playground/ExampleApps/";

int main()
{
    std::unordered_map<std::string, SourceCode> exampleApps = FindExampleAppsCode();
    for (const auto& exampleApp: exampleApps)
    {
        std::cout << "Processing " << exampleApp.first << "\n";
        std::string templateFile = APPS_FOLDER + "ExampleApp.cpp.template";
        std::string cppFile = APPS_FOLDER + exampleApp.first + ".cpp";
        auto code = fplus::read_text_file(templateFile)();
        assert(!code.empty());
        code = fplus::replace_tokens("{ExampleAppName}"s, exampleApp.first, code);
        code = fplus::replace_tokens("{ExampleAppCode}"s, exampleApp.second, code);

        if (exampleApp.first == "ExampleAppMainMenuBar")
            continue;

        assert(fplus::write_text_file(cppFile, code)());
        std::cout << templateFile << " --> " << cppFile << "\n";
    }
}