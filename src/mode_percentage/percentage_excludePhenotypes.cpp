#include "percentage_data.h"


void perc_data::excludePhenotypes(std::string fbed, std::string fout){

    output_file fdo (fout);

    //Open BED file
	std::cout << "Reading phenotype data in [" << fbed << "]" << std::endl;
	htsFile *fp = hts_open(fbed.c_str(),"r");
	if (!fp) std::cout << "Cannot open file" << std::endl;
	tbx_t *tbx = tbx_index_load(fbed.c_str());
	if (!tbx) std::cout << "Cannot open index file" << std::endl;
	kstring_t str = {0,0,0};
	if (hts_getline(fp, KS_SEP_LINE, &str) <= 0 || !str.l || str.s[0] != tbx->conf.meta_char ) std::cout << "Cannot read header line!" << std::endl;

    //Process header. 
    std::vector < std::string > tokens;
    boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));
    if (tokens.size() < 7) std::cout << "Incorrect number of columns" << std::endl;

    // Write header
    fdo << "#chr\tstart\tend\tid\tinfo\tstrand";
    for (unsigned int i=0; i < tokens.size()-6; i++){
        fdo << "\t" << tokens[i];
    }

    unsigned int linecount = 0;

    while (hts_getline(fp, KS_SEP_LINE, &str) >= 0){
        linecount ++;
        if (linecount % 1000 == 0) std::cout << "  * Read " << std::to_string(linecount) << " lines" << std::endl;
        boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));
        
        if (str.l && str.s[0] != tbx ->conf.meta_char) {
            if (tokens.size() < 7) std::cout << "Incorrect number of columns" << std::endl;
            
            // Check whether phenotype is in exclusion map or not
            std::map<std::string, int>::iterator it;
            it = phenExclusion.find(tokens[3]);
            if (it == phenExclusion.end()){
                fdo << tokens[0] << "\t" << tokens[1] << "\t" << tokens[2] << "\t" << tokens[3] << "\t" << tokens[4] << "\t" << tokens[5];
                for (unsigned int = 6; i < tokens.size(); i++){
                    fdo << "\t" << tokens[i];
                }
                fdo << "\n";
            }
        }
    }
    //Finalize & Verbose 
    tbx_destroy(tbx);
    if (hts_close(fp)) std::cout << "Cannot properly close file" << std::endl;

    std::cout << "Read " << phenotype_count << " phenotypes" << std::endl; 

}