/*Copyright (C) 2015 Olivier Delaneau, Halit Ongen, Emmanouil T. Dermitzakis
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


#include "mode_correct/correct_data.h"
#include "mode_percentage/percentage_data.h"
#include "mode_exclude/exclude_data.h"
#include "mode_include/include_data.h"
#include "mode_vcf2bed/vcf2bed_data.h"
#include "mode_merge/merge_data.h"
#include "mode_reheader/reheader_data.h"

void printModes(){
    vrb.ctitle("Usage:");
    vrb.print("  Bedntools [mode] [options]");
    vrb.print("  eg: bedntools correct --help");
    vrb.ctitle("Available modes:");
    vrb.print("\x1B[37;1m correct\033[0m Covariate correction of a BED file");
	cout << "\x1B[37;1m" <<   " filter" << "\033[0m" << " Filter phenotype by percentage of missing expression accross samples" << endl;
    cout << "\x1B[37;1m" << " exclude" << "\033[0m" << " Exclude samples from bed file" << endl;
    cout << "\x1B[37;1m" << " include" << "\033[0m" << " Include samples from bed file" << endl;
    cout << "\x1B[37;1m" << " vcf2bed" << "\033[0m" << " Convert vcf file to UCSC bed file" << endl;
    cout << "\x1B[37;1m" << " merge" << "\033[0m" << " Merge multiple single sample bed files into multisample bed files" << endl;
    cout << "\x1B[37;1m" << " reheader" << "\033[0m" << " modify the sample names in the bedfile" << endl;
}

int main(int argc, char ** argv) {

	//1. Start timing
	cout << "\n" << "\x1B[32;1m" << "BedNtools" << "\033[0m" << endl; 
    cout << " * Authors : Nikolaos M.R. LYKOSKOUFIS" << endl;
    cout << " * Contact : nikolaos.lykoskoufis@unige.ch" << endl;
    cout << " * Version : version 1.1" << endl;

	//2. Open LOG file if necessary

	for (int a = 1 ; a < argc - 1 ; a ++) {
		if ((strcmp(argv[a], "--log") == 0) && !vrb.open_log(string(argv[a+1]))) vrb.error("Impossible to open log file!");
		if (strcmp(argv[a], "--silent") == 0) vrb.set_silent();
	}

	// MODES 
    vector < string > args;
    if(argc < 2){
        printModes();
        exit(0);
    }
    for(int a= 2; a < argc ; a++) args.push_back(string(argv[a]));

    // FILTER MODE 
    if (strcmp(argv[1], "filter") == 0) percentage_main(args);

	// CORRECT MODE 
	else if (strcmp(argv[1], "correct") == 0) correct_main(args);
    // EXCLUDE MODE 
    else if (strcmp(argv[1], "exclude") == 0) exclude_main(args);
    // INCLUDE MODE 
    else if (strcmp(argv[1], "include") == 0) include_main(args);
    // VCF2BED MODE 
    else if (strcmp(argv[1], "vcf2bed") == 0) vcf2bed_main(args);

    else if (strcmp(argv[1], "correct") ==0) correct_main(args);
    
    else if (strcmp(argv[1], "merge") ==0) merge_main(args);

    else if (strcmp(argv[1], "header") == 0) reheader_main(args);

    else if (strcmp(argv[1], "--help") == 0){
        printModes();
        exit(0);
    } else {
        printModes();
        cout << "Unrecognized bedntools mode!" << endl;
    }
}
