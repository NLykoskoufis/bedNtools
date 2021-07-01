#ifndef _MODE_MERGE_H
#define _MODE_MERGE_H

#include "../common/data.h"


class Merge {
public:
    
    // Phenotypes
    std::vector < std::string > samples;
    std::vector < std::vector < float > > expression;
    std::vector < std::string > chrom; 
    std::vector < std::string > start; 
    std::vector < std::string > stop;
    std::vector < std::string > id;
    std::vector < std::string > info; 
    std::vector < std::string > strd;

    // UTILITY
    bool first_file;
    int phenotype_count;
    // FUNCTIONS 
    void readPhenotypes(std::string);
    void writeBed(std::string);

    // Initial check 
    void checkBED(std::string);
    std::vector < std::string > geneCheck; 
    

    // OPTIONS 
    boost::program_options::options_description option_descriptions;
	boost::program_options::variables_map options;
};

void merge_main(std::vector < std::string > & argv);


#endif /* _MODE_MERGE_H */

