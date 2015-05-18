/*
# Licensed under the Apache License, Version 2.0 (the 'License')
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an 'AS IS' BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

#ifndef __UA_PARSER_H__
#define __UA_PARSER_H__

#include <string>
#include <vector>
#include <regex>
#include <yaml-cpp/yaml.h>

namespace ua_parser {
/////////////
// BROWSER //
/////////////
class Browser {
 public:
  std::string family;
  std::string major;
  std::string minor;
  std::string patch;
  std::string patch_minor;

  bool operator==(const Browser &rhs) const {
    return family==rhs.family
           && major==rhs.major
           && minor==rhs.minor
           && patch==rhs.patch
           && patch_minor==rhs.patch_minor;
  }


};


inline std::ostream& operator<<(std::ostream &o, const Browser &browser) {
  return o <<"Browser: ("
         << browser.family<<","
         <<browser.major<<","
         <<browser.minor<<","
         <<browser.patch<<","
         <<browser.patch_minor<<") ";
}

/////////////
//    OS   //
/////////////
class OperatingSystem {
 public:
  std::string os;
  std::string major;
  std::string minor;
  std::string patch;
  std::string patch_minor;

  bool operator==(const OperatingSystem &rhs) const {
    return os==rhs.os
           && major==rhs.major
           && minor==rhs.minor
           && patch==rhs.patch
           && patch_minor==rhs.patch_minor;
  }
};

inline std::ostream& operator<<(std::ostream &o, const OperatingSystem &os) {
  return o<<"OS: ("
         <<os.os<<","
         <<os.major<<","
         <<os.minor<<","
         <<os.patch<<","
         <<os.patch_minor<<") ";
}


/////////////
// DEVICE  //
/////////////
class Device {
    public:
        std::string device;
        std::string brand;
        std::string model;

        bool operator==(const Device &rhs) const {
            return device==rhs.device
                   && brand==rhs.brand
                   && model==rhs.model;
        }
};
inline std::ostream& operator<<(std::ostream &o, const Device &dev) {
    return o << "Device: ("
            << dev.device << ","
            << dev.brand << ","
            << dev.model << ") ";
}

////////////////////
// PARSER CLASSES //
////////////////////
class BrowserParser {
 public:
  BrowserParser(const YAML::Node &yaml_attributes);
  Browser Parse(const std::string &user_agent_string);

  std::string pattern;
  std::regex regex;
  Browser overrides;
};


class OperatingSystemParser {
 public:
  OperatingSystemParser(const YAML::Node &yaml_attributes);
  OperatingSystem Parse(const std::string &user_agent_string);

  std::string pattern;
  std::regex regex;
  OperatingSystem overrides;
};

class DeviceParser {
    public:
        DeviceParser(const YAML::Node &yaml_attributes);
        Device Parse(const std::string &user_agent_string);
        std::string pattern;
        std::regex regex;
        Device overrides;
    private:
        static const char* placeholders[];
        void replace_all_placeholders(std::string& device_property, const std::smatch &result);
};

/////////////
// HELPERS //
/////////////

inline void find_and_replace(std::string &original, const std::string &tofind, const std::string &toreplace) {
    std::string::size_type loc = original.find(tofind);
    if (loc == std::string::npos) {
        return;
    }

    //Because there can be spaces leading the replacement symbol " $1" and when toreplace is empty
    // This can lead to multiple spaces e.g.: "HTC Desire   " or "HTC   Desire"
    //   But you want to avoid the case of "HTC $1$2" where $2 is not empty but $2 , you don't wan't "HTCxyz"
    // If you're replacing empty:
    //   If you have a leading whitespace
    //     If the next character after the placeholder (loc+2) is also a white space OR it's the end of the string
    //       Then get rid of the leading whitespace
    if (toreplace.size()==0) {
        if (loc != 0 && original[loc-1] == ' ') {
            if( (loc+2 < original.size() && original[loc+2]==' ') || (loc+2 >= original.size()) ) {
                original.replace(loc-1, tofind.size()+1, toreplace);
                return;
            }
        }
    }
    //Otherwise just do a normal replace
    original.replace(loc, tofind.size(), toreplace);
}

inline void remove_trailing_whitespaces(std::string &original) {
    //Sometimes te regexes don't actually parse correctly leaving trailing whitespaces
    //The if statment is so that we don't always do a find.
    if (original.back()== ' ') {
        size_t pos = original.find_last_not_of(' ');
        if (pos != std::string::npos) {
            original.resize(pos+1);
        }
    }
    return;


}

//////////////////////////////
// The stuff you care about //
//////////////////////////////
class UserAgent {
 public:
  Browser browser;
  OperatingSystem os;
  Device device;
  std::string user_agent_string;

};

inline std::ostream& operator<<(std::ostream &o, UserAgent &ua) {
  return o  <<ua.browser
         <<ua.os
         <<"Device: ("<<ua.device<<")";
}


class Parser {
 public:
  Parser(const std::string &yaml_file);
  UserAgent Parse(const std::string &user_agent_string);

 private:
  std::vector<BrowserParser> _browser_parsers;
  std::vector<OperatingSystemParser> _os_parsers;
  std::vector<DeviceParser> _device_parsers;
};



}


#endif
