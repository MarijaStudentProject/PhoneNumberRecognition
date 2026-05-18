#include <filesystem>
#include <fstream>
#include <string>

class TempMetaFile {
  public:
    TempMetaFile() {
        static int counter = 0;
        path = std::filesystem::temp_directory_path() / ("vcf_meta_test_" + std::to_string(counter++) + ".json");
        std::ofstream out(path);
        out << R"json({
            "countries": {
                "RS": ["381", "00", "0"],
                "US": ["1",   "011", "1"],
                "DE": ["49",  "00",  "0"]
            }
        })json";
    }
    ~TempMetaFile() {
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }
    std::string filename() const { return path.string(); }

  private:
    std::filesystem::path path;
};
