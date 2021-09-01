#ifndef _REHEADER_DATA_H
#define _REHEADER_DATA_H

#include "../common/data.h"

class reheader_data {
public:
    //SAMPLES
    unordered_map <std::string,std::string> newHeaderMap;
    std::vector < std::string > newHeader;
    bool singleColumn;

    unsigned int phenotype_count;

    //READ DATA 
    void readPhenotypes(std::string,std::string);

    //FILTER 
    void readNewHeader(std::string);

    //CONSTRUCTOR & DESTRUCTOR
    include_data(){singleColumn=true;};
    ~include_data(){
        phenotype_count=true;
    }

    // OPTIONS 
    boost::program_options::options_description option_descriptions;
	boost::program_options::variables_map options;

};
//***************************************************************//
//******************** DECLARE FUNCTIONS *************************//
//***************************************************************//
void reheader_main(std::vector < std::string > & argv);




#endif /* _REHEADER_DATA_H */
