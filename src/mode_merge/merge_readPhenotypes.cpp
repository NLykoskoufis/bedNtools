#include "merge_data.h"

void Merge::readPhenotypes(string fbed){

    std::vector < std::vector < float > > val;

    //Open BED file
	vrb.bullet("Reading phenotype data in [" + fbed + "]");
	htsFile *fp = hts_open(fbed.c_str(),"r");
	if (!fp) std::cout << "Cannot open file" << std::endl;
	tbx_t *tbx = tbx_index_load(fbed.c_str());
	if (!tbx) std::cout << "Cannot open index file" << std::endl;
	kstring_t str = {0,0,0};
	if (hts_getline(fp, KS_SEP_LINE, &str) <= 0 || !str.l || str.s[0] != tbx->conf.meta_char ) std::cout << "Cannot read header line!" << std::endl;

    std::vector < std::string > tokens;
    boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));
    int sample_count = 0;
    for (int t = 6; t < tokens.size(); t++) 
    {
        samples.push_back(tokens[t]); // If many samples then add them
        sample_count++;
    }

    //Read phenotypes
    unsigned int linecount =0;
    phenotype_count = 0;
    while (hts_getline(fp, KS_SEP_LINE, &str) >=0) {
        linecount++;
        if (linecount % 100000 == 0) std::cout << "Read " << std::to_string(linecount) << " lines" << std::endl;
        boost::split(tokens, std::string(str.s), boost::is_any_of("\t"));
        if (str.l && str.s[0] != tbx->conf.meta_char) {
            if (tokens.size() <7) std::cout << "Incorrect number of columns" << std::endl;
            phenotype_count++;
            if (first_file){
                chrom.push_back(tokens[0]);
                start.push_back(tokens[1]);
                stop.push_back(tokens[2]);
                id.push_back(tokens[3]);
                info.push_back(tokens[4]);
                strd.push_back(tokens[5]);
            }else{
                if (tokens[3] != id[phenotype_count-1]) vrb.error("Files do not contain same genes or are not sorted identically");
            } 
            std::vector < float > values = std::vector < float > (sample_count, 0.0);
            for (v = 0; v < sample_count.size(); v++) values[v] = tokens[6+v];
            val.push_back(values);
        }
    }
    if (val.size() != id.size()) vrb.error("Files contain different number of genes");
    
    expression.push_back(val);

    //Finalize & verbose
    tbx_destroy(tbx);
    if (hts_close(fp)) std::cout << "Cannot properly close file" << std::endl;
    if (phenotype_count == 0) std::cout << "Cannot find phenotypes in target region!" << std::endl;

    vrb.bullet("Read " + std::to_string(phenotype_count) + " phenotypes");


}