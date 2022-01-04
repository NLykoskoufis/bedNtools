#include "percentage_data.h"

void perc_data::readPhenotypesToInclude(std::string fbed){
    
    //Open BED file 
    std::cout << "  * Reading phenotypes to include in [" << fbed << "]" << std::endl;
    htsFile *fp = hts_open(fbed.c_str(), "r");
    if (!fp) std::cout << "Cannot open file" << std::endl;
    kstring_t str = {0,0,0};
    if (hts_getline(fp, KS_SEP_LINE, &str) <= 0 || !str.l)
    std::cout << "Cannot read header line!" << std::endl;

    // Skip header 
    std::vector < std::string > tokens;
    boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));
    if (tokens.size() < 6) std::cout << "Incorrect number of columns!" << std::endl;

    // Read phenotypes. 
    // Only need to read the IDs into map. 

    unsigned int linecount = 0;
    while (hts_getline(fp, KS_SEP_LINE, &str) >= 0) {
        linecount ++;
        if (linecount % 100000 == 0 ) std::cout << "  * Read " << std::to_string(linecount) << " lines" << std::endl;
        
        boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));

        if (str.l){
            if (tokens.size() < 6) std::cout << "Incorrect number of columns" << std::endl;
            phenotype_count ++;

            phenExclusion.insert(std::pair<std::string,int>(tokens[3],0));
        } 
    }
    // Finalize & verbose 
    if (hts_close(fp)) std::cout << "Cannot properly close file" << std::endl;
    if (phenotype_count == 0) std::cout << "Did not find any phenotypes" << std::endl;

    std::cout << "  * Read " << phenotype_count << " phenotypes to be included from main bed file" << std::endl;

}