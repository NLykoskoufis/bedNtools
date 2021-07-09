#ifndef _INCLUDE_DATA_H
#define _INCLUDE_DATA_H

#include "../common/data.h"



class include_data {
public:
    //SAMPLES
    unordered_map <std::string,int> mappingS;
    std::vector < std::string > samplesToKeep;

    //PHENOTYPES
    int phenotype_count;

    //READ DATA 
    void readPhenotypes(std::string,std::string);

    //FILTER 
    void readInclusion(std::string);

    //CONSTRUCTOR & DESTRUCTOR
    include_data(){phenotype_count=0;};
    ~include_data(){
        phenotype_count=0;
    }

    // OPTIONS 
    boost::program_options::options_description option_descriptions;
	boost::program_options::variables_map options;

};
//***************************************************************//
//******************** DECLARE FUNCTIONS *************************//
//***************************************************************//
void include_main(std::vector < std::string > & argv);




#endif /* _INCLUDE_DATA_H */
