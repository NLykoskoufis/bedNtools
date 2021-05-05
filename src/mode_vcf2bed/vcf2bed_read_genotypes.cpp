#include "vcf2bed_data.h"

//******************************************************************//
//******************** CONVERT VCF TO BED *************************//
//******************************************************************//



void Vcf2bed::readGenotypes(std::string fvcf, std::string fout){
    // Opening files
    bcf_srs_t * sr =  bcf_sr_init();
    if (nthreads>1) bcf_sr_set_threads(sr,nthreads); // Read vcf file with multiple threads.
    if(!(bcf_sr_add_reader (sr, fvcf.c_str()))) {
		switch (sr->errnum) {
		case not_bgzf: cout << "File not compressed with bgzip!" << endl; break;
		case idx_load_failed: cout << "Impossible to load index file!"<< endl; break;
		case file_type_error: cout << "File format not detected by htslib!" << endl; break;
		default : cout << "Unknown error!" << endl;
		}
	}
    int n_samples = bcf_hdr_nsamples(sr->readers[0].header);
    
    output_file fdo (fout);
    fdo << "#chr\tstart\tend\tid\tinfo\tstrand" << std::endl;

    unsigned int linecount=0;
    // Read genotype data 
    int ngt, ngt_arr = 0, nds, nds_arr = 0, * gt_arr = NULL, nsl, nsl_arr = 0, * sl_arr = NULL;
    float * ds_arr = NULL;
    bcf1_t * line;
    while(bcf_sr_next_line (sr)){
        linecount ++;
        if (linecount % 100000 == 0) cout << "Read " << std::to_string(linecount) << " lines" << endl;
        line = bcf_sr_get_line(sr,0);
        ngt = bcf_get_genotypes(sr->readers[0].header, line, &gt_arr, &ngt_arr);
        string sid = string(line->d.id);
        string chr = string(bcf_hdr_id2name(sr->readers[0].header, line->rid));
        int POS = line->pos;
        std::string REF = line->d.allele[0];
        std::string ALT = line->d.allele[1];
        
        fdo << chr << "\t"; // Write CHROMOSOME
        fdo << std::to_string(line->pos) << "\t"; // Write START
        nsl = bcf_get_info_int32(sr->readers[0].header, line, "END", &sl_arr, &nsl_arr); 
        if (nsl >= 0 && nsl_arr == 1) fdo << std::to_string(sl_arr[0]) << "\t"; // WRITE END
        else fdo << (POS + 1 + REF.size() - 1) << "\t"; // WRITE END 
        fdo << sid << "\t"; // Read variand ID
        fdo << REF << "/" << ALT << "\t.\t+";
        fdo << std::endl;
    }
    bcf_sr_destroy(sr);
    genotype_count = linecount;
    cout << "Read " << to_string(genotype_count) << endl;
}