#include "exclude_data.h"

void exclude_data::readPhenotypes(string fbed, string fout){
    
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
    // Insert samples into unordered map sampleID->index in header.
    for(int i=6; i<tokens.size(); i++) mappingS.insert(std::make_pair(tokens[i],i)); 
    
    //Getting indexes of samples to be removed;
    std::vector < int > indexS;
    for(int i=0;i<samplesToRemove.size(); i++){
        std::unordered_map<std::string,int>::const_iterator got = mappingS.find(samplesToRemove[i]);
        if(got == mappingS.end()){
            std::cout << "Sample not found. You cannot exclude samples that are not present in the dataset" << std::endl; 
            exit(-1);
        }else{
            indexS.push_back(got->second);
        }
    }

    // Writing header filtering out samples 
 
    for(int i=0; i<tokens.size(); i++){
        if(find(indexS.begin(),indexS.end(), i) != indexS.end()) continue;
        fdo << tokens[i] << "\t";
    }
    fdo << std::endl;

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
            for(int i =0; i<tokens.size(); i++){
                if(find(indexS.begin(), indexS.end(), i) != indexS.end()) continue;
                fdo << tokens[i] << "\t";
            }
            fdo << std::endl;
        }
    }
    //Finalize & verbose
    tbx_destroy(tbx);
    if (hts_close(fp)) std::cout << "Cannot properly close file" << std::endl;
    if (phenotype_count == 0) std::cout << "Cannot find phenotypes in target region!" << std::endl;

    std::cout << "Read " << phenotype_count << " phenotypes" << std::endl; 


}