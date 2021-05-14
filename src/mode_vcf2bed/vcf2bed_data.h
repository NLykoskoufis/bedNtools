#ifndef _VCF2BED_H
#define _VCF2BED_H

#include "../common/data.h"

class Vcf2bed {
public:
    
    // GENOTYPES
    int genotype_count;

    // READ & WRITE DATA 
    void readGenotypes(std::string, std::string);

    // THREADS
    int nthreads;

    // CONSTRUCTOR & DESTRUCTOR
    Vcf2bed() { 
        genotype_count = 0;
        nthreads = 1;
    };
    ~Vcf2bed() {
        genotype_count = 0;
        nthreads = 1;
    };

    // OPTIONS 
    boost::program_options::options_description option_descriptions;
	boost::program_options::variables_map options;

};

//***************************************************************//
//******************** DECLARE FUNCTIONS *************************//
//***************************************************************//
void vcf2bed_main(std::vector < std::string > & argv);

#endif /* _VCF2BED_H */
