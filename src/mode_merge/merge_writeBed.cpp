#include "merge_data.h"


void Merge::writeBed(std::string fout) {

    output_file fdo (fout);

    fdo << "#chr\tstart\tend\tgene\tinfo\tstrand"; 
    for (int i=0; i < samples.size(); i++) fdo << "\t" << samples[i]; 
    fdo << std::endl;

    for(int i = 0; i < id.size(); i++){
        fdo << chrom[i] << "\t";
        fdo << start[i] << "\t";
        fdo << stop[i] << "\t";
        fdo << id[i] << "\t";
        fdo << info[i] << "\t";
        fdo << strd[i];
        for (int s = 0; s < expression.size(); s++) fdo << "\t" << expression[s][i];
        fdo << std::endl;
    }

}