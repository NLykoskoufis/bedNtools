#include "reheader_data.h"


void reheader_main(std::vector < std::string > & argv) {
    reheader_data P;
    
    boost::program_options::options_description opt_basic ("\x1B[32mBasics\33[0m");
	opt_basic.add_options()
		("help", "Produces option description")
		("seed", boost::program_options::value< unsigned int >()->default_value(123456), "Random number seed. Useful to replicate runs.");
    
    boost::program_options::options_description opt_files ("\x1B[32mI/O\33[0m");
	opt_files.add_options()
		("bed", boost::program_options::value< std::string >(), "Phenotypes in BED format.")
		("out", boost::program_options::value< std::string >(), "Output file.");

    boost::program_options::options_description opt_parallel ("\x1B[32mParallelization\33[0m");
	opt_parallel.add_options()
		("header", boost::program_options::value< std::string >(), "File containing new sample names to reheader bedfile. Either one column with new names of samples NEED TO BE IN CORRECT ORDER!!!. Or a two columns file: oldID newID.");

        
    P.option_descriptions.add(opt_basic).add(opt_files).add(opt_parallel);

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
    std::cout << "\n" << "\x1B[32;1m" << "FILTER BED " << "\033[0m" << endl;
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

    if(P.options.count("header")){
        P.readNewHeader(P.options["header"].as<std::string>());
        P.readPhenotypes(P.options["bed"].as<std::string>(),outFile);
    }else{
        std::cout << "You need to specify a new header file using --header." << std::endl;
        exit(-1);
    }

}