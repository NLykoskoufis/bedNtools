#include "reheader_data.h"

void reheader_data::readNewHeader(std::string fname) {
    std::cout << " * Read sample inclusion list [" << fname << "]" << std::endl;
    input_file fd (fname);
    unsigned int n_ids = 0;
    string buffer;
    std::vector < std::string > tokens;

    while(getline(fd,buffer)) {
        boost::split(tokens,buffer, boost::is_any_of("\t"));
        if(tokens.size()>1){
            singleColumn = false; // Set singleColumn to false for later.
            newHeaderMap.insert(std::make_pair(tokens[0],tokens[1]));
        }else{
            newHeader.push_back(tokens[0]);
        } n_ids++;
    }
    std::cout << "Read " << n_ids << std::endl;
    fd.close();
}