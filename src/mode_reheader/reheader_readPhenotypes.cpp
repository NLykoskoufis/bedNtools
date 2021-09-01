#include "include_data.h"

void include_data::readPhenotypes(string fbed, string fout){
    
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
    
    if (singleColumn){
        if (tokens.size()-6 != newHeader.size()){
            std::cout << "Number of samples do not match!"; 
            exit(-1);
        }
    }else{
        if (tokens.size()-6 != newHeaderMap.size()){
            std::cout << "Number of samples do not match!"; 
            exit(-1);
        }
    }
    

    fdo << "#chr\tstart\tend\tid\tinfo\tstrand";
    for(int i=0; i< tokens.size()-6; i++){
        if(singleColumn){
            fdo << "\t" << newHeader[i];
        }else{
            std::unordered_map<std::string,std::string>::const_iterator got = newHeaderMap.find(tokens[i]);
            if(got == newHeaderMap.end()){
                std::cout << "Sample not found in header file!!";
                exit(-1);
            }else{
                fdo << "\t" << got->second; 
            }
        }
    }
    fdo << "\n";

    //Read phenotypes
    unsigned int linecount =0;

    while (hts_getline(fp, KS_SEP_LINE, &str) >=0) {
        linecount++;
        if (linecount % 100000 == 0) std::cout << "Read " << std::to_string(linecount) << " lines" << std::endl;
        boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));

        if (str.l && str.s[0] != tbx->conf.meta_char) {
            if (tokens.size() <7) std::cout << "Incorrect number of columns" << std::endl;
            phenotype_count++;
            
            fdo << tokens[0] << "\t" << tokens[1] << "\t" << tokens[2] << "\t" << tokens[3] << "\t" << tokens[4] << "\t" << tokens[5];

            for(int i =6; i<tokens.size(); i++){
                fdo << "\t" << tokens[i];
            }
            fdo << "\n";
        }
    }
    //Finalize & verbose
    tbx_destroy(tbx);
    if (hts_close(fp)) std::cout << "Cannot properly close file" << std::endl;
    if (phenotype_count == 0) std::cout << "Cannot find phenotypes in target region!" << std::endl;

    std::cout << "Read " << phenotype_count << " phenotypes" << std::endl; 

}