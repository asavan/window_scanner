#ifndef CONFIG_H
#define CONFIG_H_H
#include <string>

class ConfigParser {
public:
    ConfigParser(const std::string& filename);
    std::string baseName() const;
    unsigned long long processId() const;
private:
    unsigned long long pr_id;
    std::string base_name;
};

#endif
