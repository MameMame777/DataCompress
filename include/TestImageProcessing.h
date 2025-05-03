#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <unordered_map>
#include <string>

namespace Testing {
  class TestImageProcessing {
    public:
      enum class colorfilename{
        Aerial,
        Airplane,
        Balloon,
        checker,
        couple,
        Earth,
        Girl,
        Lenna,
        Mandrill,
        milkdrop,
        Parrots,
        Pepper,
        Sailboat
      };
      enum class monofilename{
        Airplane,
        BARBARA,
        BOAT,
        BRIDGE,
        Building,
        Cameraman,
        checker_8bit,
        checker,
        couple_BW,
        girl,
        LAX,
        LENNA,
        Lighthouse,
        Text,
        WOMAN
      };
      const std::unordered_map<colorfilename, std::string> ColorToString = {
        {colorfilename::Aerial,    "Aerial"   },
        {colorfilename::Airplane,  "Airplane" },
        {colorfilename::Balloon,   "Balloon"  },
        {colorfilename::checker,   "checker"  },
        {colorfilename::couple,    "couple"   },
        {colorfilename::Earth,     "Earth"    },
        {colorfilename::Girl,      "Girl"     },
        {colorfilename::Lenna,     "Lenna"    },
        {colorfilename::Mandrill,  "Mandrill" },
        {colorfilename::milkdrop,  "milkdrop" },
        {colorfilename::Parrots,   "Parrots"  },
        {colorfilename::Pepper,    "Pepper"   },
        {colorfilename::Sailboat,  "Sailboat" }
      };
      const std::unordered_map<monofilename, std::string> MonoToString = {
        {monofilename::Airplane,    "Airplane"    },
        {monofilename::BARBARA,     "BARBARA"    },
        {monofilename::BOAT,        "BOAT"        },
        {monofilename::BRIDGE,      "BRIDGE"      },
        {monofilename::Building,    "Building"    },
        {monofilename::Cameraman,   "Cameraman"   },
        {monofilename::checker_8bit,"checker_8bit"},
        {monofilename::checker,     "checker"     },
        {monofilename::couple_BW,   "couple_BW"   },
        {monofilename::girl,        "girl"        },
        {monofilename::LAX,         "LAX"         },
        {monofilename::LENNA,       "LENNA"       },
        {monofilename::Lighthouse,  "Lighthouse"  },
        {monofilename::Text,        "Text"        },
        //{monofilename::WOMAN,       "WOMAN"       }
      };
      static bool compareImages(const std::string& beforePath, const std::string& afterPath);
      static bool compareFileSizes(const std::string& beforePath, const std::string& afterPath);
    private:
        static std::vector<uint8_t> readFile(const std::string& filePath);
  };
}