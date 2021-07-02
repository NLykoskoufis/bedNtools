#include "merge_data.h"


void merge_main(std::vector < std::string > & argv) {
    Merge P; 
    
    boost::program_options::options_description opt_basic ("\x1B[32mBasics\33[0m");
    opt_basic.add_options()
       ("help", "Produces option description");
    
    boost::program_options::options_description opt_files ("\x1B[32mI/O\33[0m");
	opt_files.add_options()
		("bed", boost::program_options::value< vector < string > >()->multitoken(), "List of phenotype files in BED format. \033[1;31mBED files need to contain the same genes in same order!\033[0m")
		("out", boost::program_options::value< std::string >(), "Output file.");

    P.option_descriptions.add(opt_basic).add(opt_files);

    //-------------------
	// 2. PARSE OPTIONS
	//-------------------
	try {
		boost::program_options::store(boost::program_options::command_line_parser(argv).options(P.option_descriptions).run(), P.options);
		boost::program_options::notify(P.options);
	} catch ( const boost::program_options::error& e ) {
		std::cerr << "Error parsing [null] command line :" << string(e.what()) << std::endl;
		exit(0);
	}

    //---------------------
	// 3. PRINT HELP/HEADER
	//---------------------
    std::cout << "\n" << "\x1B[32;1m" << "MERGE BED FILES" << "\033[0m" << endl;
    vrb.title("Merges multiple bed files together. The bed files need to contain the same genes in identical order");
    if(P.options.count("help")) {
        std::cout << P.option_descriptions << std::endl;
        exit(0);
    }


    //-----------------
	// 4. COMMON CHECKS
	//-----------------
    if (!P.options.count("bed")) std::cout <<"Phenotype data needs to be specified with --bed [file.bed]" << std::endl;
    if (!P.options.count("out")) std::cout << "Output needs to be specified with --out [file.out]" << std::endl;
    std::string outFile = P.options["out"].as<std::string>();

    //-------------
    // RUN ANALYSIS
    //-------------

    vector < string > bed_list = P.options["bed"].as < vector < string > > ();
	for (int b = 0 ; b < bed_list.size() ; b ++){
        if ( b == 0) P.first_file = true;
        else P.first_file = false;
        P.readPhenotypes(bed_list[b]);
    }
    vrb.cprint("Writing file");
    P.writeBed(outFile);
    

}