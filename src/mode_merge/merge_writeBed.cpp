#include "merge_data.h"


void Merge::writeBed(std::string fout) {

    output_file fdo (fout);

    fdo << "#chr\tstart\tend\tgene\tinfo\tstrand"; 
    for (int i=0; i < samples.size(); i++) fdo << "\t" << samples[i]; 
    fdo << std::endl;

    for(int g = 0; g < id.size(); g++){
        fdo << chrom[g] << "\t";
        fdo << start[g] << "\t";
        fdo << stop[g] << "\t";
        fdo << id[g] << "\t";
        fdo << info[g] << "\t";
        fdo << strd[g];
        for (int f= 0; f < expression.size(); f++){
            for (int s=0; s < expression[f][g].size(); s++){
                fdo << "\t" << expression[f][g][s];
            }
        }
        fdo << std::endl;
    }
}