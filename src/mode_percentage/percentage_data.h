#ifndef _MODE_PERCENTAGE_H
#define _MODE_PERCENTAGE_H

#include "../common/data.h"

// ANALYSIS MODES 
#define PERC_NONE   0
#define PERC_WRITE  1
#define PERC_FILTER 2


class perc_data {
public:

    // PARAMETERS 
    unsigned int mode;
    double perc_threshold;

    std::vector < std::string > samples;

    // PHENOTYPES 
    int phenotype_count;
    std::vector < std::string > phenotype_id;
    std::vector < std::string > phenotype_chr;
    std::vector < int > phenotype_start;
    std::vector < int > phenotype_end;
    std::vector < std::string > phenotype_strand;
    std::vector < std::vector < float > > phenotype_val;
    std::vector < std::string > phenotype_info;
    
    // PROCESS 
    void writeData(std::string,std::string);

    // COMPUTATION METHODS [ALL INLINE FOR SPEED] 
    double getPercentage_missing(std::vector<float> &);
    
    // CONSTRUCTOR & DESTRUCTOR
    perc_data(){
        phenotype_count=0;
    };
    ~perc_data(){
        phenotype_count =0;
        phenotype_id.clear();
        phenotype_start.clear();
        phenotype_start.clear();
        phenotype_end.clear();
        phenotype_strand.clear();
        phenotype_val.clear();
    };

    // OPTIONS 
    boost::program_options::options_description option_descriptions;
	boost::program_options::variables_map options;

};
//***************************************************************//
//******************** DECLARE FUNCTIONS *************************//
//***************************************************************//
void percentage_main(std::vector < std::string > & argv);

//***************************************************************//
//******************** INLINE FUNCTIONS *************************//
//***************************************************************//

inline double perc_data::getPercentage_missing(std::vector<float> & data){
    int len = data.size();
    double missing = std::count(data.begin(), data.end(), 0);
    return missing/len;
}

#endif /* _PERCENTAGE_DATA_H */
