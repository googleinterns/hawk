#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

const char DELIMITER = ',';


class Config
{
public:
    std::vector<int> ppid;
    std::vector<std::string> name;
    int n_proc; /* number of tasks to record */

    Config();
    void detect_usecase(int argc, char *argv[]);
    void exec_monitor_parse_args_run();

    // VALIDATORS
    static bool checkSaveAndExportFormat(const char *flagname, const std::string &value);
};

#endif