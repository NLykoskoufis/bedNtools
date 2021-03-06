#include "include_data.h"

void include_data::readInclusion(std::string fname) {
    std::cout << " * Read sample inclusion list [" << fname << "]" << std::endl;
    input_file fd (fname);
    unsigned int n_ids = 0;
    string buffer;
    std::vector < std::string > tokens;

    while(getline(fd,buffer)) {
        boost::split(tokens,buffer, boost::is_any_of("\n"));
        if(tokens.size()>1){
            std::cout << "Only one sample per line" << std::endl; 
            exit(-1);
        }else{
            samplesToKeep.push_back(tokens[0]);
        } n_ids++;
    }
    std::cout << "Read " << n_ids << std::endl;
}