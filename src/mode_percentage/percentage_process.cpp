#include "percentage_data.h"




void perc_data::writeData(string fbed, string fout){
    
    output_file fdo (fout);

    //Open BED file
	std::cout << "Reading phenotype data in [" << fbed << "]" << std::endl;
	htsFile *fp = hts_open(fbed.c_str(),"r");
	if (!fp) std::cout << "Cannot open file" << std::endl;
	tbx_t *tbx = tbx_index_load(fbed.c_str());
	if (!tbx) std::cout << "Cannot open index file" << std::endl;
	kstring_t str = {0,0,0};
	if (hts_getline(fp, KS_SEP_LINE, &str) <= 0 || !str.l || str.s[0] != tbx->conf.meta_char ) std::cout << "Cannot read header line!" << std::endl;

    //Process sample names
    std::vector < std::string > tokens;
    boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));
    if (tokens.size() < 7) std::cout << "Incorrect number of columns" << std::endl;
    int sample_count = 0;
    
    // If mode write percentage (--percentage)
    if (mode == PERC_WRITE){
        fdo << "#chr\tstart\tend\tgene\tinfo\tstrand\tperc_missing\tperc_present" << std::endl;
    }
    // If mode filter phenotypes (--missingness)
    if (mode == PERC_FILTER){
        for (int t = 0 ; t < tokens.size() ; t ++) {
            fdo << tokens[t] << "\t";
	    }
        fdo << std::endl;
    }

    //Read phenotypes
    unsigned int linecount =0;

    while (hts_getline(fp, KS_SEP_LINE, &str) >=0) {
        linecount++;
        if (linecount % 100000 == 0) std::cout << "Read " << std::to_string(linecount) << " lines" << std::endl;
        boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));

        if (str.l && str.s[0] != tbx->conf.meta_char) {
            if (tokens.size() <7) std::cout << "Incorrect number of columns" << std::endl;
            phenotype_count++;
            std::vector < float > val;
            for(int i=6; i<tokens.size(); i++){
                val.push_back(stof(tokens[i]));
            }
            
            // If mode write percentage (--percentage)
            if (mode == PERC_FILTER){
                if(getPercentage_missing(val) >= perc_threshold){   continue;}
                else{
                fdo << tokens[0] << "\t";
                fdo << tokens[1] << "\t";
                fdo << tokens[2] << "\t";
                fdo << tokens[3] << "\t";
                fdo << tokens[4] << "\t";
                fdo << tokens[5] << "\t";
                for(auto x:val){
                    fdo << std::to_string(x) << "\t";
                }
                fdo << std::endl;
                }
            }
            // If mode filter phenotypes (--missingness)
            if (mode == PERC_WRITE){
                fdo << tokens[0] << "\t";
                fdo << tokens[1] << "\t";
                fdo << tokens[2] << "\t";
                fdo << tokens[3] << "\t";
                fdo << tokens[4] << "\t";
                fdo << tokens[5] << "\t";
                fdo << std::to_string(getPercentage_missing(val)) << "\t";
                fdo << std::to_string(1-getPercentage_missing(val));
                fdo << std::endl;
            }
        }
    }
    //Finalize & verbose
    tbx_destroy(tbx);
    if (hts_close(fp)) std::cout << "Cannot properly close file" << std::endl;
    if (phenotype_count == 0) std::cout << "Cannot find phenotypes in target region!" << std::endl;


    std::cout << "Read " << phenotype_count << " phenotypes" << std::endl; 


}
