#include <iostream>
#include <vector>
#include <string>
#include "Script.hpp"

namespace Utils
{
    inline bool StringStartsWith(const std::string& s1, const std::string& s2)
    {
        return (s1.compare(0, s2.length(), s2) == 0);
    }
}

class Arguments
{
public:
    Arguments(int argc, char** argv)
    {
        std::vector<std::string> args;
        for (int i = 0; i < argc; i++)
        {
            args.push_back(argv[i]);
        }
        Parse(args);
    }

    std::string mInputFile;
    std::string mOutputFile;

private:

    void Parse(std::vector<std::string>& args)
    {
        for (const std::string& arg : args)
        {
            if (Utils::StringStartsWith(arg, "-input"))
            {
                mInputFile = arg;
            }
        }
    }
};

int main(int argc, char** argv)
{
    std::cout << "GCL ASM" << std::endl;

    Arguments arguments(argc, argv);

    std::cout << "Input: " << arguments.mInputFile << std::endl;

    return 0;
}
